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
 * @FilePath: \yLib\include\core\ycompiler\basic\ysource_manager.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/basic/ysource_manager.hpp"

using namespace yLib::ycompiler;

uint64_t ySourceManager::get_file_offset(yFileEntry & file_entry, uint32_t row, uint32_t col)
{
    yMemoryBuffer * _mem_buf = file_mgr.GetFileMemroyBuffer(file_entry.get_file_path());
    const char * _file_content_ptr = (char *)_mem_buf->get_buf_start();

    uint32_t _row = 1;
    uint32_t _col = 1;
    for (; _file_content_ptr != (char *)_mem_buf->get_buf_end(); _file_content_ptr++){

        if (_row == row && _col == col)//find row, col
            break;

        if ('\r' == *_file_content_ptr){

            _file_content_ptr ++;

            if (_file_content_ptr == (char *)_mem_buf->get_buf_end())//get end of buf, wrong row, col
                break;
            
            _row ++;
            _col = 1;
        }
        else if ('\n' == *_file_content_ptr){

            _row ++;
            _col = 1;
        }
        else{

            _col ++;
        }
    }

    return _file_content_ptr - (char*)_mem_buf->get_buf_start();
}


ySourceLocation ySourceManager::translate_file_row_col(yFileEntry & file_entry, uint32_t row, uint32_t col)
{
    
    return ySourceLocation(file_mgr.GetFileID(const_cast<yFileEntry *>(&file_entry)), get_file_offset(file_entry, row, col));
}

ySourceLocation ySourceManager::translate_row_col(yFileID file_id, uint32_t row, uint32_t col)
{
    yFileEntry * _file_entry = file_mgr.GetFileEntry(file_id);
    return ySourceLocation(file_id, get_file_offset(*_file_entry, row, col));
}

std::pair<uint32_t, uint32_t> ySourceManager::get_row_col_impl(yFileID file_id, uint32_t file_offset)
{

    yFileEntry * _file_entry = file_mgr.GetFileEntry(file_id);
    yMemoryBuffer * _mem_buf = file_mgr.GetFileMemroyBuffer(_file_entry->get_file_path());
    const char * _file_content_ptr = (char *)_mem_buf->get_buf_start();

    uint32_t _row = 1;
    uint32_t _col = 1;
    for (; _file_content_ptr != (char *)_mem_buf->get_buf_end(); _file_content_ptr++){

        if (file_offset == (_file_content_ptr - (char*)_mem_buf->get_buf_start()))//find offset
            break;

        if ('\r' == *_file_content_ptr){

            _file_content_ptr ++;

            if (_file_content_ptr == (char *)_mem_buf->get_buf_end())//get end of buf
                break;
            
            _row ++;
            _col = 1;
        }
        else if ('\n' == *_file_content_ptr){

            _row ++;
            _col = 1;
        }
        else{

            _col ++;
        }
    }

    return {_row, _col};
}

uint32_t ySourceManager::get_col_num(yFileID file_id, uint32_t file_offset)
{
    return get_row_col_impl(file_id, file_offset).second;
}
uint32_t ySourceManager::get_file_col_num(ySourceLocation loc)
{
    return get_row_col_impl(loc.GetFileID(), loc.GetOffset()).second;
}

uint32_t ySourceManager::get_row_num(yFileID file_id, uint32_t file_offset)
{
    return get_row_col_impl(file_id, file_offset).first;
}
uint32_t ySourceManager::get_file_row_num(ySourceLocation loc)
{
    return get_row_col_impl(loc.GetFileID(), loc.GetOffset()).first;
}