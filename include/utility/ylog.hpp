/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:52
 * @LastEditors: Sky
 * @LastEditTime: 2021-04-07 16:24:50
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YLOG_HPP__
#define __YLIB_UTILITY_YLOG_HPP__

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32

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


#include "core/yobject.hpp"



namespace log4cpp{

    class Category;
}


namespace yLib{

#ifdef _WIN32

    #define LOG_FILE_AND_FUNCTION_INFO_STRING std::string("<<  FileName=") + std::string(__FILE__)+ \
        std::string("  LineNum=") + std::to_string(__LINE__)+ \
        std::string("  FuncName=") + std::string(__FUNCDNAME__)    

#elif __linux__ || __linux
    // Those contain the signature of the function as well as its bare name
    //__FUNCDNAME__  vs
    //__PRETTY_FUNCTION__ gcc

    /** \def LOG_FILE_AND_FUNCTION_INFO_STRING
        \brief This macro will record file and func infos of current using postion.
    */
    #define LOG_FILE_AND_FUNCTION_INFO_STRING std::string("<<  FileName=") + std::string(__FILE__)+ \
        std::string("  LineNum=") + std::to_string(__LINE__)+ \
        std::string("  FuncName=") + std::string(__PRETTY_FUNCTION__)       

#elif __unix__ || __unix

    //TODO
#endif //__unix__ || __unix


    /** \def MSG_BUF_SIZE
        \brief The max string buffer of yLog.
    */
    #define MSG_BUF_SIZE 4096 //4k ,linux-func-stack max size is 8MB

    //parity: debug < info < warn < error
    //deprecated the follow macro  ---------  start --------------
    //For new codes,you should do not use the fllow macro.
    #define ENABLE_DEBUG_LOG_LEVEL 0x01
    #define ENABLE_INFO_LOG_LEVEL 0x02
    #define ENABLE_WARN_LOG_LEVEL 0x04
    #define ENABLE_ERROR_LOG_LEVEL 0x08
    #define ENABLE_ALL_LOG_LEVEL (ENABLE_DEBUG_LOG_LEVEL | ENABLE_INFO_LOG_LEVEL | ENABLE_WARN_LOG_LEVEL | ENABLE_ERROR_LOG_LEVEL)
    #define DISABLE_ALL_LOG_LEVEL 0x00
    //deprecated the  macro  ---------         end --------------


    /** \enum yLogLevel
        \brief the level of ylog.
    */
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

    /** \struct SubCategoryProperty
        \brief the sub category property.
    */
    typedef struct __st_sub_category_property__
    {
        /* data */
        log4cpp::Category * _ptr_sub_category = nullptr;
        uint16_t _c_sub_log4cpp_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;
        uint16_t _c_sub_ylog_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;

    } SubCategoryProperty;
    
    /** \typedef SubCategoryProperty
        \brief typedef std::unordered_map<std::string, SubCategoryProperty> to TypeSubCategoryMap
    */
    typedef std::unordered_map<std::string, SubCategoryProperty> TypeSubCategoryMap;

    /** \struct EnableyLogFileParam
        \brief the param of enabling log file.
    */
    typedef struct __st_enable_ylog_file_param__{

        std::string cfg_file_path;

        /**
         *  @var    other_category_name_vec
         *  @brief If this vector is empty, the yLog only have root-category. 
         *  If we want to save log to different files, we can create other categories. 
        */  
        std::vector<std::string> other_category_name_vec;
    } EnableyLogFileParam;

    //yLog support thread-safety,defaultly.
    /**
     *  @class yLog
     *  @brief This is log-class based on log4cpp in yLib.
     */
    class __YLIB_EXPORT__ yLog MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
            /**
             *  @fn      yLog(const yLog & log) = delete
             *  @brief   Copy constructor
             *  @param   log exsited obj.
             *  @warning This op is deleted.
             *  @return 
             */            
            yLog(const yLog & log) = delete;

            /**
             *  @fn      yLog & operator=(const yLog & log) = delete
             *  @brief   Assignment constructor
             *  @param   log exsited obj.
             *  @warning This op is deleted.
             *  @return The reference of yLog's object.
             */
            yLog & operator=(const yLog & log) = delete;

            /**
             *  @fn      yLog(const yLog && log) = delete
             *  @brief   Move constructor
             *  @param   log exsited obj.
             *  @warning This op is deleted.
             *  @return 
             */
            yLog(const yLog && log) = delete;

