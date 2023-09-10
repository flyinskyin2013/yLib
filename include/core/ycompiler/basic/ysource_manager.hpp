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
 * @FilePath: \yLib\include\core\ycompiler\basic\ysource_manager.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_BASIC_YSOURCE_MANAGER_HPP__
#define __CORE_YCOMPILER_BASIC_YSOURCE_MANAGER_HPP__

#include <string>
#include <cstdint>

#include "core/yobject.hpp"

#include "core/ycompiler/basic/ysource_location.hpp"
#include "core/ycompiler/basic/yfile_entry.hpp"
#include "core/ycompiler/basic/yfile_manager.hpp"

namespace yLib
{
    namespace ycompiler
    {
        class yCompilerInstance;

        /// This class handles loading and caching of source files into memory.
        ///
        /// This object owns the MemoryBuffer objects for all of the loaded
        /// files and assigns unique FileID's for each unique \#include chain.
        ///
        /// The SourceManager can be queried for information about SourceLocation
        /// objects, turning them into either spelling or expansion locations. Spelling
        /// locations represent where the bytes corresponding to a token came from and
        /// expansion locations represent where the location is in the user's view. In
        /// the case of a macro expansion, for example, the spelling location indicates
        /// where the expanded token came from and the expansion location specifies
        /// where it was expanded.
        class __YLIB_CLASS_DECLSPEC__ ySourceManager
        {
            private:
            yCompilerInstance & ci;
            yFileManager& file_mgr;
            yFileID main_file_id;
            
            public:
            ySourceManager(yCompilerInstance &CI);

            /// Get the source location for the given file:line:col triplet.
            ///
            /// If the source file is included multiple times, the source location will
            /// be based upon the first inclusion.
            // ySourceLocation translate_file_row_col(yFileEntry & file_entry, uint32_t row, uint32_t col);
            // ySourceLocation translate_row_col(yFileID file_id, uint32_t row, uint32_t col);

            // uint64_t get_file_offset(yFileEntry& file_entry, uint32_t row, uint32_t col);


            uint32_t get_col_num(yFileID file_id, uint32_t file_offset);
            uint32_t get_file_col_num(ySourceLocation loc);

            uint32_t get_row_num(yFileID file_id, uint32_t file_offset);
            uint32_t get_file_row_num(ySourceLocation loc);


            std::pair<uint32_t, uint32_t> get_row_col_impl(yFileID file_id, uint32_t file_offset);


            yFileManager& GetFileManager(void) {return file_mgr;}

            /// Returns the FileID of the main source file.
            yFileID getMainFileID() const { return main_file_id; }

            /// Set the file ID for the main source file.
            void setMainFileID(yFileID FID) {
                main_file_id = FID;
            }
        };
    }
}

#endif //__CORE_YCOMPILER_BASIC_YSOURCE_MANAGER_HPP__