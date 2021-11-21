
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
 * @Date: 2021-11-27 15:38:10
 * @LastEditors: Sky
 * @LastEditTime: 2021-12-01 16:50:40
 * @Description: 
 * @FilePath: /yLib/src/core/ycompiler/basic/yfile_location.cpp
 * https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/basic/yfile_location.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;


bool yFileLocation::GetLocationInfo(std::string & line, uint64_t & row, uint64_t &col)
{
#ifndef YLIB_CODECOVERAGE_SKIP_CODE    
    const char * _tmp_buf_start = buf_start;
    const char * _line_start = _tmp_buf_start;

    row = 1;
    col = 0;

    while(_tmp_buf_start != buf_cur_ptr){

        if (*_tmp_buf_start == '\r'){

            _tmp_buf_start ++;
        }
        else if (*_tmp_buf_start == '\n'){

            row ++;

            _tmp_buf_start ++;
            _line_start = _tmp_buf_start;

            col = 0;
        }
        else{

            _tmp_buf_start ++;
        }
    }

    while (*_tmp_buf_start != '\n'){//get this line end

        _tmp_buf_start++;
    }

    _tmp_buf_start --;
    if (*_tmp_buf_start == '\r'){

        _tmp_buf_start--;
    }

    std::string _cur_line(_line_start, _tmp_buf_start - _line_start + 1);

    line = _cur_line;

    return true;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
}