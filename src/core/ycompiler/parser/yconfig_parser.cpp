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
 * @Date: 2021-11-20 17:37:58
 * @LastEditTime: 2021-11-20 17:37:58
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\parse\yconfig_parser.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/frontend/ycompiler_instance.hpp"

#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ycompiler/sema/ysema.hpp"

#include "core/ycompiler/ast/ydecl_group.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yConfigParser::yConfigParser(yCompilerInstance & ci)
:ci(ci), 
lexer(ci.GetLexer()), 
sema(ci.GetSema()), 
// preprocessor(sema.GetPreprocessor()), //the initial order is not ci,lexer,sema,preprocessor,diag_engine
diag_engine(ci.GetDiagnosticsEngine()){}

yConfigParser::~yConfigParser(){}

bool yConfigParser::ParseAST(void){

    return ParseDecl();
}

bool yConfigParser::ParseDecl(void){

    return ParserTranslationUnit();
}

void * yConfigParser::GetASTData(){

    return &root_object;
}
//yConfigParser Notes
// G=(Vt, Vn, P, S)
/* ref: n1256.pdf 


Syntax-in-c99:
identifier:
    ref: n1256.pdf 6.4.2 Identifiers

floating-constant:
    ref: 6.4.4.1 Integer constants

integer-constant:
    ref: 6.4.4.2 Floating constants

string-literal:
    ref: n1256.pdf 6.4.5 String literals


yConfig Language Define
EBNF:
Syntax1:
translation-unit:
    object-list


Syntax2:
object-list:
    object
    object-list ';'
    object-list ';' object

Syntax3:
object:    
    identifier ':' '{' declaration-list '}'

Syntax4:
declaration-list:
    declaration-item
    declaration-item ';'
    object
    object ';'
    declaration-list ';' declaration-item
    declaration-list ';' object

Syntax5:
declaration-item:
    identifier '=' value-item 

Syntax6:
value-item:
    floating-constant
    integer-constant
    string-literal
    bool-constant

Syntax7:
bool-constant:
    'true'
    'false'
*/
bool yConfigParser::ParserTranslationUnit(void){

    return ParserObjectList();
}

bool yConfigParser::ParserObjectList(void){

    lexer.NextToken(cur_token);//get first token

    if (cur_token.kind != tok::identifier && cur_token.kind != tok::comment){

        yDiagnosticsEngine::DiagReport(cur_token.token_loc, diag::yconfigparser_error_expect_obj_or_comment);
        return false;
    }

    while(cur_token.kind == tok::identifier || cur_token.kind == tok::comment){

        if (cur_token.kind == tok::comment){//comment token

            yConfigDeclComment* _comment = new (std::nothrow) yConfigDeclComment();
            _comment->comment = std::string((const char *)cur_token.token_data, cur_token.token_data_len);
            std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _comment);

            root_object.comment_vec.push_back((yConfigDecl *) _comment);

            root_object.decl_order_vec.push_back(std::move(_decl));//

            lexer.NextToken(cur_token);//consumer token
            continue;//skip
        }

        //create object decl
        yConfigDeclObject* _object = new (std::nothrow) yConfigDeclObject();
 
        std::string _obj_name((const char *)cur_token.token_data, cur_token.token_data_len);
        std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _object); 
        root_object.decl_order_vec.push_back(std::move(_decl));  
        
        if (root_object.decl_map.count(_obj_name) > 0){//exsit object name error

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_objname_exsit);
            return false;
        }
        else{

            root_object.decl_map.insert(std::make_pair(_obj_name, _object));//insert a object to root object
            if (!ParserObject(*_object, _obj_name)){//object token

                LOGE("yConfigParser")<<"parse a object failed.";
                return false;
            }
        }
    }

    if (cur_token.kind != tok::eof){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_obj_or_comment);
        return false;
    }

    return true;
}

