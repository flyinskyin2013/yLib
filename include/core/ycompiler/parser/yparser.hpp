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
 * @Date: 2021-11-20 16:49:17
 * @LastEditTime: 2021-11-21 10:11:57
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\parse\yparser.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#ifndef __CORE_YCOMPILER_PARSE_YPARSER_HPP__
#define __CORE_YCOMPILER_PARSE_YPARSER_HPP__

#include "core/yobject.hpp"

#include "core/ycompiler/lexer/ylexer.hpp"


#include <list>

namespace yLib
{
    namespace ycompiler
    {
        class yDeclGroup;

        class __YLIB_CLASS_DECLSPEC__ yParser:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            protected:
            bool meet_error = false;

            public:
            yParser():meet_error(false){}
            virtual ~yParser(){}
            virtual void Initialize() = 0;
            virtual bool ParseFirstTopLevelDecl(yDeclGroup &result) = 0;
            virtual bool ParseTopLevelDecl(yDeclGroup &result, bool is_first_decl) = 0;

            bool has_error(){return meet_error;}
            void set_paser_error(bool error){meet_error = error;}
        };
    }
}


#endif //__CORE_YCOMPILER_PARSE_YPARSER_HPP__