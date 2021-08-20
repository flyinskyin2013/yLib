/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 10:44:05
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-20 16:33:26
 * @FilePath: \yLib\include\core\ylibdefs.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __YLIB_CORE_YLIBDEFS_HPP__
#define __YLIB_CORE_YLIBDEFS_HPP__


namespace yLib{

    #ifdef _WIN32
        #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
        #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
    #endif //_WIN32


    //_GNUC__ >= 4  &&  __GNUC_MINOR__ >= 1
    //_MSC_VER >= 1400 //vs 2005 or higher

    #ifdef _WIN32

        #define __YLIB_EXPORT__ __declspec(dllexport)
        #define __YLIB_IMPORT__ __declspec(dllimport)

        #define __YLIB_DEPRECATED_ATTRIBUTE__ __declspec(deprecated)
        #define __YLIB_DEPRECATED_ATTRIBUTE_WITH_MSG__(msg) __declspec(deprecated(msg))
    #elif __linux__ || __linux

        #define __YLIB_EXPORT__
        #define __YLIB_IMPORT__

        #define __YLIB_DEPRECATED_ATTRIBUTE__ __attribute__((deprecated))
        #define __YLIB_DEPRECATED_ATTRIBUTE_WITH_MSG__(msg) __attribute__((deprecated))
    #elif __unix__ || __unix

    #endif //__unix__ || __unix



    //define some useful macroes ----------------------------   end

    // c99 for variadic macros
    // #define example(...) \
    //     do{ \
    //         char msg_buf[1024]; \
    //         sprintf(msg_buf, __VA_ARGS__); \
    //         std::cout<<std::string(msg_buf)<<std::endl; \
    //     }while(0)


    /** \def YLIB_STR
        \brief Convert __S to string.
    */
    #define YLIB_STR(__S) YLIB_STR_CONVERT(__S)

    /** \def YLIB_STR_CONVERT
        \brief Convert __S to string.
    */
    #define YLIB_STR_CONVERT(__S) #__S
}




#endif //__YLIB_CORE_YLIBDEFS_HPP__