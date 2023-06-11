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
 * @Date: 2021-11-21 09:35:56
 * @LastEditTime: 2021-11-21 09:36:58
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\basic\ydiagnostics.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

static const char * DiagTageNameTable[] = {

    #define DIAG_INFO(diag_name, tag_name, level, info, argnum) \
        #tag_name,

    #include "core/ycompiler/basic/ydiagnostic_kinds.def"
    nullptr
    #undef DIAG_INFO
};

static const char * DiagInfoTable[] = {

    #define DIAG_INFO(diag_name, tag_name, level, info, argnum) \
        info,

    #include "core/ycompiler/basic/ydiagnostic_kinds.def"
    nullptr
    #undef DIAG_INFO
};

static yLogSeverity DiagLogLevelTable[] = {

    #define DIAG_INFO(diag_name, tag_name, level, info, argnum) \
        level,

    #include "core/ycompiler/basic/ydiagnostic_kinds.def"
    #undef DIAG_INFO
};

static int DiagArgNumTable[] = {

    #define DIAG_INFO(diag_name, tag_name, level, info, argnum) \
        argnum,

    #include "core/ycompiler/basic/ydiagnostic_kinds.def"
    #undef DIAG_INFO
};

void yDiagnosticsEngine::DiagReport(ySourceLocation & loc, diag::DiagID id)
{
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    if (DiagArgNumTable[id] == 0)
        yLog::E(DiagTageNameTable[id], "%s",DiagInfoTable[id]);
    
    
    std::string _str;
    uint64_t _row = src_mgr.get_row_num(loc.GetFileID(), loc.GetOffset());
    uint64_t _col = src_mgr.get_col_num(loc.GetFileID(), loc.GetOffset());
    
    yLog::E("", "File: %s:%d:%d\n", src_mgr.GetFileManager().GetFileEntry(loc.GetFileID())->get_file_path().c_str(), _row, _col);
#endif //YLIB_CODECOVERAGE_SKIP_CODE
}

void yDiagnosticsEngine::DiagReport(yToken &token, diag::DiagID id){
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    yLib::yLog::E("Error%d, token type %s\n", id, tok::getTokenName(token.kind));
    if (DiagArgNumTable[id] == 0)
        yLog::E(DiagTageNameTable[id], "%s",DiagInfoTable[id]);

    else if (DiagArgNumTable[id] == 1){

        std::string _tok_name((const char *)token.token_data, token.token_data_len);
        yLog::E(DiagTageNameTable[id], DiagInfoTable[id], _tok_name.c_str());
    }

    ySourceLocation _loc = token.getLocation();
    std::string _str((char *)token.token_data, token.token_data_len);
    uint64_t _row = src_mgr.get_row_num(_loc.GetFileID(), _loc.GetOffset());
    uint64_t _col = src_mgr.get_col_num(_loc.GetFileID(), _loc.GetOffset());
    
    yLog::E("", "File: %s:%d:%d\n", src_mgr.GetFileManager().GetFileEntry(_loc.GetFileID())->get_file_path().c_str(), _row, _col);
    yLog::E("", "%s\n", _str.c_str());
#endif //YLIB_CODECOVERAGE_SKIP_CODE
}


yDiagnosticsEngine::yDiagnosticsEngine(std::unique_ptr<yDiagnosticsIDHandle> &&diag_id_handle, 
                    std::unique_ptr<yDiagnosticOptions> &&diag_options,
                    std::unique_ptr<yDiagnosticConsumer> &&diag_consumer, ySourceManager& src_mgr)
:src_mgr(src_mgr)
{
    this->diag_id_handle = std::move(diag_id_handle);
    this->diag_options = std::move(diag_options);
    this->diag_consumer = std::move(diag_consumer);
}



yDiagnosticOptions::yDiagnosticOptions(){}
yDiagnosticOptions::~yDiagnosticOptions(){}

yDiagnosticsIDHandle::yDiagnosticsIDHandle(){}
yDiagnosticsIDHandle::~yDiagnosticsIDHandle(){}


std::string yDiagnosticsIDHandle::get_description(diag::DiagID id)
{
    return "";
}


yDiagnosticConsumer::yDiagnosticConsumer(){}
yDiagnosticConsumer::~yDiagnosticConsumer(){}