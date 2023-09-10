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
 * @Date: 2021-11-20 14:35:44
 * @LastEditTime: 2021-11-21 10:09:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\basic\ytoken.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#ifndef __CORE_YCOMPILER_BASIC_YTOKEN_HPP__
#define __CORE_YCOMPILER_BASIC_YTOKEN_HPP__

#include "core/yobject.hpp"
#include "core/ycompiler/basic/ysource_location.hpp"
#include "core/ycompiler/basic/yidentifier_table.hpp"

namespace yLib
{
    namespace ycompiler
    {

 

        class __YLIB_CLASS_DECLSPEC__ yToken:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            public:
            tok::yTokenKind kind;
            uint64_t token_data_len;
            /// PtrData - This is a union of four different pointer types, which depends
            /// on what type of token this is:
            ///  Identifiers, keywords, etc:
            ///    This is an IdentifierInfo*, which contains the uniqued identifier
            ///    spelling.
            ///  Literals:  isLiteral() returns true.
            ///    This is a pointer to the start of the token in a text buffer, which
            ///    may be dirty (have trigraphs / escaped newlines).
            ///  Annotations (resolved type names, C++ scopes, etc): isAnnotation().
            ///    This is a pointer to sema-specific data for the annotation token.
            ///  Eof:
            //     This is a pointer to a Decl.
            ///  Other:
            ///    This is null.
            void *token_data;

            uint64_t loc; 
            yFileID file_id;
            uint64_t offset;
            
            yToken(){

                kind  = tok::unknown;
                token_data_len = 0;
                token_data = nullptr;
                
            }

            void clean(){

                kind  = tok::unknown;
                token_data_len = 0;
                token_data = nullptr;                
            }

            /// Return a source location identifier for the specified
            /// offset in the current file.
            ySourceLocation getLocation() const {
                return ySourceLocation(file_id, offset);
            }

            yIdentifierInfo *GetIdentifierInfo(){return (yIdentifierInfo *)token_data;}

            /// Return true if this is a C or C++ string-literal (or
            /// C++11 user-defined-string-literal) token.
            bool is_string_literal(){

                return  kind == tok::string_literal || \
                        kind == tok::wide_string_literal || kind == tok::utf8_string_literal || \
                        kind == tok::utf16_string_literal || kind == tok::utf32_string_literal;
            }
            
            /// Return true if this is a "literal" kind, like a numeric
            /// constant, string, etc.
            bool is_literal(){
                return  kind == tok::numeric_constant || kind == tok::char_constant ||
                        kind == tok::wide_char_constant || kind == tok::utf8_char_constant ||
                        kind == tok::utf16_char_constant || kind == tok::utf32_char_constant ||
                        is_string_literal() || kind == tok::header_name;                
            }
        };
    } // namespace ycompiler
} // namespace yLib


#endif //__CORE_YCOMPILER_BASIC_YTOKEN_HPP__