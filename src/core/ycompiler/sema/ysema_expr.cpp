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
 * @FilePath: \yLib\include\core\ycompiler\sema\ysema_expr.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/sema/ysema.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

static inline yUnaryOperatorKind ConvertTokenKindToUnaryOpcode(tok::yTokenKind Kind) {

  yUnaryOperatorKind Opc = UO_INVALID;
  switch (Kind) {

    case tok::plus:         Opc = UO_Plus; break;
    case tok::minus:        Opc = UO_Minus; break;
    default: //Unknown unary op!
        break;
  }
  return Opc;
}

yExpr * ySema::CreateBuiltinUnaryOp(ySourceLocation OpLoc,
                                    yUnaryOperatorKind Opc,
                                    yExpr *InputExpr)
{
    return yUnaryOperator::Create(OpLoc, Opc, InputExpr);
}

yExpr * ySema::BuildUnaryOp(ySourceLocation OpLoc,
                        yUnaryOperatorKind Opc, yExpr *Input)
{
    return CreateBuiltinUnaryOp(OpLoc, Opc, Input);
}

yExpr * ySema::ActOnUnaryOp(ySourceLocation OpLoc,
                        tok::yTokenKind Op, yExpr *Input)
{
    return BuildUnaryOp(OpLoc, ConvertTokenKindToUnaryOpcode(Op), Input);
}