bool yConfigParser::ParserDeclareItem(yConfigDeclItem & item, std::string &item_name){

    if (cur_token.kind != tok::identifier){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_identifier);
        return false;
    }
    std::string _identifier_str((const char *)cur_token.token_data, cur_token.token_data_len);

    lexer.NextToken(cur_token);//next tok, skip identifier
    if (cur_token.kind != tok::equal){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_equal);
        return false;
    }            

    /*
    Syntax6:
    value-item:
        floating-constant
        integer-constant
        string-literal
        bool-constant
    */
    lexer.NextToken(cur_token);//next tok, skip equal
    if (cur_token.kind == tok::numeric_constant || cur_token.kind == tok::plus || cur_token.kind == tok::minus){

        uint64_t _num_val;
        int8_t _call_ret = sema.ParseNumberConstantExpression(_num_val);
        if (-1 == _call_ret){

            return false;
        }
        else if (1 == _call_ret){//int

            int64_t _int64_t_val = 0;
            ::memcpy((char *)&_int64_t_val, (char *)&_num_val, sizeof(int64_t));
            item.SetInt64t(_int64_t_val);
        }
        else{//float

            double _double_val = 0;
            ::memcpy((char *)&_double_val, (char *)&_num_val, sizeof(double));
            item.SetDouble(_double_val);
        }

        lexer.NextToken(cur_token);//skip numeric_constant
    }
    else if (cur_token.kind == tok::string_literal){

        std::string _str(((char *)cur_token.token_data) + 1, cur_token.token_data_len - 2);//get rid of two '"'
        item.SetStringLiteral(_str);
        lexer.NextToken(cur_token);//skip string_literal
    }
    else if (cur_token.kind == tok::identifier){//bool-constant only true or false

        std::string _str((char *)cur_token.token_data, cur_token.token_data_len);
        if (_str == "true" || _str == "false"){
            
            if (_str == "true")
                item.SetBool(true);
            else
                item.SetBool(false);
        }
        else{

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_true_or_false);
            return false;
        }
        
        lexer.NextToken(cur_token);//skip bool val
    }
    else{

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_valid_item_val);
        return false;
    }

    if (cur_token.kind == tok::semi){

        lexer.NextToken(cur_token);//next token, skip semi
    }
    else{

        lexer.TryNextToken(next_token);//try get next token
        if (next_token.kind == tok::identifier){//two object must split by ;

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_semi);
            return false;
        }
    }

    return true;
}
bool yConfigParser::ParserDeclareList(yConfigDeclObject & cur_obj, std::string &obj_name){
    
    while(cur_token.kind == tok::identifier || cur_token.kind == tok::comment){

        if (cur_token.kind == tok::comment){//comment token

            yConfigDeclComment* _comment = new (std::nothrow) yConfigDeclComment();
            _comment->comment = std::string((const char *)cur_token.token_data, cur_token.token_data_len);
            std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _comment);

            cur_obj.comment_vec.push_back((yConfigDecl *) _comment);

            cur_obj.decl_order_vec.push_back(std::move(_decl));//

            lexer.NextToken(cur_token);//consumer token
            continue;//skip
        }

        if (cur_token.kind != tok::identifier){

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_identifier);
            return false;
        }

        std::string _identifier_name((const char *)cur_token.token_data, cur_token.token_data_len);

        lexer.TryNextToken(next_token);//try next token
        if (next_token.kind == tok::colon || next_token.kind == tok::equal){

            if (next_token.kind == tok::colon){//object

                yConfigDeclObject* _object = new (std::nothrow) yConfigDeclObject();
                 
                std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _object);    
                cur_obj.decl_order_vec.push_back(std::move(_decl));    

                if (cur_obj.decl_map.count(_identifier_name) > 0){//exsit object name error

                    yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_exsit_identifier);
                    return false;
                }
                else{           

                    cur_obj.decl_map.insert(std::make_pair(_identifier_name, _object));//insert a object 
                    if (!ParserObject(*_object, _identifier_name)){

                        LOGE("yConfigParser")<<"parse a object failed.";
                        return false;
                    }
                }
            }
            else{//declare item

                yConfigDeclItem* _item = new (std::nothrow) yConfigDeclItem();
                
                std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _item);    

                cur_obj.decl_order_vec.push_back(std::move(_decl));  
                if (cur_obj.decl_map.count(_identifier_name) > 0){//exsit object name error

                    yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_exsit_identifier);
                    return false;
                }
                else{   
                    
                    cur_obj.decl_map.insert(std::make_pair(_identifier_name, _item));//insert a item 
                    if (!ParserDeclareItem(*_item, _identifier_name)){

                        LOGE("yConfigParser")<<"parse a item failed.";
                        return false;
                    }
                }
            }
        }
        else{

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_colon_or_equal);
            return false;                
        }
    }

    return true;
}

