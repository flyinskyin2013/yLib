/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:07
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-23 15:57:04
 * @Description: 
 */

#include "ycommon.hpp" 

std::string  yLib::yCommon::yCommon_Get_yLib_Version() const noexcept{


    return std::string("V") + std::to_string(YLIB_VERSION_MAJOR) + "." + std::to_string(YLIB_VERSION_MINOR0) + "." + std::to_string(YLIB_VERSION_MINOR1);;
}