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
 * @Date: 2018-10-23 11:09:07
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 12:00:14
 * @Description: 
 */

#include "core/ycommon.hpp" 
#include "core/ylog/ylog.hpp"

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yCommon)

#include <iostream>

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <string.h>
#include <time.h>

    #ifdef _WIN32
        #include <Windows.h>
        #include <io.h>
    #elif __linux__ || __linux

        #include <unistd.h>
        #include <sys/syscall.h>   /* For SYS_xxx definitions */

    #elif __unix__ || __unix
    #endif //__unix__ || __unix



#ifdef __cplusplus
}
#endif //__cplusplus



/**
 *  @fn    YLIB_STD_STRING ConvertErrnoToStr(uint64_t err_num) noexcept
 *  @brief convert errno to str, it wrapper strerror_r() and FormatMessage().
 */
YLIB_STD_STRING yLib::yCommon::ConvertErrnoToStr(uint64_t err_num) noexcept
{
    YLIB_STD_STRING _error_msg;

#ifdef _WIN32

    HLOCAL LocalAddress = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, err_num, 0, (PTSTR)&LocalAddress, 0, NULL);
    
    _error_msg = (PTSTR)&LocalAddress;

    LocalFree(LocalAddress);

#elif __linux__ || __linux
    
    char _str_buf[50];
    ::memset(_str_buf, 0, 50);

    _error_msg = strerror_r(err_num, _str_buf, 50);

#endif 

    return _error_msg;
}


int yLib::yCommon::GetCurrentThreadId() noexcept{

#if defined(linux) || defined(__linux) || defined(__linux__)

    return syscall(__NR_gettid);

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

  return ::GetCurrentThreadId();

#else
    return -1;
#endif  // OS_LINUX

}

//utc time to time_spec
//local time to tm_time, tm_time.tm_yday can't be used in windows.
void yLib::yCommon::GetUtcTimeAndLocalTime(struct ::timespec &time_spec, struct ::tm &tm_time) noexcept {

#ifdef _WIN32

    //get tm_time
    {
        
        SYSTEMTIME _cur_systime;
        GetLocalTime(&_cur_systime);

        // convert SYSTEMTIME to struct ::tm
        tm_time.tm_sec = _cur_systime.wSecond;   // seconds after the minute - [0, 60] including leap second
        tm_time.tm_min = _cur_systime.wMinute;   // minutes after the hour - [0, 59]
        tm_time.tm_hour = _cur_systime.wHour;  // hours since midnight - [0, 23]
        tm_time.tm_mday = _cur_systime.wDay;  // day of the month - [1, 31]
        tm_time.tm_mon = _cur_systime.wMonth - 1;   // months since January - [0, 11]
        tm_time.tm_year = _cur_systime.wYear - 1900;  // years since 1900
        tm_time.tm_wday = _cur_systime.wDayOfWeek;  // days since Sunday - [0, 6]
        tm_time.tm_yday = -1;  // days since January 1 - [0, 365]
        // tm_time.tm_isdst; // daylight savings time flag
    }

    // get time_spec
    {
        static bool _is_get_utc_systime = false;
        static bool _enable_performance_counter = false;

        //A file time is a 64-bit value that represents the number of 100-nanosecond intervals that 
        //    have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal Time (UTC). 
        //    The system records file times when applications create, access, and write to files.
        static FILETIME _the_utc_filetime;
        static SYSTEMTIME _the_utc_systime;
        static LARGE_INTEGER _the_utctime_var;

        FILETIME _the_cur_filetime;
        // SYSTEMTIME _the_cur_systime;
        LARGE_INTEGER _the_curtime_var;

        if (!_is_get_utc_systime){

            _is_get_utc_systime = true;

            // set UTC start-time-point
            _the_utc_systime.wYear = 1970;
            _the_utc_systime.wMonth = 1;
            _the_utc_systime.wDay = 1;
            _the_utc_systime.wHour = 0;
            _the_utc_systime.wMinute = 0;
            _the_utc_systime.wSecond = 0;
            _the_utc_systime.wMilliseconds = 0;

            // Converts a system time to file time format. System time is based on Coordinated Universal Time (UTC).
            // https://docs.microsoft.com/en-us/windows/win32/api/timezoneapi/nf-timezoneapi-systemtimetofiletime
            if (!SystemTimeToFileTime(&_the_utc_systime, &_the_utc_filetime)){

                LOGE("yCommon")<<"yLibGetSystemLocalTime(): It's failed to convert systime to filetime .Failed Info = "<<yLib::yCommon::ConvertErrnoToStr(GetLastError());
                return ;
            }

            _the_utctime_var.QuadPart = _the_utc_filetime.dwHighDateTime;
            _the_utctime_var.QuadPart <<= 32;
            _the_utctime_var.QuadPart |= _the_utc_filetime.dwLowDateTime;
            
        }

        GetSystemTimeAsFileTime(&_the_cur_filetime);
        _the_curtime_var.QuadPart = _the_cur_filetime.dwHighDateTime;
        _the_curtime_var.QuadPart <<= 32;
        _the_curtime_var.QuadPart |= _the_cur_filetime.dwLowDateTime;
        
        _the_curtime_var.QuadPart -= _the_utctime_var.QuadPart;

        _the_curtime_var.QuadPart /= 10;//convert ns to us

        time_spec.tv_sec = _the_curtime_var.QuadPart / 1000000;
        time_spec.tv_nsec = (long)((_the_curtime_var.QuadPart - (time_spec.tv_sec * 1000000)) * 1000);
    }

#elif __linux__ || __linux

    clock_gettime(CLOCK_REALTIME, &time_spec);
    localtime_r(&time_spec.tv_sec, &tm_time);

#elif __unix__ || __unix
#endif //__unix__ || __unix

}


bool  yLib::yCommon::CheckFileExist(const char * full_name) noexcept
{

#ifdef _WIN32
    //if (0 != _access(full_name, 0))//io.h
    //    return false;
    //return true;
    WIN32_FIND_DATA _fd;
    HANDLE _file_handle = FindFirstFile(full_name, &_fd);
    
    //_fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
    BOOL _exsit = (_file_handle != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
    FindClose(_file_handle);
    return _exsit;

#elif __linux__ || __linux

    if (-1 == access(full_name, F_OK)){
        
        std::string _err_msg = std::string("CheckFileExist(): file(") + full_name +  ") is not exist.";
        LOGE("yCommon")<<_err_msg;
        return false;
    }
    return true;
#elif __unix__ || __unix
#endif //__unix__ || __unix
}