bool yConfigParser::ParserObject(yConfigDeclObject & cur_obj, std::string &obj_name){

    if (cur_token.kind != tok::identifier){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_obj_start_of_identifier);
        return false;
    }
    lexer.NextToken(cur_token);//next token, skip identifier
    if (cur_token.kind != tok::colon){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_obj_second_field);
        return false;
    }

    lexer.NextToken(cur_token);//next token, skip colon
    if (cur_token.kind != tok::l_brace){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_obj_third_field);
        return false;
    }
    
    lexer.NextToken(cur_token);//next token, object body, skip l_brace
    if (cur_token.kind != tok::r_brace)//not a empty body
        if (!ParserDeclareList(cur_obj, obj_name))
            return false;
    
    //we get new token in ParserObject or ParserDeclareList
    if (cur_token.kind != tok::r_brace){

        yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_obj_final_field);
        return false;
    }

    lexer.NextToken(cur_token);//next token, skip r_brace
    if (cur_token.kind == tok::semi){

        lexer.NextToken(cur_token);//next token, skip semi
    }
    else{

        lexer.TryNextToken(next_token);//try get next token
        if (next_token.kind == tok::identifier){//two object must split by ;

            yDiagnosticsEngine::DiagReport(cur_token, diag::yconfigparser_error_expect_semi);
            return false;
        }
    }

    return true;
}




yDiagnosticBuilder yConfigParser::Diag(ySourceLocation loc, unsigned diag_id) {
  return diag_engine.Report(loc, diag_id);
}

yDiagnosticBuilder yConfigParser::Diag(const yToken &cur_token, unsigned diag_id) {
  return Diag(cur_token.getLocation(), diag_id);
}


void yConfigParser::Initialize(void){
    
}
/// Parse the first top-level declaration in a translation unit.
///
///   translation-unit:
/// [C]     external-declaration
/// [C]     translation-unit external-declaration
/// [C++]   top-level-declaration-seq[opt]
/// [C++20] global-module-fragment[opt] module-declaration
///                 top-level-declaration-seq[opt] private-module-fragment[opt]
///
/// Note that in C, it is an error if there is no first declaration.
bool yConfigParser::ParseFirstTopLevelDecl(yDeclGroup &result){

    // C11 6.9p1 says translation units must have at least one top-level
    // declaration. C++ doesn't have this restriction. We also don't want to
    // complain if we have a precompiled header, although technically if the PCH
    // is empty we should still emit the (pedantic) diagnostic.
    // If the main file is a header, we're only pretending it's a TU; don't warn.
    bool _is_no_top_level_decls = ParseTopLevelDecl(result, true);
    if (_is_no_top_level_decls){

        // Diag(diag::)
        
    }
}

/// ParseTopLevelDecl - Parse one top-level declaration, return whatever the
/// action tells us to.  This returns true if the EOF was encountered.
///
///   top-level-declaration:
///           declaration
/// [C++20]   module-import-declaration
bool yConfigParser::ParseTopLevelDecl(yDeclGroup &result, bool is_first_decl){

    result = ParseExternalDeclaration();
}

/// ParseExternalDeclaration:
///
///       external-declaration: [C99 6.9], declaration: [C++ dcl.dcl]
///         function-definition
///         declaration
/// [GNU]   asm-definition
/// [GNU]   __extension__ external-declaration
/// [OBJC]  objc-class-definition
/// [OBJC]  objc-class-declaration
/// [OBJC]  objc-alias-declaration
/// [OBJC]  objc-protocol-definition
/// [OBJC]  objc-method-definition
/// [OBJC]  @end
/// [C++]   linkage-specification
/// [GNU] asm-definition:
///         simple-asm-expr ';'
/// [C++11] empty-declaration
/// [C++11] attribute-declaration
///
/// [C++11] empty-declaration:
///           ';'
///
/// [C++0x/GNU] 'extern' 'template' declaration
///
/// [Modules-TS] module-import-declaration
///
yDeclGroup yConfigParser::ParseExternalDeclaration(void)
{
    return yDeclGroup();
}