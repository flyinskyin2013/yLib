/*
 * @Author: Sky
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 10:03:20
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMMON_HPP__
#define __YLIB_CORE_YCOMMON_HPP__

#include <string>

#include "yobject.hpp"
namespace yLib{
    
    /**
     *  @class yCommon
     *  @brief This is common-class in yLib.
     */
    class __YLIB_EXPORT__ yCommon:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    public:
    /**
     *  @fn    std::string ConvertErrnoToStr(uint64_t err_num) noexcept
     *  @brief convert errno to str, it wrappers strerror_r() and FormatMessage().it's thread-safety.
     */
    std::string ConvertErrnoToStr(uint64_t err_num) noexcept;

    YLIB_DECLARE_CLASSINFO_CONTENT(yCommon);
    };

 }

#endif //__YLIB_CORE_YCOMMON_HPP__