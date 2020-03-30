/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 11:27:55
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-19 11:47:15
 * @FilePath: \yLib\include\core\yversion.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef __YLIB_CORE_YVERSION_HPP__
#define __YLIB_CORE_YVERSION_HPP__

namespace yLib{

    #define YLIB_STR(__S) YLIB_STR_CONVERT(__S)
    #define YLIB_STR_CONVERT(__S) #__S

    #define YLIB_VERSION_MAJOR 0
    #define YLIB_VERSION_MINOR 0
    #define YLIB_VERSION_PATCH 7

    #define YLIB_VERSION YLIB_STR(YLIB_VERSION_MAJOR) "." YLIB_STR(YLIB_VERSION_MINOR) "." YLIB_STR(YLIB_VERSION_PATCH)

}



#endif //__YLIB_CORE_YVERSION_HPP__