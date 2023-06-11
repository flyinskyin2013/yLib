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
 * @FilePath: \yLib\include\core\ycompiler\basic\ymemory_buffer.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include <fstream>
#include <memory>
#include <string>
#include <cstdint>

#include "core/ycompiler/basic/ymemory_buffer.hpp"
#include "core/ylog.hpp"

using namespace yLib::ycompiler;

yMemoryBuffer::yMemoryBuffer(uint64_t buf_size)
{
    mem_ins = std::unique_ptr<uint8_t>(new (std::nothrow) uint8_t[buf_size]);

    buf_start = mem_ins.get();
    buf_end = buf_start + buf_size;

    ::memset(buf_start, 0, buf_size);
}

std::unique_ptr<yMemoryBuffer> yMemoryBuffer::get_file_memory_buf(const std::string & file_path)
{
    std::unique_ptr<yMemoryBuffer> _file_buf;
    std::ifstream _in_file(file_path);

    if (!_in_file.is_open()){

        yLog::E("yFileManager", "Can't open input-c-file(%s)!!!\n", file_path.c_str());
        return _file_buf;        
    }

    _in_file.seekg(0, _in_file.end);
    uint64_t _content_size = _in_file.tellg();
    _in_file.seekg(0, _in_file.beg);    

    uint64_t content_buf_size = _content_size + 1;        

    _file_buf = std::unique_ptr<yMemoryBuffer>(new yMemoryBuffer(content_buf_size));

    if (nullptr == _file_buf.get()) {

        yLog::E("yFileManager", "Can't open input-c-file(%lu)!!!\n", content_buf_size);
        return _file_buf;
    }

    //read file-content
    _in_file.read((char *)_file_buf->get_buf_start(), _content_size);

    _in_file.close();

    return std::move(_file_buf);
}