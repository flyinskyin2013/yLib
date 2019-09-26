/*
 * @Author: Sky
 * @Date: 2018-10-23 11:07:58
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-23 15:56:51
 * @Description: 
 */

#ifndef _YLIB_CORE_YCOMMON_H_
#define _YLIB_CORE_YCOMMON_H_

#include <string>


#include "yobject.hpp"


namespace yLib{


//define some useful macroes ---------------------------- start
//for yObject
#define MACRO_PUBLIC_INHERIT_YOBJECT :public yLib::yObject
#define MACRO_INIT_YOBJECT_PROPERTY(object_name) \
    :yLib::yObject::yObject(std::string(#object_name))




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

    class yCommon MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::string _ylib_version;
    public:
        yCommon(/* args */) noexcept MACRO_INIT_YOBJECT_PROPERTY(yCommon){}
        ~yCommon()noexcept{}
        yCommon(yCommon & common) = delete;
        yCommon & operator=(yCommon & common) = delete;

        std::string  yCommon_Get_yLib_Version() const noexcept;

        
    };

    
 }

#endif //_YLIB_CORE_YCOMMON_H_