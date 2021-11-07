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
 * @LastEditTime: 2021-05-26 15:18:37
 * @Description: 
 */


#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "utility/ylog.hpp"

#include "core/ycompiler/core/yfilebuffermanager.hpp"

using namespace yLib;

yFileBufferManager::yFileBufferManager(/* args */)
MACRO_INIT_YOBJECT_PROPERTY(yFileBufferManager),
process_pos(0),
cur_row(1),
cur_column(1)
{
}

yFileBufferManager::~yFileBufferManager()
{
}

int8_t yFileBufferManager::LoadSourceFile(const std::string &src_file)
{return LoadSourceFile(src_file.c_str());}

int8_t yFileBufferManager::LoadSourceFile(const char *src_file)
{

    std::ifstream _input_file;
    
    //open the src file
    _input_file.open(src_file, std::ifstream::in);
    if (!_input_file.is_open()){

        yLib::yLog::E("Open source file(%s) failed.", src_file);
        return -1;
    }

    //get len of src-file
    _input_file.seekg (0, _input_file.end);
    uint64_t _input_file_len = _input_file.tellg();
    _input_file.seekg (0, _input_file.beg);

    //allocate memory for file_buffer
    file_buffer.resize((8 - _input_file_len%8) + _input_file_len);//8bytes align

    //read src-file to file_buffer
    _input_file.read ( (char *)(file_buffer.data()), _input_file_len);

    if (_input_file_len != _input_file.gcount()){

        yLib::yLog::E("The file len is %ld, only %ld bytes could be read.", _input_file_len, _input_file.gcount());
        _input_file.close();
        return -1;        
    }

    _input_file.close();


    process_pos = 0;//reset the pos
    cur_column = 1;//reset column
    cur_row = 1;//reset row
    return 0;
}

int8_t yFileBufferManager::GetNext(void)
{

    int8_t _tmp_char;

    //check now pos
    if (process_pos  >= file_buffer.size())//read  all bytes
        return -1;

RE_GET_CHAR:
    _tmp_char = file_buffer[process_pos];
    process_pos++;

    //process cur_row cur_column
    if (0x1F >= _tmp_char){//special char
        
        if ('\n' == _tmp_char){//0xa  new line

            cur_row++;
            cur_column = 0;
            goto RE_GET_CHAR;//
        }
        else {//other special char, include '\t' '\r' ... ...

            cur_column++;
            goto RE_GET_CHAR;//
        }
    }

    return _tmp_char;
}

void yFileBufferManager::BackPos(uint64_t num)
{

    if (process_pos >= num){
        
        process_pos = process_pos - num;//process pos

        for (uint64_t _i = 0; _i < num; _i++){//process cur_row cur_column

            if ('\n' == file_buffer[process_pos - _i]){

                cur_row --;
                cur_column = 1;
            }
            else{

                cur_column --;
            }
        }
    }
    else{
    
        process_pos = 0;
        cur_column = 1;
        cur_row = 1;
    }
}


uint64_t yFileBufferManager::GetCurColumn(void)
{return cur_column;}

uint64_t yFileBufferManager::GetCurRow(void)
{return cur_row;}