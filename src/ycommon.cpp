/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:07
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-19 11:40:00
 * @Description: 
 */

#include "ycommon.hpp" 

std::string  yLib::yCommon::GetyLibVersion() const noexcept{


    //return std::string("V") + std::to_string(YLIB_VERSION_MAJOR) + "." + std::to_string(YLIB_VERSION_MINOR) + "." + std::to_string(YLIB_VERSION_PATCH);
    return std::string("V") + YLIB_VERSION;
}