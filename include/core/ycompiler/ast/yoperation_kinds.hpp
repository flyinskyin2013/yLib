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
 * @Date: 2023-08-27 14:27:50
 * @LastEditTime: 2023-08-27 18:59:57
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\yoperation_kinds.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YOPERATION_KINDS_HPP__
#define __CORE_YCOMPILER_AST_YOPERATION_KINDS_HPP__

#include <cstdint>

#include "core/yobject.hpp"
#include "core/ycompiler/lexer/ytoken.hpp"

namespace yLib
{
    namespace ycompiler
    {
        /// CastKind - The kind of operation required for a conversion.
        enum yCastKind {
        #define CAST_OPERATION(Name) CK_##Name,


        #include "operation_kinds.def"
        };

        enum yBinaryOperatorKind {

        #define BINARY_OPERATION(Name, Spelling) BO_##Name,

        #include "operation_kinds.def"
        };

        enum yUnaryOperatorKind {

        #define UNARY_OPERATION(Name, Spelling) UO_##Name,
        #include "operation_kinds.def"
        };
        
    }
}

#endif //__CORE_YCOMPILER_AST_YOPERATION_KINDS_HPP__