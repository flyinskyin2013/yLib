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
 * @Date: 2021-09-18 16:42:20
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-24 14:48:40
 * @Description: 
 */

#include "ylib_c.h"

#include "ylib.hpp"

#include <cstdarg>


/********************************yLog c api********************************************/
/********************************yLog c api********************************************/
/********************************yLog c api********************************************/
//yLog c api
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_init(const yLogTagPropertyC * tag_prop, const char * tag){

    yLib::yLogTagProperty _prop;
    _prop.is_log_to_file = tag_prop->is_log_to_file;
    _prop.is_log_to_stdio = tag_prop->is_log_to_stdio;
    _prop.log_level = tag_prop->log_level;

    _prop.file_param.file_base_name = tag_prop->file_base_name;
    _prop.file_param.file_dir = tag_prop->file_dir;
    _prop.file_param.flush_every_times = tag_prop->flush_every_times;
    _prop.file_param.flush_timeout = tag_prop->flush_timeout;
    _prop.file_param.log_file_max_backup_num = tag_prop->log_file_max_backup_num;
    _prop.file_param.log_file_max_size = tag_prop->log_file_max_size;
    _prop.file_param.multi_log_file = tag_prop->multi_log_file;

    std::string _tag = tag;
    yLib::yLog::Init(_prop, tag);

}
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_i(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_INFO, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_i_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_INFO, fmt, _arg, tag);

    va_end(_arg);
}

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_w(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_WARN, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_w_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_WARN, fmt, _arg, tag);

    va_end(_arg);
}

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_d(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_DEBUG, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_d_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_DEBUG, fmt, _arg, tag);

    va_end(_arg);
}
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_e(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_ERROR, fmt, _arg, "");

    va_end(_arg);
}
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_e_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_ERROR, fmt, _arg, tag);

    va_end(_arg);
}

/********************************yLog c api********************************************/
/********************************yLog c api********************************************/
/********************************yLog c api********************************************/