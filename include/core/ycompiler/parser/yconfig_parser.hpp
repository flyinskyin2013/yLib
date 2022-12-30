/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: Sky
 * @Date: 2021-11-20 17:10:29
 * @LastEditTime: 2021-11-20 17:11:39
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\parse\yconfig_parser.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_PARSE_YCONFIG_PARSER_HPP__
#define __CORE_YCOMPILER_PARSE_YCONFIG_PARSER_HPP__


#include "core/yobject.hpp"


#include "core/ycompiler/basic/ysource_location.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ycompiler/lexer/ylexer.hpp"
#include "core/ycompiler/lexer/ypreprocessor.hpp"


#include "core/ycompiler/lexer/ytoken.hpp"

#include "core/ycompiler/parser/yparser.hpp"
#include "core/ycompiler/basic/yidentifier.hpp"
#include "core/ycompiler/sema/ysema.hpp"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace yLib
{
    namespace ycompiler
    {

        class yDeclGroup;
        class __YLIB_CLASS_DECLSPEC__ yConfigDecl{
            public:
            enum yConfigDeclType{
                OBJECT_TYPE,
                ITEM_TYPE,
                COMMENT_TYPE,
            };
            yConfigDeclType GetDeclType(void){return decl_type;}
            std::string & GetDeclName(void){return name;}
            void SetDeclName(const std::string & name){this->name = name;}

            yConfigDecl(){}
            ~yConfigDecl(){}

            yConfigDecl(const yConfigDecl&) = delete;
            yConfigDecl& operator=(const yConfigDecl&) = delete;

            protected:
            yConfigDeclType decl_type = OBJECT_TYPE;
            std::string name = "";
        };

        class __YLIB_CLASS_DECLSPEC__ yConfigDeclObject: public yConfigDecl{

            public:
            yConfigDeclObject(){

                decl_type = yConfigDecl::OBJECT_TYPE;
            }
            yConfigDeclObject(const yConfigDeclObject&obj) = delete;
            yConfigDeclObject &operator=(const yConfigDeclObject&obj) = delete;

            std::unordered_map<std::string, yConfigDecl*> decl_map;//object, item
            std::vector<std::unique_ptr<yConfigDecl>> decl_order_vec;//order
            std::vector<yConfigDecl*> comment_vec;//comment
        };

        class __YLIB_CLASS_DECLSPEC__ yConfigDeclItem: public yConfigDecl{
            public:
            enum ItemType{

                NONE_TYPE,
                STRING_TYPE,
                DOUBLE_TYPE,
                INT64T_TYPE,
                BOOL_TYPE
            };

            private:
            std::string string_literal_val;
            double double_val;
            int64_t int64t_val;
            bool bool_val;


            ItemType type;
            public:
            yConfigDeclItem(){

                decl_type = yConfigDecl::ITEM_TYPE;
            }            
            std::string & GetStringLiteral(void){return string_literal_val;}
            void SetStringLiteral(const std::string &str){type = STRING_TYPE; string_literal_val = str;}

            double GetDouble(void){return double_val;}
            void SetDouble(double d){type = DOUBLE_TYPE; double_val = d;}

            int64_t GetInt64t(void){return int64t_val;}
            void SetInt64t(int64_t i){type = INT64T_TYPE; int64t_val = i;}

            bool GetBool(void){return bool_val;}
            void SetBool(bool b){type = BOOL_TYPE; bool_val = b;}

            ItemType GetItemType(void){return type;}
        };

        class __YLIB_CLASS_DECLSPEC__ yConfigDeclComment: public yConfigDecl{

            public:
            yConfigDeclComment(){

                decl_type = yConfigDecl::COMMENT_TYPE;
            }           

            std::string comment; 

        };
        //yConfigParser Notes
        // G=(Vt, Vn, P, S)
        //ref: n1256.pdf 
        class __YLIB_CLASS_DECLSPEC__ yConfigParser:
        public yParser
        {
            friend class ySema;

            private:
            yPreprocessor & preprocessor;


            yToken next_token;

            /// Tok - The current token we are peeking ahead.  All parsing methods assume
            /// that this is valid.
            yToken cur_token;


            yConfigDeclObject root_object;
            


            yLexer &lexer;


            ySema &sema;

            yDiagnosticsEngine & diag_engine;



            
            public:
            
            yConfigParser(yLexer * lexer);
            yConfigParser(ySema & sema);
            ~yConfigParser();

            bool ParseAST(void);
            bool ParseDecl(void);
            void * GetASTData();

            bool ParserTranslationUnit(void);
            bool ParserDeclareItem(yConfigDeclItem & item, std::string &item_name);
            bool ParserDeclareList(yConfigDeclObject & cur_obj, std::string &obj_name);

            bool ParserObject(yConfigDeclObject & cur_obj, std::string &obj_name);
            bool ParserObjectList(void);


            void Initialize(void);
            bool ParseFirstTopLevelDecl(yDeclGroup &result);
            bool ParseTopLevelDecl(yDeclGroup &result, bool is_first_decl);



            public:
            yDiagnosticBuilder Diag(ySourceLocation loc, unsigned diag_id);
            yDiagnosticBuilder Diag(const yToken &cur_token, unsigned diag_id);
            yDiagnosticBuilder Diag(unsigned diag_id) {
                return Diag(cur_token, diag_id);
            }
        };
    }
}

#endif //__CORE_YCOMPILER_PARSE_YCONFIG_PARSER_HPP__