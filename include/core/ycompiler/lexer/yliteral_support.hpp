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
 * @Date: 2023-08-26 14:29:50
 * @LastEditTime: 2023-08-26 17:51:43
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\lex\yliteral_support.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_LEX_YLITERAL_SUPPORT_HPP__
#define __CORE_YCOMPILER_LEX_YLITERAL_SUPPORT_HPP__

#include <cstdint>
#include <string>

#include "core/yobject.hpp"
#include "core/ycompiler/lexer/ytoken.hpp"

namespace yLib
{
    namespace ycompiler
    {
        /// NumericLiteralParser - This performs strict semantic analysis of the content
        /// of a ppnumber, classifying it as either integer, floating, or erroneous,
        /// determines the radix of the value and can convert it to a useful value.
        class yNumericLiteralParser {
            std::string literal_str;

            bool is_integer_literal = false;
            bool is_float_literal = false;
            public:
            yNumericLiteralParser(yToken & tok);
            /// GetIntegerValue - Convert this numeric literal value to an APInt that
            /// matches Val's input width.  If there is an overflow (i.e., if the unsigned
            /// value read is larger than the APInt's bits will hold), set Val to the low
            /// bits of the result and return true.  Otherwise, return false.
            bool GetIntegerValue(int64_t &Val);

            /// GetFloatValue - Convert this numeric literal to a floating value, using
            /// the specified APFloat fltSemantics (specifying float, double, etc).
            /// The optional bool isExact (passed-by-reference) has its value
            /// set to true if the returned APFloat can represent the number in the
            /// literal exactly, and false otherwise.
            bool GetFloatValue(double &Result);

            bool isIntegerLiteral() const {
                return is_integer_literal;
            }
            bool isFloatingLiteral() const {
                return is_float_literal;
            }
        };

    }
}


#endif //__CORE_YCOMPILER_LEX_YLITERAL_SUPPORT_HPP__