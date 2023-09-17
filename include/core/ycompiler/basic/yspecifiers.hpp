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
 * @FilePath: \yLib\include\core\ycompiler\basic\yspecifiers.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_BASIC_YSPECIFIERS_HPP__
#define __CORE_YCOMPILER_BASIC_YSPECIFIERS_HPP__


#include <memory>
#include <cstdint>

namespace yLib
{
    namespace ycompiler
    {
        /// Specifies the kind of type.
        enum TypeSpecifierType {
            TST_unspecified,
            TST_void,
            TST_char,
            TST_wchar,        // C++ wchar_t
            TST_char8,        // C++20 char8_t (proposed)
            TST_char16,       // C++11 char16_t
            TST_char32,       // C++11 char32_t
            TST_int,
            TST_int128,
            TST_bitint,       // Bit-precise integer types.
            TST_half,         // OpenCL half, ARM NEON __fp16
            TST_Float16,      // C11 extension ISO/IEC TS 18661-3
            TST_Accum,        // ISO/IEC JTC1 SC22 WG14 N1169 Extension
            TST_Fract,
            TST_BFloat16,
            TST_float,
            TST_double,
            TST_float128,
            TST_ibm128,
            TST_bool,         // _Bool
            TST_decimal32,    // _Decimal32
            TST_decimal64,    // _Decimal64
            TST_decimal128,   // _Decimal128
            TST_enum,
            TST_union,
            TST_struct,
            TST_class,        // C++ class type
            TST_interface,    // C++ (Microsoft-specific) __interface type
            TST_typename,     // Typedef, C++ class-name or enum name, etc.
            TST_typeofType,
            TST_typeofExpr,
            TST_decltype,         // C++11 decltype
            TST_underlyingType,   // __underlying_type for C++11
            TST_auto,             // C++11 auto
            TST_decltype_auto,    // C++1y decltype(auto)
            TST_auto_type,        // __auto_type extension
            TST_unknown_anytype,  // __unknown_anytype extension
            TST_atomic,           // C11 _Atomic
            TST_error // erroneous type
        };
    }
}

#endif //__CORE_YCOMPILER_BASIC_YSPECIFIERS_HPP__