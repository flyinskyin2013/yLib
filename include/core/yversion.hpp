/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 11:27:55
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-15 17:42:11
 * @FilePath: \yLib\include\core\yversion.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef __YLIB_CORE_YVERSION_HPP__
#define __YLIB_CORE_YVERSION_HPP__

namespace yLib{

    /** \def YLIB_STR
        \brief Convert __S to string.
    */
    #define YLIB_STR(__S) YLIB_STR_CONVERT(__S)

    /** \def YLIB_STR_CONVERT
        \brief Convert __S to string.
    */
    #define YLIB_STR_CONVERT(__S) #__S

    // #define YLIB_VERSION_MAJOR 0
    // #define YLIB_VERSION_MINOR 0
    // #define YLIB_VERSION_PATCH 7

    // #define YLIB_VERSION YLIB_STR(YLIB_VERSION_MAJOR) "." YLIB_STR(YLIB_VERSION_MINOR) "." YLIB_STR(YLIB_VERSION_PATCH)

}



#endif //__YLIB_CORE_YVERSION_HPP__