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
 * @Date: 2023-07-09 13:50:41
 * @LastEditTime: 2023-07-09 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\yidentifier_table.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YIDENTIFIER_TABLE_HPP__
#define __CORE_YCOMPILER_AST_YIDENTIFIER_TABLE_HPP__


#include <memory>
#include <cstdint>

#include "core/ycompiler/basic/ytoken_kind.hpp"

namespace yLib
{
    namespace ycompiler
    {

        /// One of these records is kept for each identifier that
        /// is lexed.  This contains information about whether the token was \#define'd,
        /// is a language keyword, or if it is a front-end token of some sort (e.g. a
        /// variable or function name).  The preprocessor keeps this information in a
        /// set, and all tok::identifier tokens have a pointer to one of these.
        /// It is aligned to 8 bytes because DeclarationName needs the lower 3 bits.
        class yIdentifierInfo {
            std::string identifier;
            tok::yTokenKind token_kind;
            public:
            yIdentifierInfo(const std::string & name, tok::yTokenKind tok_kind):identifier(name), token_kind(tok_kind){}
            std::string get_identifier_name(){return identifier;}

        };

        /// Implements an efficient mapping from strings to IdentifierInfo nodes.
        ///
        /// This has no other purpose, but this is an extremely performance-critical
        /// piece of the code, as each occurrence of every identifier goes through
        /// here when lexed.
        class yIdentifierTable {
            
        };
    }
}

#endif //__CORE_YCOMPILER_AST_YIDENTIFIER_TABLE_HPP__