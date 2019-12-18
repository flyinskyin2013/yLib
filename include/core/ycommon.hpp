/*
 * @Author: Sky
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Sky
 * @LastEditTime: 2019-12-10 10:15:52
 * @Description: 
 */

#ifndef _YLIB_CORE_YCOMMON_HPP_
#define _YLIB_CORE_YCOMMON_HPP_

#include <string>


#include "yobject.hpp"


namespace yLib{
    
#ifdef _WIN32

    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32 || _WIN64
    #define __yLib_EXPORT__ __declspec(dllexport)
    #define __yLib_IMPORT__ __declspec(dllimport)

#elif __linux__ || __linux
    #define __yLib_EXPORT__
    #define __yLib_IMPORT__
#elif __unix__ || __unix

#endif //__unix__ || __unix


//define some useful macroes ---------------------------- start
//for yObject
#define MACRO_PUBLIC_INHERIT_YOBJECT :public yLib::yObject

//call the yObject
#define MACRO_INIT_YOBJECT_PROPERTY(object_name) \
    :yObject(std::string(#object_name))

#define __MACRO_YLIB_DEPRECATED__ __attribute__((deprecated))


//define some useful macroes ----------------------------   end


// //c99 for variadic macros
// #define yLogD(...) \
//     do{ \
//         char msg_buf[1024]; \
//         sprintf(msg_buf, __VA_ARGS__); \
//         std::cout<<std::string("LogDebug:>")+std::string(msg_buf)<<std::endl; \
//     }while(0)
// #define yLogW(...) \
//     do{ \
//         char msg_buf[1024]; \
//         sprintf(msg_buf, __VA_ARGS__); \
//         std::cout<<std::string("LogWarn:>")+std::string(msg_buf)<<std::endl; \
//     }while(0)
// #define yLogE(...) \
//     do{ \
//         char msg_buf[1024]; \
//         sprintf(msg_buf, __VA_ARGS__); \
//         std::cout<<std::string("LogError:>")+std::string(msg_buf)<<std::endl; \
//     }while(0)
// #define yLogI(...) \
//     do{ \
//         char msg_buf[1024]; \
//         sprintf(msg_buf, __VA_ARGS__); \
//         std::cout<<std::string("LogInfo:>")+std::string(msg_buf)<<std::endl; \
//     }while(0)
    
    #define YLIB_VERSION_MAJOR 1
    #define YLIB_VERSION_MINOR0 0
    #define YLIB_VERSION_MINOR1 0

    class __yLib_EXPORT__ yCommon MACRO_PUBLIC_INHERIT_YOBJECT
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

        std::string  GetyLibVersion() const noexcept;
    };

    
 }

#endif //_YLIB_CORE_YCOMMON_H_