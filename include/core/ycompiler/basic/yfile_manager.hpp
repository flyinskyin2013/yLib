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
 * @Date: 2021-11-20 09:22:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-27 11:06:11
 * @Description: 
 * @FilePath: /yLib/include/core/ycompiler/basic/yfile_manager.hpp
 * https://github.com/flyinskyin2013/yLib
 */



#ifndef __CORE_YCOMPILER_BASIC_YFILE_MANAGER_HPP__
#define __CORE_YCOMPILER_BASIC_YFILE_MANAGER_HPP__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstdint>


#include "core/yobject.hpp"
#include "core/ycompiler/basic/yfile_entry.hpp"
#include "core/ycompiler/basic/ysource_location.hpp"
namespace yLib
{
    namespace ycompiler
    {
        class yCompilerInstance;

        /// Implements support for file system lookup, file system caching,
        /// and directory search management.
        ///
        /// This also handles more advanced properties, such as uniquing files based
        /// on "inode", so that a file with two names (e.g. symlinked) will be treated
        /// as a single file.
        ///
        class __YLIB_CLASS_DECLSPEC__ yFileManager:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            private:
            yCompilerInstance & ci;

            static int32_t reference_count;
            
            private:
            std::vector<std::unique_ptr<yFileEntry>> file_entry_vec;
            std::unordered_map<std::string, yFileEntry *> file_entry_map;
            std::unordered_map<std::string, yMemoryBuffer *> file_memory_buffer_map;

            protected:
            yFileManager(yCompilerInstance & ci);
            yFileManager(const yFileManager&) = delete;
            yFileManager& operator=(const yFileManager&) = delete;
            public:
            ~yFileManager(void);

            bool InitFileManager(const std::string & file_name);

            const char * GetFileContentPtr(void);
            uint64_t GetFileContentSize(void);

            static yFileManager * GetInstance(yCompilerInstance & ci);

            yFileEntry * GetFileEntry(const std::string & file_path);
            yFileEntry * GetFileEntry(yFileID file_id);
            yMemoryBuffer * GetFileMemroyBuffer(const std::string & file_path);

            yFileID GetFileID(yFileEntry * file_entry);

            yFileID GetCachedMaxFileID(void);

            bool open_and_cache_file(const std::string & file_path);
            
        };
    } // namespace ycompiler
} // namespace yLib


#endif //__CORE_YCOMPILER_BASIC_YFILE_MANAGER_HPP__