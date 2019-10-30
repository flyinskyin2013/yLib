/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:07
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-28 16:33:13
 * @Description: 
 */

#include "ycommon.hpp" 

std::string  yLib::yCommon::GetyLibVersion() const noexcept{


    return std::string("V") + std::to_string(YLIB_VERSION_MAJOR) + "." + std::to_string(YLIB_VERSION_MINOR0) + "." + std::to_string(YLIB_VERSION_MINOR1);;
}