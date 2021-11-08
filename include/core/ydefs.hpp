/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 10:44:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-12 12:02:07
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

        #define __YLIB_EXPORT_DECLSPEC__ __declspec(dllexport)
        #ifdef BUILD_YLIB_WITH_EXPORT
            #define __YLIB_CLASS_DECLSPEC__ __declspec(dllexport)
        #else
            #define __YLIB_CLASS_DECLSPEC__ __declspec(dllimport)
        #endif //BUILD_YLIB_WITH_EXPORT
        
        #define __YLIB_TEAMPLATE_CLASS_EXPORT__ __declspec(dllexport)
        #define __YLIB_TEAMPLATE_FUNC_EXPORT__ __declspec(dllexport)

        #define __YLIB_IMPORT__ __declspec(dllimport)

        #define __YLIB_DEPRECATED_ATTRIBUTE__ __declspec(deprecated)
        #define __YLIB_DEPRECATED_ATTRIBUTE_WITH_MSG__(msg) __declspec(deprecated(msg))
        
    #elif __linux__ || __linux

        #define __YLIB_EXPORT_DECLSPEC__
        #define __YLIB_CLASS_DECLSPEC__
        #define __YLIB_IMPORT__
        #define __YLIB_TEAMPLATE_CLASS_EXPORT__
        #define __YLIB_TEAMPLATE_FUNC_EXPORT__

        #define __YLIB_DEPRECATED_ATTRIBUTE__ __attribute__((deprecated))
        #define __YLIB_DEPRECATED_ATTRIBUTE_WITH_MSG__(msg) __attribute__((deprecated))
    #elif __unix__ || __unix

    #endif //__unix__ || __unix

#define YLIB_STD_CHAR char
#define CONVERT_CHAR_TO_YLIB_STD_CHAR(chr) \
    chr

#define YLIB_STD_STRING std::string
#define CONVERT_STR_TO_YLIB_STD_STRING(str) \
    #str

#ifdef _WIN32

    #ifdef UNICODE

        #define YLIB_STD_CHAR 
        #undef YLIB_STD_STRING
        #undef CONVERT_STR_TO_YLIB_STD_STRING

        #define YLIB_STD_CHAR wchar_t
        #define CONVERT_CHAR_TO_YLIB_STD_CHAR(chr) \
            Lchr

        #define YLIB_STD_STRING std::wstring
        #define CONVERT_STR_TO_YLIB_STD_STRING(str) \
            L#str
    #endif //UNICODE

#endif //_WIN32

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


    // #define YLIB_VERSION_MAJOR 0
    // #define YLIB_VERSION_MINOR 0
    // #define YLIB_VERSION_PATCH 7

    // #define YLIB_VERSION YLIB_STR(YLIB_VERSION_MAJOR) "." YLIB_STR(YLIB_VERSION_MINOR) "." YLIB_STR(YLIB_VERSION_PATCH)


    // YLIB_ENABLE_UNIT_TEST control some specail unit-test
    // YLIB_CODECOVERAGE_SKIP_CODE control code-coverage skip some code
    }




#endif //__YLIB_CORE_YLIBDEFS_HPP__