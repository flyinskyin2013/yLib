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
 * @Date: 2023-05-50 13:50:41
 * @LastEditTime: 2023-05-20 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\ydecl_spec.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YDECL_SPEC_HPP__
#define __CORE_YCOMPILER_AST_YDECL_SPEC_HPP__

#include <string>

namespace yLib
{
    namespace ycompiler
    {
        enum class yDeclaratorContext {
            File,                // File scope declaration.
            Prototype,           // Within a function prototype.
            ObjCResult,          // An ObjC method result type.
            ObjCParameter,       // An ObjC method parameter type.
            KNRTypeList,         // K&R type definition list for formals.
            TypeName,            // Abstract declarator for types.
            FunctionalCast,      // Type in a C++ functional cast expression.
            Member,              // Struct/Union field.
            Block,               // Declaration within a block in a function.
            ForInit,             // Declaration within first part of a for loop.
            SelectionInit,       // Declaration within optional init stmt of if/switch.
            Condition,           // Condition declaration in a C++ if/switch/while/for.
            TemplateParam,       // Within a template parameter list.
            CXXNew,              // C++ new-expression.
            CXXCatch,            // C++ catch exception-declaration
            ObjCCatch,           // Objective-C catch exception-declaration
            BlockLiteral,        // Block literal declarator.
            LambdaExpr,          // Lambda-expression declarator.
            LambdaExprParameter, // Lambda-expression parameter declarator.
            ConversionId,        // C++ conversion-type-id.
            TrailingReturn,      // C++11 trailing-type-specifier.
            TrailingReturnVar,   // C++11 trailing-type-specifier for variable.
            TemplateArg,         // Any template argument (in template argument list).
            TemplateTypeArg,     // Template type argument (in default argument).
            AliasDecl,           // C++11 alias-declaration.
            AliasTemplate,       // C++11 alias-declaration template.
            RequiresExpr         // C++2a requires-expression.
        };

        /// Information about one declarator, including the parsed type
        /// information and the identifier.
        ///
        /// When the declarator is fully formed, this is turned into the appropriate
        /// Decl object.
        ///
        /// Declarators come in two types: normal declarators and abstract declarators.
        /// Abstract declarators are used when parsing types, and don't have an
        /// identifier.  Normal declarators do have ID's.
        ///
        /// Instances of this class should be a transient object that lives on the
        /// stack, not objects that are allocated in large quantities on the heap.
        class yDeclarator {
            public:
            /// Where we are parsing this declarator.
            yDeclaratorContext declearator_ctx;

            yDeclarator(yDeclaratorContext declearator_ctx){this->declearator_ctx = declearator_ctx;}
        };
    } // namespace ycompiler
} // namespace yLib







#endif //__CORE_YCOMPILER_AST_YDECL_SPEC_HPP__