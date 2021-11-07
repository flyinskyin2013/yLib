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
 * @Date: 2021-03-20 14:31:59
 * @LastEditors: Sky
 * @LastEditTime: 2021-05-26 15:15:36
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__
#define __YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>


#include "core/yobject.hpp"

namespace yLib
{
    
    class __YLIB_CLASS_DECLSPEC__ yFileBufferManager MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::vector<int8_t> file_buffer;
        uint64_t process_pos = 0;
        uint64_t cur_row = 0;
        uint64_t cur_column = 0;
    public:
        yFileBufferManager(/* args */);
        ~yFileBufferManager();

        //read api
        int8_t LoadSourceFile(const std::string &src_file);
        int8_t LoadSourceFile(const char * src_file);

        //
        int8_t GetNext(void);
        void BackPos(uint64_t num);

        uint64_t GetCurColumn(void);
        uint64_t GetCurRow(void);
    };

} // namespace yLib


#endif //__YLIB_CORE_YCOMPILER_CORE_YFILEBUFFERMANAGER_HPP__