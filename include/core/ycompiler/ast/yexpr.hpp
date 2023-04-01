/*
Copyright (c) 2018 - 2023 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: Sky
 * @Date: 2023-03-19 13:50:41
 * @LastEditTime: 2023-03-19 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\yexpr.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YEXPR_HPP__
#define __CORE_YCOMPILER_AST_YEXPR_HPP__

#include "core/ycompiler/ast/ystmt.hpp"

namespace yLib
{
    namespace ycompiler
    {
        /// This represents one expression.  Note that Expr's are subclasses of Stmt.
        /// This allows an expression to be transparently used any place a Stmt is
        /// required.
        class yExpr : public yValueStmt {};



        class yIntegerLiteral : public yExpr{};

        class yFloatingLiteral : public yExpr{};

        /// StringLiteral - This represents a string literal expression, e.g. "foo"
        /// or L"bar" (wide strings). The actual string data can be obtained with
        /// getBytes() and is NOT null-terminated. The length of the string data is
        /// determined by calling getByteLength().
        ///
        /// The C type for a string is always a ConstantArrayType. In C++, the char
        /// type is const qualified, in C it is not.
        ///
        /// Note that strings in C can be formed by concatenation of multiple string
        /// literal pptokens in translation phase #6. This keeps track of the locations
        /// of each of these pieces.
        ///
        /// Strings in C can also be truncated and extended by assigning into arrays,
        /// e.g. with constructs like:
        ///   char X[2] = "foobar";
        /// In this case, getByteLength() will return 6, but the string literal will
        /// have type "char[2]".
        class yStringLiteral final
            : public yExpr{};

        /// A boolean literal, per ([C++ lex.bool] Boolean literals).
        class yBoolLiteralExpr : public yExpr {};
    } // namespace ycompiler
    
} // namespace yLib



#endif //__CORE_YCOMPILER_AST_YEXPR_HPP__