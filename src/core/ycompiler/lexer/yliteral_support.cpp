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
 * @Date: 2023-08-26 14:22:45
 * @LastEditTime: 2023-08-26 14:52:45
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\sema\yliteral_support.cpp
 */

#include "core/ycompiler/lexer/yliteral_support.hpp"

#include "core/ylog/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yNumericLiteralParser::yNumericLiteralParser(yToken & tok)
{
    std::string _literal_str((char *)tok.token_data, tok.token_data_len);

    literal_str = _literal_str;

    if (std::string::npos == _literal_str.find(".")){//int

        is_integer_literal = true;
        is_float_literal = false;
    }
    else{//double

        is_integer_literal = false;
        is_float_literal = true;
    }
}

/// GetIntegerValue - Convert this numeric literal value to an APInt that
/// matches Val's input width.  If there is an overflow (i.e., if the unsigned
/// value read is larger than the APInt's bits will hold), set Val to the low
/// bits of the result and return true.  Otherwise, return false.
bool yNumericLiteralParser::GetIntegerValue(int64_t &Val)
{
    
    // Val = std::stoi(literal_str); //we need no exception
    char * _end_ptr = nullptr;

    Val = strtoll(literal_str.c_str(), &_end_ptr, 10);

    if (_end_ptr != literal_str.c_str() + literal_str.length()){

        yLib::yLog::E("we can't parse num = %s\n", literal_str.c_str());
        return false;
    }

    return true;
}

/// GetFloatValue - Convert this numeric literal to a floating value, using
/// the specified APFloat fltSemantics (specifying float, double, etc).
/// The optional bool isExact (passed-by-reference) has its value
/// set to true if the returned APFloat can represent the number in the
/// literal exactly, and false otherwise.
bool yNumericLiteralParser::GetFloatValue(double &Result)
{
    // Result = std::stod(literal_str);//we need no exception

    char * _end_ptr = nullptr;

    Result = strtod(literal_str.c_str(), &_end_ptr);

    if (_end_ptr != literal_str.c_str() + literal_str.length()){

        yLib::yLog::E("we can't parse num = %s\n", literal_str.c_str());
        return false;
    }
    return true;
}
