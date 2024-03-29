/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: your name
 * @Date: 2021-11-27 14:16:42
 * @LastEditTime: 2021-11-27 14:16:43
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\sema\ysema.hpp
 */

#ifndef __CORE_YCOMPILER_PARSE_YSEMA_HPP__
#define __CORE_YCOMPILER_PARSE_YSEMA_HPP__

#include "core/yobject.hpp"

#include "core/ycompiler/ast/yast_context.hpp"
#include "core/ycompiler/ast/yast_consumer.hpp"
#include "core/ycompiler/ast/yoperation_kinds.hpp"

#include "core/ycompiler/lexer/ypreprocessor.hpp"

#include "core/ycompiler/parser/yraii_objects_for_parser.hpp"

#include "core/ycompiler/ast/ydeclaration_name.hpp"
#include "core/ycompiler/lexer/yliteral_support.hpp"

#include "core/ycompiler/ast/ystmt.hpp"
#include "core/ycompiler/ast/yexpr.hpp"
#include "core/ycompiler/ast/ydecl.hpp"

#include <cstdint>

namespace yLib
{
    namespace ycompiler
    {
        class yConfigParser;
        class yCompilerInstance;

        //semantic analysis
        class __YLIB_CLASS_DECLSPEC__ ySema:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            private:
            std::unique_ptr<yPreprocessor> preprocessor;
            std::unique_ptr<yASTContext> ast_ctx;
            std::unique_ptr<yASTConsumer> ast_consumer;

            yCompilerInstance & ci;
            
            public:
            ySema() = delete;
            ySema(yCompilerInstance & ci);
            ~ySema(){}

            yPreprocessor &GetPreprocessor();
            yASTContext &GetASTContext();
            yASTConsumer &GetASTConsumer();
            void SetASTConsumer(yASTConsumer &ast_consumer){
                this->ast_consumer = std::unique_ptr<yASTConsumer>(new yASTConsumer(ast_consumer));
            }

            //TranslationUnit
            void ActOnStartOfTranslationUnit();
            void ActOnEndOfTranslationUnit();


            /// Called before parsing a function declarator belonging to a function
            /// declaration.
            void ActOnStartFunctionDeclarationDeclarator(yDeclarator &D);

            /// Called after parsing a function declarator belonging to a function
            /// declaration.
            void ActOnFinishFunctionDeclarationDeclarator(yDeclarator &D);

            yDecl *ActOnFinishObjectBody(yDecl *obj_decl, yStmt *obj_body);
            yDecl *ActOnFinishObjectBody(yDecl *obj_decl, yStmt *obj_body, bool IsInstantiation){return nullptr;}

            void ActOnAfterCompoundStatementLeadingPragmas(){}
            yStmt* ActOnCompoundStmt(std::vector<yStmt*> &&stmt_vec);
            yStmt* ActOnAttributedStmt(yStmt *SubStmt){return SubStmt;}

            //We get a decl by yDeclarator
            yDecl *ActOnDeclarator(yDeclarator &D);
            /// Retrieves the declaration name from a parsed unqualified-id.
            yDeclarationNameInfo GetNameFromUnqualifiedId(yUnqualifiedId &Name);

            /// GetNameForDeclarator - Determine the full declaration name for the
            /// given Declarator.
            yDeclarationNameInfo GetNameForDeclarator(yDeclarator &D);
            yNamedDecl *HandleDeclarator(yDeclarator &D);
            yNamedDecl *ActOnVariableDeclarator(yDeclarator &D);
            void AddInitializerToDecl(yDecl *decl, yExpr *init, bool DirectInit);

            void FinalizeDeclaration(yDecl *D);
            yDeclGroup &FinalizeDeclaratorGroup(yDecl *decl);


            yExpr * ActOnNumericConstant(yToken & tok);
            static yExpr *BuildFloatingLiteral(ySema &S, yNumericLiteralParser &Literal, ySourceLocation loc);

            /// ActOnStringLiteral - The specified tokens were lexed as pasted string
            /// fragments (e.g. "foo" "bar" L"baz").
            yExpr *ActOnStringLiteral(std::vector<yToken> &str_tok_vec);

            /// ActOnCXXBoolLiteral - Parse {true,false} literals.
            yExpr * ActOnCXXBoolLiteral(yToken & BoolToks);

            yDecl *ActOnStartOfFunctionDef(yDeclarator &D);

            yExpr * BuildUnaryOp(ySourceLocation OpLoc,
                                    yUnaryOperatorKind Opc, yExpr *Input);
            yExpr * ActOnUnaryOp(ySourceLocation OpLoc,
                                    tok::yTokenKind Op, yExpr *Input);
            yExpr * CreateBuiltinUnaryOp(ySourceLocation OpLoc,
                                                yUnaryOperatorKind Opc,
                                                yExpr *InputExpr);


            yStmt * ActOnDeclStmt(yDecl *decl, ySourceLocation loc){

                return new yDeclStmt(decl, loc);
            }
        };
    }
}


#endif //__CORE_YCOMPILER_PARSE_YSEMA_HPP__