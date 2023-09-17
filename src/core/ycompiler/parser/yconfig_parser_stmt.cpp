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
 * @FilePath: \yLib\src\core\ycompiler\parse\yconfig_parser_stmt.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/parser/yconfig_parser.hpp"

using namespace yLib::ycompiler;
using namespace yLib;



yStmt* yConfigParser::ParseStatementOrDeclarationAfterAttributes(std::vector<yStmt*> &Stmts)
{
    //parse first declarator
    yParsingDeclarator _declarator(*this, yDeclaratorContext::File);
    ParseDeclarator(_declarator);

    if (!_declarator.is_valid()){

        return nullptr;
    }

    yDecl * _decl = nullptr;
    if (_declarator.IsObjectDefinition()){//parse object defination

        _decl = ParseObjectDefinition(_declarator);
    }
    else{//parse declaration

        _decl = ParseDeclarationAfterDeclaratorAndAttributes(_declarator);
    }

    return sema.ActOnDeclStmt(_decl, _declarator.GetIdentifier().GetStartLocation());
}


// In yConfigLanguage.md 2.6.1
// compound-statement:
//      { block-item-list[opt] }
// block-item-list:
//      block-item
//      block-item-list block-item
// block-item:
//      declaration
//      statement
yStmt* yConfigParser::ParseStatementOrDeclaration(std::vector<yStmt*> &Stmts)
{

    yStmt* _stmt = ParseStatementOrDeclarationAfterAttributes(Stmts);
    
    return sema.ActOnAttributedStmt(_stmt);
}


/// ParseCompoundStatementBody - Parse a sequence of statements and invoke the
/// ActOnCompoundStmt action.  This expects the '{' to be the current token, and
/// consume the '}' at the end of the block.  It does not manipulate the scope
/// stack.
yStmt* yConfigParser::ParseCompoundStatementBody(bool isStmtExpr) 
{
    // Parse any pragmas at the beginning of the compound statement.
    //Now, Do nothing
    ParseCompoundStatementLeadingPragmas();
    sema.ActOnAfterCompoundStatementLeadingPragmas();

    //DeclStmt
    //ObjectDecl
    std::vector<yStmt*> _stmt_vec;
    while(cur_token.kind != tok::r_brace){//now, we parse statement

        if (tok::comment == cur_token.kind){//In body, we eat comment

            //
            ConsumeToken();
        }

        yStmt* _stmt = ParseStatementOrDeclaration(_stmt_vec);
        if (nullptr == _stmt){

            return nullptr;
        }

        _stmt_vec.push_back(_stmt);
    }
    
    //eat '}'
    ConsumeToken();

    return sema.ActOnCompoundStmt(std::move(_stmt_vec));
}

yDecl *yConfigParser::ParseFunctionStatementBody(yDecl *Decl, yParseScope &BodyScope)
{
    ConsumeToken();//eat '{'

    // Do not enter a scope for the brace, as the arguments are in the same scope
    // (the function body) as the body itself.  Instead, just read the statement
    // list and put it into a CompoundStmt for safe keeping.
    yStmt * _stmt = ParseCompoundStatementBody(false);//eat '}'

    if (nullptr == _stmt){

        return nullptr;
    }

    ConsumeToken();//eat ';'

    return sema.ActOnFinishObjectBody(Decl, _stmt);
}