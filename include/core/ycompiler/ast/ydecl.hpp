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

namespace yLib
{
    namespace ycompiler
    {
        class yASTContext;

        /// The top declaration context.
        class yTranslationUnitDecl : public yDecl, public yDeclContext
        {
            private:

                explicit yTranslationUnitDecl(yASTContext &ctx);
                yASTContext &ctx;


            public:
                static yTranslationUnitDecl *Create(yASTContext &ctx);  
                yASTContext &getASTContext() const { return ctx; }
        };


        /// This represents a decl that may have a name.  Many decls have names such
        /// as ObjCMethodDecl, but not \@class, etc.
        ///
        /// Note that not every NamedDecl is actually named (e.g., a struct might
        /// be anonymous), and not every name is an identifier.
        class yNamedDecl : public yDecl {
            /// The name of this declaration, which is typically a normal
            /// identifier but may also be a special kind of name (C++
            /// constructor, Objective-C selector, etc.)
            std::string Name;

            public:
            std::string & getName() {return Name;}
        };


        /// Represent the declaration of a variable (in which case it is
        /// an lvalue) a function (in which case it is a function designator) or
        /// an enum constant.
        class yValueDecl : public yNamedDecl {};

        /// Represents a ValueDecl that came out of a declarator.
        /// Contains type source information through TypeSourceInfo.
        class yDeclaratorDecl : public yValueDecl {};

        /// Represents a variable declaration or definition.
        class yVarDecl : public yDeclaratorDecl{};

        /// Represents a declaration of a type.
        class yTypeDecl : public yNamedDecl {};

        /// Represents the declaration of a struct/union/class/enum.
        class yTagDecl : public yTypeDecl,
                        public yDeclContext
        {

        };

        /// Represents a struct/union/class.  For example:
        ///   struct X;                  // Forward declaration, no "body".
        ///   union Y { int A, B; };     // Has body with members A and B (FieldDecls).
        /// This decl will be marked invalid if *any* members are invalid.
        class yRecordDecl : public yTagDecl {

        };

    } // namespace ycompiler
} // namespace yLib







#endif //__CORE_YCOMPILER_AST_YDECL_HPP__