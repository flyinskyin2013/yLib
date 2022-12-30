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
 * @Date: 2021-09-18 15:51:57
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 16:42:51
 * @Description: 
 */
#ifndef __YLIB_C_HPP__
#define __YLIB_C_HPP__

#ifndef __YLIB_EXPORT_DECLSPEC__
#ifdef _WIN32

    #define __YLIB_EXPORT_DECLSPEC__ __declspec(dllexport)

#elif __linux__ || __linux

    #define __YLIB_EXPORT_DECLSPEC__

#elif __unix__ || __unix

#endif //__unix__ || __unix

#endif //__YLIB_EXPORT_DECLSPEC__


#define YLIB_C_API \
    extern "C"



#include <stdint.h>



// export class ylog, no override
/*
    typedef enum __em_ylog_level__:uint16_t{

        DISABLE_ALL_LOG_LEVEL = 0x0000,

        ENABLE_DEBUG_LOG_LEVEL = 0x0001,
        DISABLE_DEBUG_LOG_LEVEL = (uint16_t)~ENABLE_DEBUG_LOG_LEVEL,

        ENABLE_INFO_LOG_LEVEL = 0x0002,
        DISABLE_INFO_LOG_LEVEL = (uint16_t)~ENABLE_INFO_LOG_LEVEL,

        ENABLE_WARN_LOG_LEVEL = 0x0004,
        DISABLE_WARN_LOG_LEVEL = (uint16_t)~ENABLE_WARN_LOG_LEVEL,

        ENABLE_ERROR_LOG_LEVEL = 0x0008,
        DISABLE_ERROR_LOG_LEVEL = (uint16_t)~ENABLE_ERROR_LOG_LEVEL,

        ENABLE_ALL_LOG_LEVEL = 0xFFFF,
    } yLogLevel;
*/
__YLIB_EXPORT_DECLSPEC__ struct yLogTagPropertyC{

    uint16_t log_level = 0xFFFF;
    bool is_log_to_file = false;
    bool is_log_to_stdio = true;

    const char * file_base_name = "";
    const char * file_dir = "";
    /**
     *  @var    multi_log_file
     *  @brief if true store multi-log-file by tag. default value is false. 
     */  
    bool multi_log_file = false;

    bool flush_every_times = true;

    // default 20MB 1024*1024*20 = 20971520
    uint64_t log_file_max_size = 20971520;

    uint32_t log_file_max_backup_num = 2;

    // (ms)
    uint32_t flush_timeout = 200;
};

YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_init(const yLogTagPropertyC * tag_prop, const char * tag);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_i(const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_i_tag(const char * tag, const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_w(const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_w_tag(const char * tag, const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_d(const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_d_tag(const char * tag, const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_e(const char * fmt, ...);
YLIB_C_API __YLIB_EXPORT_DECLSPEC__ void ylib_ylog_e_tag(const char * tag, const char * fmt, ...);


#endif //__YLIB_C_HPP__