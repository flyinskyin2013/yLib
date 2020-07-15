/*
 * @Author: Sky
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:30:58
 * @Description: 
 */

#ifndef __YLIB_CORE_YCOMMON_HPP__
#define __YLIB_CORE_YCOMMON_HPP__

#include <string>

#include "yobject.hpp"


namespace yLib{
    


    class __YLIB_EXPORT__ yCommon MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::string _ylib_version;
    public:
        yCommon(/* args */) noexcept MACRO_INIT_YOBJECT_PROPERTY(yCommon){}
        ~yCommon()noexcept{}
        yCommon(yCommon & common) = delete;
        yCommon & operator=(yCommon & common) = delete;
        yCommon(yCommon && common) = delete;
        yCommon & operator=(yCommon && common) =delete;

        std::string GetyLibVersion() const noexcept;
        std::string GetyLibBuildInfo() const noexcept;
    };

 }

#endif //__YLIB_CORE_YCOMMON_HPP__