            /**
             *  @fn      yLog && operator=(const yLog && log) = delete
             *  @brief   Assignment constructor
             *  @param   log exsited obj.
             *  @warning This op is deleted.
             *  @return The reference of yLog's object.
             */
            yLog && operator=(const yLog && log) = delete;
  
            /**
             *  @fn      static void EnableyLogFile(bool is_enable, const EnableyLogFileParam & param);
             *  @brief   Enable or disable yLog to file.
             *  @param   is_enable if enable yLog to file.
             *  @param   param the param of yLog to file.
             *  @warning None
             *  @return  void
             *  @exception None
             */    
            static void EnableyLogFile(bool is_enable, const EnableyLogFileParam & param);


            /**
             *  @fn      static void SetLog4cpp(bool enable_log4cpp = false, std::string log_path = "log4cplus.properties")
             *  @brief   Enable or disable log4cpp
             *  @param   enable_log4cpp if enable log4cpp
             *  @param   log_path log4cpp's cfg-file
             *  @warning 
             *  @return 
             *  @exception throw a anonymous exception if the log4cpp's cfg-file is invalid.
             */    
			static void SetLog4cpp(bool enable_log4cpp = false, std::string log_path = "log4cplus.properties");

            /**
             *  @fn      static void SetLog4cppSubCategory(std::string category_name)
             *  @brief   Create a sub-category for log4cpp.
             *  @param   category_name the sub-category name
             *  @warning 
             *  @return 
             *  @exception 
             */    	
            static void SetLog4cppSubCategory(std::string category_name);

            //If you want to enable this feature,system must define _POSIX_SHARED_MEMORY_OBJECTS(getconf -a)
            /**
             *  @fn      static void SetProcessSafetyFeature(bool enable_feature)
             *  @brief   If you want to enable this feature,system must define _POSIX_SHARED_MEMORY_OBJECTS(getconf -a)
             *  @param   enable_feature enable or disable thread-satety.
             *  @warning 
             *  @return 
             *  @exception 
             */    	
            static void SetProcessSafetyFeature(bool enable_feature);

            /**
             *  @fn      static void SetLog4cppLogLevel(uint16_t log_level)
             *  @brief   set the log4cpp level
             *  @param   log_level enable log-level.
             *  @warning 
             *  @return 
             *  @exception 
             */    	
            static void SetLog4cppLogLevel(uint16_t log_level);

            /**
             *  @fn      static void SetLog4cppLogLevel(std::string &category_name, uint16_t log_level)
             *  @brief   set the log4cpp level(parity: debug < info < warn < error)
             *  @param   category_name the sub-category name
             *  @param   log_level enable log-level.
             *  @warning 
             *  @return 
             *  @exception 
             */    	
            static void SetLog4cppLogLevel(std::string &category_name, uint16_t log_level);
            

            /**
             *  @fn      static void SetyLogLogLevel(uint16_t log_level)
             *  @brief   set the ylog level
             *  @param   log_level enable log-level.
             *  @warning 
             *  @return 
             *  @exception 
             */    	
            static void SetyLogLogLevel(uint16_t log_level);

            /**
             *  @fn      static void SetyLogLogLevel(std::string &category_name, uint16_t log_level)
             *  @brief   set the ylog level(parity: debug < info < warn < error)
             *  @param   category_name the sub-category name
             *  @param   log_level enable log-level.
             *  @warning 
             *  @return 
             *  @exception 
             */               
            static void SetyLogLogLevel(std::string &category_name, uint16_t log_level);
            
            /**
             *  @fn      static void D(const std::string fmt , ...)
             *  @brief   the debug log
             *  @param   fmt the format of info-string.
             */    	            
            static void D(const std::string fmt , ...);

            /**
             *  @fn      static void W(const std::string fmt , ...)
             *  @brief   the warn log
             *  @param   fmt the format of info-string.
             */    
            static void W(const std::string fmt , ...);

            /**
             *  @fn      static void I(const std::string fmt , ...)
             *  @brief   the info log
             *  @param   fmt the format of info-string.
             */    
            static void I(const std::string fmt , ...);

            /**
             *  @fn      static void E(const std::string fmt , ...)
             *  @brief   the error log
             *  @param   fmt the format of info-string.
             */    
            static void E(const std::string fmt , ...);
            
            /**
             *  @fn      static void D(std::string &category_name, const std::string fmt , ...)
             *  @brief   the debug log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */    
            static void D(std::string &category_name, const std::string fmt , ...);

