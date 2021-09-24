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
YLIB_C_API void ylib_ylog_i(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_INFO, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API void ylib_ylog_i_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_INFO, fmt, _arg, tag);

    va_end(_arg);
}

YLIB_C_API void ylib_ylog_w(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_WARN, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API void ylib_ylog_w_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_WARN, fmt, _arg, tag);

    va_end(_arg);
}

YLIB_C_API void ylib_ylog_d(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_DEBUG, fmt, _arg, "");

    va_end(_arg);
}

YLIB_C_API void ylib_ylog_d_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_DEBUG, fmt, _arg, tag);

    va_end(_arg);
}
YLIB_C_API void ylib_ylog_e(const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_ERROR, fmt, _arg, "");

    va_end(_arg);
}
YLIB_C_API void ylib_ylog_e_tag(const char * tag, const char * fmt, ...){

    va_list _arg ;

    va_start(_arg, fmt);

    yLib::yLog::convert_fmt_to_str(yLib::yLogSeverity::LOG_ERROR, fmt, _arg, tag);

    va_end(_arg);
}

/********************************yLog c api********************************************/
/********************************yLog c api********************************************/
/********************************yLog c api********************************************/