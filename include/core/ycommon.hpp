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
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-12 10:18:01
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMMON_HPP__
#define __YLIB_CORE_YCOMMON_HPP__

#include <string>
#include <ctime>

#include "yobject.hpp"






namespace yLib{
    
    /**
     *  @class yCommon
     *  @brief This is common-class in yLib.
     */
    class __YLIB_CLASS_DECLSPEC__ yCommon:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    public:
    /**
     *  @fn    static YLIB_STD_STRING ConvertErrnoToStr(uint64_t err_num) noexcept
     *  @brief convert errno to str, it wrappers strerror_r() and FormatMessage().it's thread-safety.
     */
    static YLIB_STD_STRING ConvertErrnoToStr(uint64_t err_num) noexcept;

    static int  GetCurrentThreadId(void) noexcept;

    /**
     *  @fn    static void yLibGetUtcTimeAndLocalTime(struct ::timespec &time_spec, struct ::tm &tm_time) noexcept
     *  @brief convert utc time to time_spec and tm_time, tm_time.tm_yday can't be used in windows. It's like clock_gettime() and localtime_r() on linux.
     */
    static void GetUtcTimeAndLocalTime(struct ::timespec &time_spec, struct ::tm &tm_time) noexcept;


    static bool CheckFileExist(const char * full_name) noexcept;


    YLIB_DECLARE_CLASSINFO_CONTENT(yCommon);
    };

 }

#endif //__YLIB_CORE_YCOMMON_HPP__