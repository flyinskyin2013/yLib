
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
 * @Date: 2021-11-20 12:23:45
 * @LastEditTime: 2021-11-21 09:41:04
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\basic\ysource_location.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_BASIC_YSOURCE_LOCATION_HPP__
#define __CORE_YCOMPILER_BASIC_YSOURCE_LOCATION_HPP__

#include <string>
#include <cstdint>
namespace yLib
{
    namespace ycompiler
    {
        /// Encodes a location in the source. The SourceManager can decode this
        /// to get at the full include stack, line and column information.
        ///
        /// Technically, a source location is simply an offset into the manager's view
        /// of the input source, which is all input buffers (including macro
        /// expansions) concatenated in an effectively arbitrary order. The manager
        /// actually maintains two blocks of input buffers. One, starting at offset
        /// 0 and growing upwards, contains all buffers from this module. The other,
        /// starting at the highest possible offset and growing downwards, contains
        /// buffers of loaded modules.
        ///
        /// In addition, one bit of SourceLocation is used for quick access to the
        /// information whether the location is in a file or a macro expansion.
        ///
        /// It is important that this type remains small. It is currently 32 bits wide.
        class ySourceLocation{
            private:
            uint64_t offset;
            
            public:

            /// Turn a raw encoding of a SourceLocation object into
            /// a real SourceLocation.
            ///
            /// \see getRawEncoding.
            static ySourceLocation getFromRawEncoding(uint64_t Encoding) {
                ySourceLocation X;
                X.offset = Encoding;
                return X;
            }

        };
    } // namespace ycompiler
} // namespace yLib

#endif //__CORE_YCOMPILER_BASIC_YSOURCE_LOCATION_HPP__