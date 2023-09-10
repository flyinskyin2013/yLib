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
 * @Date: 2023-07-09 13:50:41
 * @LastEditTime: 2023-07-09 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\ydeclaration_name.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YDECLARATION_NAME_HPP__
#define __CORE_YCOMPILER_AST_YDECLARATION_NAME_HPP__


#include <memory>
#include <cstdint>


#include "core/ycompiler/basic/ysource_location.hpp"

namespace yLib
{
    namespace ycompiler
    {
        /// The name of a declaration. In the common case, this just stores
        /// an IdentifierInfo pointer to a normal name. However, it also provides
        /// encodings for Objective-C selectors (optimizing zero- and one-argument
        /// selectors, which make up 78% percent of all selectors in Cocoa.h),
        /// special C++ names for constructors, destructors, and conversion functions,
        /// and C++ overloaded operators.
        class yDeclarationName {
            /// StoredNameKind represent the kind of name that is actually stored in the
            /// upper bits of the Ptr field. This is only used internally.
            ///
            /// NameKind, StoredNameKind, and DeclarationNameExtra::ExtraKind
            /// must satisfy the following properties. These properties enable
            /// efficient conversion between the various kinds.
            ///
            /// * The first seven enumerators of StoredNameKind must have the same
            ///   numerical value as the first seven enumerators of NameKind.
            ///   This enable efficient conversion between the two enumerations
            ///   in the usual case.
            ///
            /// * The enumerations values of DeclarationNameExtra::ExtraKind must start
            ///   at zero, and correspond to the numerical value of the first non-inline
            ///   enumeration values of NameKind minus an offset. This makes conversion
            ///   between DeclarationNameExtra::ExtraKind and NameKind possible with
            ///   a single addition/substraction.
            ///
            /// * The enumeration values of Selector::IdentifierInfoFlag must correspond
            ///   to the relevant enumeration values of StoredNameKind.
            ///   More specifically:
            ///    * ZeroArg == StoredObjCZeroArgSelector,
            ///    * OneArg == StoredObjCOneArgSelector,
            ///    * MultiArg == StoredDeclarationNameExtra
            ///
            /// * PtrMask must mask the low 3 bits of Ptr.
            enum StoredNameKind {
                StoredIdentifier = 0,
                StoredObjCZeroArgSelector = 1,
                StoredObjCOneArgSelector = 2,
                StoredCXXConstructorName = 3,
                StoredCXXDestructorName = 4,
                StoredCXXConversionFunctionName = 5,
                StoredCXXOperatorName = 6,
                StoredDeclarationNameExtra = 7,
                PtrMask = 7,
                UncommonNameKindOffset = 8
            };

            StoredNameKind kind = StoredNameKind::StoredIdentifier;
            
            /// The lowest three bits of Ptr are used to express what kind of name
            /// we're actually storing, using the values of StoredNameKind. Depending
            /// on the kind of name this is, the upper bits of Ptr may have one
            /// of several different meanings:
            ///
            ///   StoredIdentifier - The name is a normal identifier, and Ptr is
            ///   a normal IdentifierInfo pointer.
            uintptr_t ptr = 0;


            public:
            yDeclarationName(){}
            yDeclarationName(yIdentifierInfo * identifierinfo):ptr((uintptr_t)identifierinfo){}
            yIdentifierInfo * GetAsIdentifierInfo(){ return (yIdentifierInfo *)ptr;}
        };



        /// DeclarationNameInfo - A collector data type for bundling together
        /// a DeclarationName and the correspnding source/type location info.
        struct yDeclarationNameInfo {
        private:
            /// Name - The declaration name, also encoding name kind.
            yDeclarationName Name;

            /// Loc - The main source location for the declaration name.
            ySourceLocation NameLoc;
        public:
        /// getName - Returns the embedded declaration name.
        yDeclarationName get_name() const { return Name; }
        /// setName - Sets the embedded declaration name.
        void set_name(yDeclarationName N) { Name = N; }

        ySourceLocation get_nameloc() const {return NameLoc;}
        void set_nameloc(ySourceLocation loc) {NameLoc = loc;}
        };
    }
}

#endif //__CORE_YCOMPILER_AST_YDECLARATION_NAME_HPP__