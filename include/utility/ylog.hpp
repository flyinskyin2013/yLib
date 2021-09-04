/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:52
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 21:48:14
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YLOG_HPP__
#define __YLIB_UTILITY_YLOG_HPP__

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include <fstream>



// c header
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

    #include <stdarg.h>
    #include <string.h>
    #include <stddef.h>
    #include <stdint.h>

    #ifdef _WIN32

        #include <windows.h>

    #elif __linux__ || __linux

        #include <unistd.h>
        #include <pthread.h>

    #elif __unix__ || __unix
    #endif //__unix__ || __unix

#ifdef __cplusplus
}
#endif//__cplusplus

// 
#include "core/yobject.hpp"


namespace yLib{

    // Those contain the signature of the function as well as its bare name
    //__FUNCDNAME__  vs
    //__PRETTY_FUNCTION__ gcc

    // __LINE__
    // __FILE__




    //parity: all < trace < debug < info < warn < error < fatal

    typedef enum __em_ylog_severity__:uint16_t{

        LOG_ALL = 0,
        LOG_TRACE,
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL
    } yLogSeverity;


    /** \enum yLogLevel
        \brief the level of ylog.
    */
    typedef enum __em_ylog_level__:uint16_t{

        DISABLE_ALL_LOG_LEVEL = 0x0000,

        ENABLE_DEBUG_LOG_LEVEL = 0x0001,
        DISABLE_DEBUG_LOG_LEVEL = (uint16_t)~ENABLE_DEBUG_LOG_LEVEL,

        ENABLE_INFO_LOG_LEVEL = 0x0002,
        DISABLE_INFO_LOG_LEVEL = (uint16_t)~ENABLE_INFO_LOG_LEVEL,

        ENABLE_WARN_LOG_LEVEL = 0x0004,
        DISABLE_WARN_LOG_LEVEL = (uint16_t)~ENABLE_WARN_LOG_LEVEL,

        ENABLE_ERROR_LOG_LEVEL = 0x0008,
        DISABLE_ERROR_LOG_LEVEL = (uint16_t)~ENABLE_ERROR_LOG_LEVEL,

        ENABLE_ALL_LOG_LEVEL = 0xFFFF,
    } yLogLevel;
    /*
        idx------>
    */


    /** \struct EnableyLogFileParam
        \brief the param of enabling log file.
    */
    typedef struct __YLIB_EXPORT__ __st_enable_ylog_file_param__{

        std::string file_base_name = "";
        std::string file_dir = "";
        /**
         *  @var    multi_log_file
         *  @brief if true store multi-log-file by tag. default value is false. 
         */  
        bool multi_log_file = false;

        bool flush_every_times = true;

        // default 20MB 1024*1024*20 = 20971520
        uint64_t log_file_max_size = 20971520;

        uint32_t log_file_max_backup_num = 2;

        // (ms)
        uint32_t flush_timeout = 200;
    } EnableyLogFileParam;

    struct __YLIB_EXPORT__ yLogTagProperty{
 
        uint16_t log_level = ENABLE_ALL_LOG_LEVEL;
        static EnableyLogFileParam file_param;
        bool is_log_to_file = false;
        bool is_log_to_stdio = true;
    };

    class yLogMessageVoidify {
    public:
    yLogMessageVoidify() { }

    // for (exp)?(void)0:yLogMessageVoidify()&(other-exp)
    void operator&(std::ostream&) { }
    };

    #define YLIB_YLOGMESSAGE_MAX_LEN  1024*8
    class __YLIB_EXPORT__ yLogMessage:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
        public:
        const static int max_message_len;//8k

        public:
        class yLogStreamBuf:public std::streambuf{
            public:
            yLogStreamBuf(char * buf, size_t len){
                
                // set new stream buf, the len of buf must be greater than 1.
                setp(buf, buf + len - 1);
            }
            // overrite
            int overflow (int c = EOF) override{

                is_overflow = true;
                return 0;
            }

            char *buf_base_ptr(void){return pbase();}
            char *buf_cur_ptr(void){return pptr();}
            char *buf_end_ptr(void){return epptr();}

            bool is_overflow = false;
        };
        class yLogStream:public std::ostream{

            public:
            yLogStream(char * buf, size_t len)
            :std::ostream(NULL), stream_buf(buf, len)
            {
                rdbuf(&stream_buf);
            }
            
            
            yLogStreamBuf stream_buf;
        };
        struct yLogMessageData{
            yLogMessageData():log_stream(msg_buf, YLIB_YLOGMESSAGE_MAX_LEN + 1){}
            char msg_buf[YLIB_YLOGMESSAGE_MAX_LEN + 1];
            yLogStream log_stream;
            yLib::yLogSeverity log_severity;
            int file_line;
            std::string file_name;
            std::string log_tag;
            struct ::timespec time_spec;
            struct ::tm tm_time;      
            int thread_id;

