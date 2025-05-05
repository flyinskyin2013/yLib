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
 * @FilePath: \yLib\include\core\ycompiler\ast\ydecl.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YDECL_HPP__
#define __CORE_YCOMPILER_AST_YDECL_HPP__

#include <string>


#include "core/ycompiler/ast/ydecl_base.hpp"
#include "core/ycompiler/ast/ydecl_group.hpp"
#include "core/ycompiler/ast/yexpr.hpp"
#include "core/ycompiler/ast/ydecl_spec.hpp"
namespace yLib
{
    namespace ycompiler
    {
        class yStmt;
        class yASTContext;

        /// The top declaration context.
        class __YLIB_CLASS_DECLSPEC__ TranslationUnitDecl : public yDecl, public yDeclContext
        {
            private:

                explicit TranslationUnitDecl(yASTContext &ctx);
                yASTContext &ctx;


            public:
                static TranslationUnitDecl *Create(yASTContext &ctx);  
                yASTContext &getASTContext() const { return ctx; }

        YLIB_DEFINE_CLASS_TYPE_KEY(yLib::ycompiler::TranslationUnitDecl);
        };


        /// This represents a decl that may have a name.  Many decls have names such
        /// as ObjCMethodDecl, but not \@class, etc.
        ///
        /// Note that not every NamedDecl is actually named (e.g., a struct might
        /// be anonymous), and not every name is an identifier.
        class yNamedDecl : public yDecl {
            protected:
            /// The name of this declaration, which is typically a normal
            /// identifier but may also be a special kind of name (C++
            /// constructor, Objective-C selector, etc.)
            std::string decl_name;

            public:
            std::string & getName() {return decl_name;}
        };
        using NamedDecl = yNamedDecl;


        /// Represent the declaration of a variable (in which case it is
        /// an lvalue) a function (in which case it is a function designator) or
        /// an enum constant.
        class yValueDecl : public yNamedDecl {

        };
        using ValueDecl = yValueDecl;

        /// Represents a ValueDecl that came out of a declarator.
        /// Contains type source information through TypeSourceInfo.
        class yDeclaratorDecl : public yValueDecl {


        };
        using DeclaratorDecl = yDeclaratorDecl;

        /// Represents a variable declaration or definition.
        class yVarDecl : public yDeclaratorDecl{
            yDeclarator D;
            ySourceLocation loc;
            typedef union __init_pointer__
            {
                /* data */
                yStmt * stmt;
            } InitType;
            
            InitType init;

            yVarDecl(yDeclarator &D, ySourceLocation &loc):D(D), loc(loc){
                decl_name = D.GetIdentifier().GetIdentifierInfo()->get_identifier_name();
                set_decl_kind(yDecl::Kind::VarDecl);
            }
            public:
            static yVarDecl * Create(yDeclarator &D, ySourceLocation &loc){
                return new yVarDecl(D, loc);
            }
            void set_init(yExpr * expr){init.stmt = expr;}
            InitType get_init(){return init;}
        };
        using VarDecl = yVarDecl;

        /// Represents a declaration of a type.
        class yTypeDecl : public yNamedDecl {};
        using TypeDecl = yTypeDecl;

        /// Represents the declaration of a struct/union/class/enum.
        class yTagDecl : public yTypeDecl,
                        public yDeclContext
        {

        };
        using TagDecl = yTagDecl;

        /// Represents a struct/union/class.  For example:
        ///   struct X;                  // Forward declaration, no "body".
        ///   union Y { int A, B; };     // Has body with members A and B (FieldDecls).
        /// This decl will be marked invalid if *any* members are invalid.
        class yRecordDecl : public yTagDecl {

        };
        using RecordDecl = yRecordDecl;


        /// Represents a function declaration or definition.
        ///
        /// Since a given function can be declared several times in a program,
        /// there may be several FunctionDecls that correspond to that
        /// function. Only one of those FunctionDecls will be found when
        /// traversing the list of declarations in the context of the
        /// FunctionDecl (e.g., the translation unit); this FunctionDecl
        /// contains all of the information known about the function. Other,
        /// previous declarations of the function are available via the
        /// getPreviousDecl() chain.
        class yObjectDecl : public yDeclaratorDecl,
                            public yDeclContext{
            std::unique_ptr<yStmt> body;
            yUnqualifiedId * UnqualifiedId;
            
            yObjectDecl(yUnqualifiedId * Name):UnqualifiedId(Name){
                this->decl_name = Name->GetIdentifierInfo()->get_identifier_name();
                set_decl_kind(yDecl::ObjectDecl);
            }
            public:
            void set_body(yStmt * obj_body){
                body = std::unique_ptr<yStmt>(obj_body);
            } 

            yStmt * get_body(void){return body.get();}

            static yObjectDecl* Create(yUnqualifiedId * Name){

                return new yObjectDecl(Name);
            }                           
        };
        using ObjectDecl = yObjectDecl;

        class FunctionDecl : public DeclaratorDecl,
                                public DeclContext
        {
            
        };

    } // namespace ycompiler
} // namespace yLib







#endif //__CORE_YCOMPILER_AST_YDECL_HPP__