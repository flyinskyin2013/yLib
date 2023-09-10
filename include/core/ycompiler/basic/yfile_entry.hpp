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
 * @FilePath: \yLib\include\core\ycompiler\basic\yfile_entry.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_BASIC_YFILE_ENTRY_HPP__
#define __CORE_YCOMPILER_BASIC_YFILE_ENTRY_HPP__

#include <memory>
#include <string>
#include <cstdint>

#include "core/yobject.hpp"

#include "core/ycompiler/basic/ymemory_buffer.hpp"
namespace yLib
{
    namespace ycompiler
    {
        class yFileManager;
        /// Cached information about one file (either on disk
        /// or in the virtual file system).
        ///
        /// If the 'File' member is valid, then this FileEntry has an open file
        /// descriptor for the file.
        class __YLIB_CLASS_DECLSPEC__ yFileEntry
        {
            friend class yFileManager;
            private:
            std::string file_path; // path to the file; could be empty.
            std::string dir_path; // Directory file lives in.
            uint64_t file_size; // File size in bytes.

            std::unique_ptr<yMemoryBuffer> content;

            public:
            yFileEntry(){}
            yFileEntry(const yFileEntry&) = delete;
            yFileEntry& operator=(const yFileEntry&) = delete;

            const std::string & get_file_path(void){return file_path;}
            const std::string & get_dir_path(void){return dir_path;}
            const uint64_t & get_file_size(void){return file_size;}
            
        };
    }
}

#endif //__CORE_YCOMPILER_BASIC_YFILE_ENTRY_HPP__