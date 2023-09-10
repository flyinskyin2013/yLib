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
 * @Date: 2023-07-16 17:37:58
 * @LastEditTime: 2023-07-16 17:37:58
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\parse\yconfig_parser_expr.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/frontend/ycompiler_instance.hpp"




using namespace yLib::ycompiler;
using namespace yLib;

/// Parse a binary expression that starts with \p LHS and has a
/// precedence of at least \p MinPrec.
yExpr* yConfigParser::ParseRHSOfBinaryExpression(yExpr* lhs)
{
    if (tok::semi == cur_token.kind){//we meet ';'

        return lhs;
    }

    //Other expr
    return nullptr;
}

yExpr* yConfigParser::ParseCastExpression()
{   
    yExpr* result = nullptr;

    switch (cur_token.kind)
    {
    case tok::numeric_constant:{

        result = ci.GetSema().ActOnNumericConstant(cur_token);
        ConsumeToken();//eat 'numeric_constant' token
        break;
    }
    case tok::string_literal:{

        //In ParseStringLiteralExpression, we eat many 'string_literal' token
        result = ParseStringLiteralExpression();

        break;
    }
    case tok::kw_true:
    case tok::kw_false:
    {

        result = ParseCXXBoolLiteral();
        ConsumeToken();//eat 'kw_true or kw_false' token
        break;
    }

    case tok::minus:// unary-expression: '-' cast-expression
    case tok::plus:// unary-expression: '+' cast-expression
    {
        //for unary-expression
        tok::yTokenKind _saved_kind = cur_token.kind;
        ySourceLocation _saved_loc = ConsumeToken();

        result = ParseCastExpression();

        //we combine UnaryOp and result
        result = sema.ActOnUnaryOp(_saved_loc, _saved_kind, result);
        break;
    }
    default:

        //unkown expr
        break;
    }


    return result;
}

// Expr that doesn't include commas.
yExpr* yConfigParser::ParseAssignmentExpression()
{
    yExpr* _expr = ParseCastExpression();

    //lhs refers to “left hand side”
    //rhs refers to the “right hand side”

    return ParseRHSOfBinaryExpression(_expr);
}


yExpr* yConfigParser::ParseStringLiteralExpression()
{

    //support '"std::string var = string1" "string 2" "string 3" ...;'
    std::vector<yToken> _str_tok_vec;

    while(cur_token.is_string_literal()){

        _str_tok_vec.push_back(cur_token);
        ConsumeToken();
    }

    return sema.ActOnStringLiteral(_str_tok_vec);
}

yExpr* yConfigParser::ParseCXXBoolLiteral()
{
    return sema.ActOnCXXBoolLiteral(cur_token);
}