/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:52
 * @LastEditors  : Sky
 * @LastEditTime : 2020-01-14 16:19:04
 * @Description: 
 */

#ifndef _YLIB_CORE_YLOG_HPP_
#define _YLIB_CORE_YLOG_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32 || _WIN64

#include <windows.h>

#elif __linux__ || __linux

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
}
#endif//__cplusplus


#elif __unix__ || __unix

#endif //__unix__ || __unix


#include "ycommon.hpp"

namespace log4cpp{

    class Category;
};


namespace yLib{

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32 || _WIN64

    #define LOG_TAIL std::string("<<  FileName=") + std::string(__FILE__)+ \
        std::string("  LineNum=") + std::to_string(__LINE__)+ \
        std::string("  FuncName=") + std::string(__FUNCDNAME__)    

#elif __linux__ || __linux
    // Those contain the signature of the function as well as its bare name
    //__FUNCDNAME__  vs
    //__PRETTY_FUNCTION__ gcc

    #define LOG_TAIL std::string("<<  FileName=") + std::string(__FILE__)+ \
        std::string("  LineNum=") + std::to_string(__LINE__)+ \
        std::string("  FuncName=") + std::string(__PRETTY_FUNCTION__)       

#elif __unix__ || __unix

    #endif //__unix__ || __unix


    #define MSG_BUF_SIZE 4096 //4k ,linux-func-stack max size is 8MB
    //debug < info < warn < error

    //deprecated the follow macro  ---------  start --------------
    //For new codes,you should do not use the fllow macro.
    #define ENABLE_DEBUG_LOG_LEVEL 0x01
    #define ENABLE_INFO_LOG_LEVEL 0x02
    #define ENABLE_WARN_LOG_LEVEL 0x04
    #define ENABLE_ERROR_LOG_LEVEL 0x08
    #define ENABLE_ALL_LOG_LEVEL (ENABLE_DEBUG_LOG_LEVEL | ENABLE_INFO_LOG_LEVEL | ENABLE_WARN_LOG_LEVEL | ENABLE_ERROR_LOG_LEVEL)
    #define DISABLE_ALL_LOG_LEVEL 0x00
    //deprecated the  macro  ---------         end --------------



    typedef enum __em_ylog_level__:uint16_t{

        _DISABLE_ALL_LOG_LEVEL_ = 0x0000,

        _ENABLE_DEBUG_LOG_LEVEL_ = 0x0001,
        _DISABLE_DEBUG_LOG_LEVEL_ = (uint16_t)~_ENABLE_DEBUG_LOG_LEVEL_,

        _ENABLE_INFO_LOG_LEVEL_ = 0x0002,
        _DISABLE_INFO_LOG_LEVEL_ = (uint16_t)~_ENABLE_INFO_LOG_LEVEL_,

        _ENABLE_WARN_LOG_LEVEL_ = 0x0004,
        _DISABLE_WARN_LOG_LEVEL_ = (uint16_t)~_ENABLE_WARN_LOG_LEVEL_,

        _ENABLE_ERROR_LOG_LEVEL_ = 0x0008,
        _DISABLE_ERROR_LOG_LEVEL_ = (uint16_t)~_ENABLE_ERROR_LOG_LEVEL_,

        _ENABLE_ALL_LOG_LEVEL_ = 0xFFFF,
    } yLogLevel;

    /*
        idx------>
    */

    typedef struct __st_sub_category_property__
    {
        /* data */
        log4cpp::Category * _ptr_sub_category = nullptr;
        uint16_t _c_sub_log4cpp_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;
        uint16_t _c_sub_ylog_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;

    } SubCategoryProperty;
    
    
    typedef std::unordered_map<std::string, SubCategoryProperty> TypeSubCategoryMap;

    //yLog support thread-safety,defaultly.
    class __yLib_EXPORT__ yLog MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
            
            yLog(yLog & log) = delete;
            yLog & operator=(yLog & log) = delete;
            yLog(yLog && log) = delete;
            yLog && operator=(yLog && log) = delete;
 

			static void SetLog4cpp(bool enable_log4cpp = false, std::string log_path = "log4cplus.properties");	
            static void SetLog4cppSubCategory(std::string category_name);

            //If you want to enable this feature,system must define _POSIX_SHARED_MEMORY_OBJECTS(getconf -a)
            static void SetProcessSafetyFeature(bool enable_feature);

            static void SetLog4cppLogLevel(uint16_t log_level);
            static void SetLog4cppLogLevel(std::string &category_name, uint16_t log_level);
            
            static void SetyLogLogLevel(uint16_t log_level);
            static void SetyLogLogLevel(std::string &category_name, uint16_t log_level);

            static void D(const std::string fmt , ...);
            static void W(const std::string fmt , ...);
            static void I(const std::string fmt , ...);
            static void E(const std::string fmt , ...);

            static void D(std::string &category_name, const std::string fmt , ...);
            static void W(std::string &category_name, const std::string fmt , ...);
            static void I(std::string &category_name, const std::string fmt , ...);
            static void E(std::string &category_name, const std::string fmt , ...);

            static void D(const char * fmt , ...);
            static void W(const char * fmt , ...);
            static void I(const char * fmt , ...);
            static void E(const char * fmt , ...);
            
            static void D(std::string &category_name, const char * fmt , ...);
            static void W(std::string &category_name, const char * fmt , ...);
            static void I(std::string &category_name, const char * fmt , ...);
            static void E(std::string &category_name, const char * fmt , ...);
        protected:
        yLog() noexcept; // yLog is a static-class, it can not instance
        ~yLog() noexcept;
        private:

        static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list);
        static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list, std::string & category_name);
        static char _c_ptr_msg_buf[MSG_BUF_SIZE];

        static log4cpp::Category * _ptr_log4_category_root;
        static TypeSubCategoryMap _log4cpp_sub_category_map;

        static bool _b_enable_log4cpp;
        static bool _b_enable_feature_ps;
        
        static uint16_t _c_log4cpp_log_level;
        static uint16_t _c_ylog_log_level;



#ifdef _WIN32 || _WIN64

        static HANDLE _thread_mutex_handle;
        static bool _thread_mutex_is_init;
        static void _init_thread_mutex(void);

#elif __linux__ || __linux

        static pthread_mutex_t _thread_mutex;
        static pthread_mutex_t _process_mutex;

#elif __unix__ || __unix

#endif //__unix__ || __unix
    };
}



#endif //_YLOG_H_