            int log_prefix_len = 0;
        };

        std::ostream & stream(void) noexcept {return msg_data->log_stream;}
        public:
        yLogMessage(const yLogMessage &msg) = delete;
        yLogMessage(const yLogMessage &&msg) = delete;
        yLogMessage& operator=(const yLogMessage &msg) = delete;
        yLogMessage& operator=(const yLogMessage &&msg) = delete;
        yLogMessage(const std::string & log_tag, yLib::yLogSeverity log_severity, const std::string &file_name, int file_line, bool newline = true) noexcept;
        ~yLogMessage() noexcept;
        
        private:
        std::unique_ptr<yLogMessageData> msg_data;
        bool is_add_newline = false;
    };



    class yLogFile{

        public:
        void write(const char* message, size_t message_len, bool flush, const std::string &tag);

        yLogFile(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map);
        yLogFile() = delete;
        ~yLogFile();
        private:
        std::atomic<bool> write_thread_is_continue{false};
        std::atomic<bool> is_flush{false};
        std::atomic<bool> write_thread_is_ready{false};

        std::unique_ptr<std::thread> write_thread_ptr = nullptr;

        void write_thread_func(void);

        std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map = nullptr;

        // pair<tag, msg>
        std::queue<std::pair<std::string, std::string>> file_cache_queue;
        std::mutex file_cache_queue_mtx;


        bool create_log_file(void);
        
        std::unordered_map<std::string, std::pair<std::string,std::shared_ptr<std::ofstream>>> log_file_handle_map;

        struct ::timespec old_time_spec;

        void flush_log_file(void);
    };



    class yLogDestination{

        public:
        static yLogDestination* CreateInstance(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map) noexcept;
        void LogToStdIO(yLib::yLogSeverity severity, const char* message, size_t message_len) noexcept;
        void LogToFile(const char* message, size_t message_len, bool flush, const std::string &tag) noexcept;   

        yLogFile log_file;     

        protected:
        yLogDestination(std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map);

        std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> tag_prop_map = nullptr;
        static yLib::yLogDestination* instance;
    };

    //yLog support thread-safety,defaultly.
    /**
     *  @class yLog
     *  @brief This is log-class in yLib.
     *  Ref:google glog/log4cpp
     *      
     */
    class __YLIB_EXPORT__ yLog MACRO_PUBLIC_INHERIT_YOBJECT{

        public:        
            yLog() = delete;
            ~yLog() = delete;
            yLog(const yLog & log) = delete;
            yLog & operator=(const yLog & log) = delete;
            yLog(const yLog && log) = delete;
            yLog & operator=(const yLog && log) = delete;

            /**
             *  @fn      static void Init(const yLogTagProperty & tag_prop, const std::string &tag = "") noexcept
             *  @brief   Init log tag param. If you want to set log-file-prop, must call it before any yLib::yLog's memeber and macro LOGXXX.
             *  @param   tag_prop the tag param.
             *  @param   tag log tag
             * 
             */
            static void Init(const yLogTagProperty & tag_prop, const std::string &tag = "") noexcept;

            /**
             *  @fn      static void D(const std::string fmt , ...)
             *  @brief   the debug log
             *  @param   fmt the format of info-string.
             */    	            
            static void D(const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void D(const std::string &tag, const std::string &fmt , ...) noexcept
             *  @brief   the debug log
             *  @param   tag the sub-category name.
             *  @param   fmt the format of info-string.
             *  Note: remove const of tag, D("%s", "123") will not call D(const std::string &fmt , ...).
             */    
            static void D(const std::string &tag, const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void D(const char *fmt , const char * str) noexcept
             *  @brief   the error log
             *  @param   str the str of %s.
             *  @param   fmt the format of info-string.
             *  Note:
             *  This overrite-func only can be used when D("", "")
             */   
            static void D(const char *fmt , const char * str) noexcept;
            
            /**
             *  @fn      static void W(const std::string fmt , ...)
             *  @brief   the warn log
             *  @param   fmt the format of info-string.
             */    
            static void W(const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void W(const std::string &tag, const std::string &fmt , ...) noexcept
             *  @brief   the warn log
             *  @param   tag the sub-category name.
             *  @param   fmt the format of info-string.
             * Note: remove const of tag, D("%s", "123") will not call D(const std::string &fmt , ...).
             */   
            static void W(const std::string &tag, const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void W(const char *fmt , const char * str) noexcept
             *  @brief   the error log
             *  @param   str the str of %s.
             *  @param   fmt the format of info-string.
             *  Note:
             *  This overrite-func only can be used when W("", "")
             */   
            static void W(const char *fmt , const char * str) noexcept;

            /**
             *  @fn      static void I(const std::string fmt , ...)
             *  @brief   the info log
             *  @param   fmt the format of info-string.
             */    
            static void I(const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void I(const std::string &tag, const std::string &fmt , ...) noexcept
             *  @brief   the info log
             *  @param   tag the sub-category name.
             *  @param   fmt the format of info-string.
             * Note: remove const of tag, D("%s", "123") will not call D(const std::string &fmt , ...).
             */   
            static void I(const std::string &tag, const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void I(const char *fmt , const char * str) noexcept
             *  @brief   the error log
             *  @param   str the str of %s.
             *  @param   fmt the format of info-string.
             *  Note:
             *  This overrite-func only can be used when I("", "")
             */   
            static void I(const char *fmt , const char * str) noexcept; 

            /**
             *  @fn      static void E(const std::string fmt , ...)
             *  @brief   the error log
             *  @param   fmt the format of info-string.
             */    
            static void E(const std::string &fmt , ...) noexcept;
            /**
             *  @fn      static void E(const std::string &tag, const std::string &fmt , ...) noexcept
             *  @brief   the error log
             *  @param   tag the sub-category name.
             *  @param   fmt the format of info-string.
             * 
             */   
            static void E(const std::string &tag, const std::string &fmt , ...) noexcept;  

            /**
             *  @fn      static void E(const char *fmt , const char * str) noexcept
             *  @brief   the error log
             *  @param   str the str of %s.
             *  @param   fmt the format of info-string.
             *  Note:
             *  This overrite only can be used, when E("", "")
             */   
            static void E(const char *fmt , const char * str) noexcept; 

        protected:

        friend class yLib::yLogMessage;
        static void Log(const std::string & tag, yLib::yLogSeverity severity, const std::string &msg);
        
        /** \def YLIB_YLOG_MSG_BUF_MAX_SIZE
            \brief The max string buffer of yLog. default 8kb.
        */
        static uint32_t YLIB_YLOG_MSG_BUF_MAX_SIZE;
        private:

        /**
         *  @fn      static void convert_fmt_to_str(uint16_t log_type, const char * fmt, va_list arg_list, std::string & category_name)
         *  @brief   the basic-implement of D I W E
         *  @param   log_type the sub-category name.
         *  @param   fmt the format of info-string.
         *  @param   arg_list the input arg-lists
         *  @param   tag the sub-category name
         *  @param   special_override_func_str this param only used for yLib::yLog::X(const char*, const char*)
         */   
        static void convert_fmt_to_str(yLib::yLogSeverity log_type, const char * fmt, va_list arg_list, const std::string & tag, const char * special_override_func_str = nullptr);
        
        static void ylog_log_impl(yLib::yLogSeverity log_type, const std::string & msg, const std::string &tag = "");

        static bool check_log_level(yLib::yLogSeverity log_type, const std::string &tag);

        static void check_tag(const std::string & tag);

        static std::shared_ptr<std::unordered_map<std::string, yLogTagProperty>> ylog_tag_prop_map;

        static std::unique_ptr<char[]> ylog_msg_buf;

        static std::mutex ylog_msg_buf_mtx;

        static std::unique_ptr<yLogDestination> ylog_destination;
    };

    
    #define LOG_COUNTER_VARNAME_CONCAT(base, line) base ## line
    #define LOG_COUNTER_VARNAME(base, line) LOG_COUNTER_VARNAME_CONCAT(base, line)
    #define LOG_COUNTER_VARNAME_REFERENCE LOG_COUNTER_VARNAME(__ylib_ylog_log_counter, __LINE__)
    #define LOG_COUNTER_MOD_VARNAME_REFERENCE LOG_COUNTER_VARNAME(__ylib_ylog_log_counter_mod, __LINE__)

    #define LOG(tag, severity) \
            yLib::yLogMessage(std::string(#tag), severity, __FILE__, __LINE__).stream()

    #define LOG_IF(tag, condition, severity) \
            !(condition) ? (void) 0 : yLib::yLogMessageVoidify()&LOG(tag, severity) 

    #define LOG_EVERY_N(tag, n, severity) \
            static std::atomic<uint32_t> LOG_COUNTER_VARNAME_REFERENCE(0); \
            static std::atomic<uint32_t> LOG_COUNTER_MOD_VARNAME_REFERENCE(0); \
            LOG_COUNTER_VARNAME_REFERENCE++; \
            if(++LOG_COUNTER_MOD_VARNAME_REFERENCE>(n)) LOG_COUNTER_MOD_VARNAME_REFERENCE-=(n); \
            if(1 == LOG_COUNTER_MOD_VARNAME_REFERENCE) \
                LOG(tag, severity)

    #define LOG_IF_EVERY_N(tag, condition, n, severity) \
            static std::atomic<uint32_t> LOG_COUNTER_VARNAME_REFERENCE(0); \
            static std::atomic<uint32_t> LOG_COUNTER_MOD_VARNAME_REFERENCE(0); \
            LOG_COUNTER_VARNAME_REFERENCE++; \
            if(++LOG_COUNTER_MOD_VARNAME_REFERENCE>(n)) LOG_COUNTER_MOD_VARNAME_REFERENCE-=(n); \
            if ( (condition)&&(1 == LOG_COUNTER_MOD_VARNAME_REFERENCE) ) \
                LOG(tag, severity)
            

    #define LOG_FIRST_N(tag, n, severity) \
            static std::atomic<uint32_t> LOG_COUNTER_VARNAME_REFERENCE(0); \
            if(LOG_COUNTER_VARNAME_REFERENCE <= (n)) LOG_COUNTER_VARNAME_REFERENCE ++; \
            if(LOG_COUNTER_VARNAME_REFERENCE <= (n)) \
                LOG(tag, severity)


            
    // ERROR
    #define LOGE(tag)                                                                    \
        LOG(tag, yLib::yLogSeverity::LOG_ERROR)
    
    #define LOGE_IF(tag, condition)                                                      \
        LOG_IF(tag, condition, yLib::yLogSeverity::LOG_ERROR)
        
    #define LOGE_IF_EVERY_N(tag, condition, n)                                             \
        LOG_IF_EVERY_N(tag, condition, n, yLib::yLogSeverity::LOG_ERROR)

    #define LOGE_EVERY_N(tag, n)                                                            \
        LOG_EVERY_N(tag, n, yLib::yLogSeverity::LOG_ERROR)

    #define LOGE_FIRST_N(tag, n)                                                            \
        LOG_FIRST_N(tag, n, yLib::yLogSeverity::LOG_ERROR)

    // WARN
    #define LOGW(tag)                                                                    \
        LOG(tag, yLib::yLogSeverity::LOG_WARN)
    
    #define LOGW_IF(tag, condition)                                                      \
        LOG_IF(tag, condition, yLib::yLogSeverity::LOG_WARN)
        
    #define LOGW_IF_EVERY_N(tag, condition, n)                                             \
        LOG_IF_EVERY_N(tag, condition, n, yLib::yLogSeverity::LOG_WARN)

    #define LOGW_EVERY_N(tag, n)                                                            \
        LOG_EVERY_N(tag, n, yLib::yLogSeverity::LOG_WARN)

    #define LOGW_FIRST_N(tag, n)                                                            \
        LOG_FIRST_N(tag, n, yLib::yLogSeverity::LOG_WARN)

    // INFO
    #define LOGI(tag)                                                                    \
        LOG(tag, yLib::yLogSeverity::LOG_INFO)
    
    #define LOGI_IF(tag, condition)                                                      \
        LOG_IF(tag, condition, yLib::yLogSeverity::LOG_INFO)
        
    #define LOGI_IF_EVERY_N(tag, condition, n)                                             \
        LOG_IF_EVERY_N(tag, condition, n, yLib::yLogSeverity::LOG_INFO)

    #define LOGI_EVERY_N(tag, n)                                                            \
        LOG_EVERY_N(tag, n, yLib::yLogSeverity::LOG_INFO)

    #define LOGI_FIRST_N(tag, n)                                                            \
        LOG_FIRST_N(tag, n, yLib::yLogSeverity::LOG_INFO)

    // DEBUG
    #define LOGD(tag)                                                                    \
        LOG(tag, yLib::yLogSeverity::LOG_DEBUG)
    
    #define LOGD_IF(tag, condition)                                                      \
        LOG_IF(tag, condition, yLib::yLogSeverity::LOG_DEBUG)
        
    #define LOGD_IF_EVERY_N(tag, condition, n)                                             \
        LOG_IF_EVERY_N(tag, condition, n, yLib::yLogSeverity::LOG_DEBUG)

    #define LOGD_EVERY_N(tag, n)                                                            \
        LOG_EVERY_N(tag, n, yLib::yLogSeverity::LOG_DEBUG)

    #define LOGD_FIRST_N(tag, n)                                                            \
        LOG_FIRST_N(tag, n, yLib::yLogSeverity::LOG_DEBUG)
}



#endif //__YLIB_UTILITY_YLOG_HPP__
