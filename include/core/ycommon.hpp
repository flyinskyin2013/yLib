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