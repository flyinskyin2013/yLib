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
 * @Date: 2022-12-04 13:50:41
 * @LastEditTime: 2022-12-04 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\ydecl_base.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YDECL_BASE_HPP__
#define __CORE_YCOMPILER_AST_YDECL_BASE_HPP__


#include <memory>
#include "core/yobject.hpp"
#include "core/ycompiler/basic/ysource_location.hpp"
#include "core/ycompiler/ast/ydecl_group.hpp"

namespace yLib
{
    namespace ycompiler
    {
        /// Decl - This represents one declaration (or definition), e.g. a variable,
        /// typedef, function, struct, etc.
        ///
        /// Note: There are objects tacked on before the *beginning* of Decl
        /// (and its subclasses) in its Decl::operator new(). Proper alignment
        /// of all subclasses (not requiring more than the alignment of Decl) is
        /// asserted in DeclBase.cpp.
        class yDecl: public Object{
            public:
                /// Lists the kind of concrete classes of Decl.
                enum Kind {
                    #define DECL(DERIVED, BASE) DERIVED##Decl,
                    #define ABSTRACT_DECL(DECL)
                    #define DECL_RANGE(BASE, START, END) \
                            first##BASE = START, last##BASE = END,
                    #define LAST_DECL_RANGE(BASE, START, END) \
                            first##BASE = START, last##BASE = END
                    #include "decl_nodes_kinds.def"
                };

            protected:
            /// Loc - The location of this decl.
            ySourceLocation loc;
            yDecl::Kind kind;

            public:
            virtual ~yDecl(){}
            
            void set_decl_kind(yDecl::Kind kind){this->kind = kind;}
            yDecl::Kind get_decl_kind(void) {return kind;}

            void set_decl_loc(ySourceLocation loc){this->loc = loc;}
            ySourceLocation get_decl_loc(void){return loc;}
            
        YLIB_DEFINE_CLASS_TYPE_KEY(yLib::ycompiler::yDecl);
        };
        using Decl = yDecl;



        /// DeclContext - This is used only as base class of specific decl types that
        /// can act as declaration contexts. These decls are (only the top classes
        /// that directly derive from DeclContext are mentioned, not their subclasses):
        ///
        ///   TranslationUnitDecl
        ///   ExternCContext
        ///   NamespaceDecl
        ///   TagDecl
        ///   OMPDeclareReductionDecl
        ///   OMPDeclareMapperDecl
        ///   FunctionDecl
        ///   ObjCMethodDecl
        ///   ObjCContainerDecl
        ///   LinkageSpecDecl
        ///   ExportDecl
        ///   BlockDecl
        ///   CapturedDecl
        class yDeclContext: public Object{
            DeclGroup decl_group;
            public:
            virtual ~yDeclContext(){}
            DeclGroup& get_decl_group(){return decl_group;}

        YLIB_DEFINE_CLASS_TYPE_KEY(yLib::ycompiler::yDeclContext);
        };
        using DeclContext = yDeclContext;

    } // namespace ycompiler
} // namespace yLib

#endif //__CORE_YCOMPILER_AST_YDECL_BASE_HPP__