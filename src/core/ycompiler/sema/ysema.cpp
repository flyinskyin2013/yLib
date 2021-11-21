/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: your name
 * @Date: 2021-11-27 14:22:41
 * @LastEditTime: 2021-11-27 14:22:42
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\sema\ysema.cpp
 */

#include "core/ycompiler/sema/ysema.hpp"
#include "core/ycompiler/parse/yconfig_parser.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

int8_t ySema::ParseNumberConstantExpression(uint64_t & num)
{
    // process sign
    int8_t _sign = 1;
    if (parser->cur_token.kind == tok::plus){

        parser->lexer.NextToken(parser->cur_token);
    }
    else if (parser->cur_token.kind == tok::minus){

        parser->lexer.NextToken(parser->cur_token);
        _sign = -1;
    }

    if (parser->cur_token.kind == tok::numeric_constant){

        std::string _str((char *)parser->cur_token.token_data, parser->cur_token.token_data_len);
        if (_str.find('.') == _str.npos){//int

            char * _end_char = nullptr;
            int64_t _ret = std::strtoll(_str.c_str(), &_end_char, 0) * _sign;
            if (*_end_char != '\0'){

                LOGE("ySema")<<"parse a numeric constant(int64_t) failed.";
                return -1;
            }

            ::memcpy((char*)&num, (char *)&_ret, sizeof(int64_t));
            return 1;
            
        }
        else{//float

            char * _end_char = nullptr;
            double _ret = std::strtod(_str.c_str(), &_end_char) * _sign;
            if (*_end_char != '\0'){

                LOGE("ySema")<<"parse a numeric constant(double) failed.";
                return -1;
            }

            ::memcpy((char*)&num, (char *)&_ret, sizeof(double));
            return 2;
            
        }

        
    }
    else{

        yDiagnosticsEngine::DiagReport(parser->cur_token, diag::ysema_error_expect_numconstant);
        return -1;
    }
    

    return -1;

}