            /**
             *  @fn      static void W(std::string &category_name, const std::string fmt , ...)
             *  @brief   the warn log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void W(std::string &category_name, const std::string fmt , ...);

            /**
             *  @fn      static void I(std::string &category_name, const std::string fmt , ...)
             *  @brief   the info log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void I(std::string &category_name, const std::string fmt , ...);

            /**
             *  @fn      static void E(std::string &category_name, const std::string fmt , ...)
             *  @brief   the error log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void E(std::string &category_name, const std::string fmt , ...);

            /**
             *  @fn      static void D(std::string &category_name, const std::string fmt , ...)
             *  @brief   the debug log
             *  @param   fmt the format of info-string.
             */ 
            static void D(const char * fmt , ...);

            /**
             *  @fn      static void W(std::string &category_name, const std::string fmt , ...)
             *  @brief   the warn log
             *  @param   fmt the format of info-string.
             */ 
            static void W(const char * fmt , ...);

            /**
             *  @fn      static void I(std::string &category_name, const std::string fmt , ...)
             *  @brief   the info log
             *  @param   fmt the format of info-string.
             */ 
            static void I(const char * fmt , ...);

            /**
             *  @fn      static void E(std::string &category_name, const std::string fmt , ...)
             *  @brief   the error log
             *  @param   fmt the format of info-string.
             */ 
            static void E(const char * fmt , ...);



            /**
             *  @fn      static void D(std::string &category_name, const char * fmt , ...)
             *  @brief   the debug log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */               
            static void D(std::string &category_name, const char * fmt , ...);

            /**
             *  @fn      static void W(std::string &category_name, const char * fmt , ...)
             *  @brief   the warn log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void W(std::string &category_name, const char * fmt , ...);

            /**
             *  @fn      static void I(std::string &category_name, const char * fmt , ...)
             *  @brief   the info log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void I(std::string &category_name, const char * fmt , ...);

            /**
             *  @fn      static void E(std::string &category_name, const char * fmt , ...)
             *  @brief   the error log
             *  @param   category_name the sub-category name.
             *  @param   fmt the format of info-string.
             */   
            static void E(std::string &category_name, const char * fmt , ...);
        protected:
        /**
         *  @fn    yLog()
         *  @brief Default constructor
         *  @warning yLog is a static-class, we can not instance it
         */
        yLog() noexcept; // yLog is a static-class, we can not instance it

        
        /**
         *  @fn    ~yObject()
         *  @brief Default destructor
         */
        ~yLog() noexcept;
        private:


        /**
         *  @fn      static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list)
         *  @brief   the basic-implement of D I W E
         *  @param   log_type the sub-category name.
         *  @param   fmt the format of info-string.
         *  @param   arg_list the input arg-lists
         */   
        static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list);

        /**
         *  @fn      static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list, std::string & category_name)
         *  @brief   the basic-implement of D I W E
         *  @param   log_type the sub-category name.
         *  @param   fmt the format of info-string.
         *  @param   arg_list the input arg-lists
         *  @param   category_name the sub-category name
         */   
        static void _ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list, std::string & category_name);

        /**
         *  @var    _c_ptr_msg_buf
         *  @brief the buffer of the log-string
         */        
        static char _c_ptr_msg_buf[MSG_BUF_SIZE];

        /**
         *  @var    _ptr_log4_category_root
         *  @brief the handle of log4cpp::Category
         */        
        static log4cpp::Category * _ptr_log4_category_root;

        /**
         *  @var    _log4cpp_sub_category_map
         *  @brief the sub-category map
         */       
        static TypeSubCategoryMap _log4cpp_sub_category_map;


        /**
         *  @var    _b_enable_log4cpp
         *  @brief if enable log4cpp
         */  
        static bool _b_enable_log4cpp;

        
        /**
         *  @var    _b_enable_feature_ps
         *  @brief if enable process-safety.
         */  
        static bool _b_enable_feature_ps;
        

        /**
         *  @var    _c_log4cpp_log_level
         *  @brief current log4cpp log-level.
         */  
        static uint16_t _c_log4cpp_log_level;
        
        /**
         *  @var    _c_ylog_log_level
         *  @brief current ylog log-level.
         */  
        static uint16_t _c_ylog_log_level;



#ifdef _WIN32

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



#endif //__YLIB_UTILITY_YLOG_HPP__
