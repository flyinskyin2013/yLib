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


#include "core/ycompiler/basic/yspecifiers.hpp"
#include "core/ycompiler/basic/yidentifier_table.hpp"
#include "core/ycompiler/basic/ysource_location.hpp"

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


        /// Describes the kind of unqualified-id parsed.
        enum class yUnqualifiedIdKind {
        /// An identifier.
        IK_Identifier,
        /// An overloaded operator name, e.g., operator+.
        IK_OperatorFunctionId,
        /// A conversion function name, e.g., operator int.
        IK_ConversionFunctionId,
        /// A user-defined literal name, e.g., operator "" _i.
        IK_LiteralOperatorId,
        /// A constructor name.
        IK_ConstructorName,
        /// A constructor named via a template-id.
        IK_ConstructorTemplateId,
        /// A destructor name.
        IK_DestructorName,
        /// A template-id, e.g., f<int>.
        IK_TemplateId,
        /// An implicit 'self' parameter
        IK_ImplicitSelfParam,
        /// A deduction-guide name (a template-name)
        IK_DeductionGuideName
        };

        /// Represents a C++ unqualified-id that has been parsed.
        class yUnqualifiedId {
        private:
            /// When Kind == IK_Identifier, the parsed identifier, or when
            /// Kind == IK_UserLiteralId, the identifier suffix.
            yIdentifierInfo *Identifier;

            /// The location of the first token that describes this unqualified-id,
            /// which will be the location of the identifier, "operator" keyword,
            /// tilde (for a destructor), or the template name of a template-id.
            ySourceLocation StartLocation;

            /// The location of the last token that describes this unqualified-id.
            ySourceLocation EndLocation;

        public:
            /// Describes the kind of unqualified-id parsed.
            yUnqualifiedIdKind Kind;

            void SetIdentifier(yIdentifierInfo *Id, ySourceLocation IdLoc) {
                Kind = yUnqualifiedIdKind::IK_Identifier;
                Identifier = Id;
                StartLocation = EndLocation = IdLoc;
            }
            yIdentifierInfo * GetIdentifierInfo(){return Identifier;}
            ySourceLocation GetStartLocation(){return StartLocation;}
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
            private:
            yUnqualifiedId Name;
            ySourceRange Range;

            bool is_obj_def = false;
            bool valid = true;
            public:
            /// Where we are parsing this declarator.
            yDeclaratorContext declearator_ctx;

            yDeclarator(yDeclaratorContext declearator_ctx){this->declearator_ctx = declearator_ctx;}

            /// Set the name of this declarator to be the given identifier.
            void SetIdentifier(yIdentifierInfo *Id, ySourceLocation IdLoc) {
                Name.SetIdentifier(Id, IdLoc);
            }
            yUnqualifiedId & GetIdentifier(void) {
                return Name;
            }

            void SetRange(ySourceLocation b, ySourceLocation e){

                Range.SetBegin(b);
                Range.SetEnd(e);
            }

            void SetAsObjectDefinitionDeclarator(){is_obj_def = true;}
            bool IsObjectDefinition(){return is_obj_def;}

            void set_is_valid(bool is_valid){this->valid = is_valid;}
            bool is_valid(){return valid;}
        };


        /// Captures information about "declaration specifiers".
        ///
        /// "Declaration specifiers" encompasses storage-class-specifiers,
        /// type-specifiers, type-qualifiers, and function-specifiers.
        class DeclSpec {
        public:
            /// storage-class-specifier
            /// \note The order of these enumerators is important for diagnostics.
            enum StorageClassSpecifier {
                SCS_unspecified = 0,
                SCS_typedef,
                SCS_extern,
                SCS_static,
                SCS_auto,
                SCS_register,
                SCS_private_extern,
                SCS_mutable
            };

            /// ParsedSpecifiers - Flags to query which specifiers were applied.  This is
            /// returned by getParsedSpecifiers.
            enum ParsedSpecifiers {
                PQ_None                  = 0,
                PQ_StorageClassSpecifier = 1,
                PQ_TypeSpecifier         = 2,
                PQ_TypeQualifier         = 4,
                PQ_FunctionSpecifier     = 8
                // FIXME: Attributes should be included here.
            };

            // storage-class-specifier
            /*SCS*/unsigned StorageClassSpec : 3;
            /*TSCS*/unsigned ThreadStorageClassSpec : 2;
            unsigned SCS_extern_in_linkage_spec : 1;

            // type-specifier
            /*TypeSpecifierWidth*/ unsigned TypeSpecWidth : 2;
            /*TSC*/unsigned TypeSpecComplex : 2;
            /*TSS*/unsigned TypeSpecSign : 2;
            /*TST*/unsigned TypeSpecType : 6;
            unsigned TypeAltiVecVector : 1;
            unsigned TypeAltiVecPixel : 1;
            unsigned TypeAltiVecBool : 1;
            unsigned TypeSpecOwned : 1;
            unsigned TypeSpecPipe : 1;
            unsigned TypeSpecSat : 1;
            unsigned ConstrainedAuto : 1;

            // type-qualifiers
            unsigned TypeQualifiers : 5;  // Bitwise OR of TQ.

            // function-specifier
            unsigned FS_inline_specified : 1;
            unsigned FS_forceinline_specified: 1;
            unsigned FS_virtual_specified : 1;
            unsigned FS_noreturn_specified : 1;

            // friend-specifier
            unsigned Friend_specified : 1;

            // constexpr-specifier
            unsigned ConstexprSpecifier : 2;
        };
    } // namespace ycompiler
} // namespace yLib







#endif //__CORE_YCOMPILER_AST_YDECL_SPEC_HPP__