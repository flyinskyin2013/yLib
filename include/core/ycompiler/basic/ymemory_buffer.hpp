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
 * @Date: 2023-04-30 12:23:45
 * @LastEditTime: 2023-05-02 09:41:04
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\basic\ymemory_buffer.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_BASIC_YMEMORY_BUFFER_HPP__
#define __CORE_YCOMPILER_BASIC_YMEMORY_BUFFER_HPP__

#include <memory>
#include <string>
#include <cstdint>

#include "core/yobject.hpp"

namespace yLib
{
    namespace ycompiler
    {

        /// This interface provides simple read-only access to a block of memory, and
        /// provides simple methods for reading files and standard input into a memory
        /// buffer.  In addition to basic access to the characters in the file, this
        /// interface guarantees you can read one character past the end of the file,
        /// and that this character will read as '\0'.
        ///
        /// The '\0' guarantee is needed to support an optimization -- it's intended to
        /// be more efficient for clients which are reading all the data to stop
        /// reading when they encounter a '\0' than to continually check the file
        /// position to see if it has reached the end of the file.
        class __YLIB_CLASS_DECLSPEC__ yMemoryBuffer
        {
            private:
            uint8_t * buf_start;
            uint8_t * buf_end;
            std::unique_ptr<uint8_t> mem_ins;

            protected:
            yMemoryBuffer() =  default;

            public:
            yMemoryBuffer(uint64_t buf_size);
            yMemoryBuffer(const yMemoryBuffer &) = delete;
            yMemoryBuffer &operator=(const yMemoryBuffer &) = delete;

            virtual ~yMemoryBuffer(){}           

            uint8_t * get_buf_start() {return buf_start;}
            uint8_t * get_buf_end() {return buf_end;}
            uint64_t get_buf_size() {return buf_end - buf_start;}

            static std::unique_ptr<yMemoryBuffer> get_file_memory_buf(const std::string & file_path);
        };
    }
}

#endif //__CORE_YCOMPILER_BASIC_YMEMORY_BUFFER_HPP__