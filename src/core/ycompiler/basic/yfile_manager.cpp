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
 * @LastEditTime: 2021-11-27 11:22:55
 * @Description: 
 * @FilePath: /yLib/src/core/ycompiler/basic/yfile_manager.cpp
 * https://github.com/flyinskyin2013/yLib
 */


#include <fstream>
#include "core/ycompiler/frontend/ycompiler_instance.hpp"

#include "core/ycompiler/basic/yfile_manager.hpp"
#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

int32_t yFileManager::reference_count = 0;

yFileManager::yFileManager(yCompilerInstance & ci)
:ci(ci)
{}

yFileManager::~yFileManager(){}

yFileManager * yFileManager::GetInstance(yCompilerInstance & ci){

    yFileManager * _self_ins = new (std::nothrow) yFileManager(ci);
    if (nullptr == _self_ins){//check new 

        yLib::yLog::E("Create instance for yFileManager failed.");
        return nullptr;
    }

    return _self_ins;
}

yFileEntry * yFileManager::GetFileEntry(const std::string & file_path)
{
    if (file_entry_map.count(file_path) > 0)
        return file_entry_map[file_path];
    
    return nullptr;
}

yFileEntry * yFileManager::GetFileEntry(yFileID file_id)
{
    if (file_entry_vec.size() <= file_id.GetRawID()){

        yLog::E("yFileManager::GetFileEntry error, file_entry_vec.size[%d] not <= file_id.GetRawID[%d]\n", \
            file_entry_vec.size(), file_id.GetRawID());
        return nullptr;
    }
    
    return file_entry_vec[file_id.GetRawID()].get();
}

yMemoryBuffer * yFileManager::GetFileMemroyBuffer(const std::string & file_path)
{
    if (file_memory_buffer_map.count(file_path) > 0)
        return file_memory_buffer_map[file_path];
    
    return nullptr;
}

yFileID yFileManager::GetFileID(yFileEntry * file_entry)
{
    uint32_t _i = 0; 
    for(; _i < file_entry_vec.size(); _i++){

        if (file_entry_vec[_i].get() == file_entry)
            return yFileID(_i);
    }

    // the file entry not found, we should show some msg

    return yFileID(0);//
}

bool yFileManager::open_and_cache_file(const std::string & file_path)
{
    //we has cached this file
    if (0 < file_entry_map.count(file_path) && 0 < file_memory_buffer_map.count(file_path)){

        return true;
    }

    auto _file_entry = std::unique_ptr<yFileEntry>(new yFileEntry());

    _file_entry->file_path = file_path;
    _file_entry->content = yMemoryBuffer::get_file_memory_buf(file_path);
    if (_file_entry->content == nullptr)
        return false;

    _file_entry->file_size = _file_entry->content->get_buf_size() - 1;//more one byte　'0'

    file_entry_map.insert({file_path, _file_entry.get()});
    file_memory_buffer_map.insert({file_path, _file_entry->content.get()});

    file_entry_vec.emplace_back(std::move(_file_entry));

    return true;
}


yFileID yFileManager::GetCachedMaxFileID(void)
{
    return yFileID(file_entry_vec.size() - 1);
}