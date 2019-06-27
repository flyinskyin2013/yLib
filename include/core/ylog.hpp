#ifndef _YLOG_H_
#define _YLOG_H_

#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
}
#endif //__cplusplus


#include "ycommon.hpp"

namespace log4cpp{

    class Category;
};


namespace yLib{

    // Those contain the signature of the function as well as its bare name
    //__FUNCDNAME__  vs
    //__PRETTY_FUNCTION__ gcc

    #define LOG_TAIL std::string("<<  FileName=") + std::string(__FILE__)+ \
		std::string("  LineNum=") + std::to_string(__LINE__)+ \
		std::string("  FuncName=") + std::string(__PRETTY_FUNCTION__)

    #define MSG_BUF_SIZE 4096 //4k ,linux-func-stack max size is 8MB
    //debug < info < warn < error
    #define ENABLE_DEBUG_LOG_LEVEL 0x01
    #define ENABLE_INFO_LOG_LEVEL 0x02
    #define ENABLE_WARN_LOG_LEVEL 0x04
    #define ENABLE_ERROR_LOG_LEVEL 0x08
    #define ENABLE_ALL_LOG_LEVEL (ENABLE_DEBUG_LOG_LEVEL | ENABLE_INFO_LOG_LEVEL | ENABLE_WARN_LOG_LEVEL | ENABLE_ERROR_LOG_LEVEL)
    #define DISABLE_ALL_LOG_LEVEL 0x00
    //yLog support thread-safety,defaultly.
    class yLog{

        public:
            yLog();
            ~yLog();
            //
            //If you want to enable this feature,system must define _POSIX_SHARED_MEMORY_OBJECTS(getconf -a)
			static void SetLog4cpp(bool enable_log4cpp = false, std::string log_path = "log4cplus.properties");	
            static void SetProcessSafetyFeature(bool enable_feature);
            static void SetLog4cppLogLevel(char log_level);
            static void SetyLogLogLevel(char log_level);


            static void D(const char * fmt , ...);
            static void W(const char * fmt , ...);
            static void I(const char * fmt , ...);
            static void E(const char * fmt , ...);
        protected:
        private:
        static void _ylog_log_impl(char log_type, const char * fmt, va_list arg_list);
        static char m_ptr_msg_buf[MSG_BUF_SIZE];
        //static bool m_b_is_class_access;
        static pthread_mutex_t m_mutex;
        static pthread_mutex_t m_process_mutex;
        static log4cpp::Category * root;
        static bool m_enable_log4cpp;
        static bool m_enable_feature_ps;
        static char g_log4cpp_log_level;
        static char g_ylog_log_level;
    };
}



#endif //_YLOG_H_
