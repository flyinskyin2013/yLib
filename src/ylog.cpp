/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors  : Sky
 * @LastEditTime : 2020-01-14 16:03:24
 * @Description: 
 */

#include "ylog.hpp"

//log4cpp
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

///property
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
//log4cpp

#ifdef _WIN32 || _WIN64

HANDLE yLib::yLog::_thread_mutex_handle = NULL;
bool yLib::yLog::_thread_mutex_is_init = false;

#elif __linux__ || __linux

pthread_mutex_t yLib::yLog::_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t yLib::yLog::_process_mutex;

#elif __unix__ || __unix

#endif //__unix__ || __unix


char  yLib::yLog::_c_ptr_msg_buf[MSG_BUF_SIZE];
//bool yLib::yLog::m_b_is_class_access = true;
log4cpp::Category * yLib::yLog::_ptr_log4_category_root = nullptr;
yLib::TypeSubCategoryMap yLib::yLog::_log4cpp_sub_category_map;

bool yLib::yLog::_b_enable_log4cpp = false;
bool yLib::yLog::_b_enable_feature_ps = false;



uint16_t yLib::yLog::_c_log4cpp_log_level = yLib::yLogLevel::_ENABLE_ALL_LOG_LEVEL_;
uint16_t yLib::yLog::_c_ylog_log_level = yLib::yLogLevel::_ENABLE_ALL_LOG_LEVEL_;

yLib::yLog::yLog() noexcept MACRO_INIT_YOBJECT_PROPERTY(yLog){


}

yLib::yLog::~yLog() noexcept{

    // we can't  call log4cpp::Category::shutdown() here.
    // if ( _b_enable_log4cpp ){
    //     //log4cpp
    //     log4cpp::Category::shutdown();
    //     _b_enable_log4cpp = false;
	//     _ptr_log4_category_root = nullptr;
    // }
}


#ifdef _WIN32 || _WIN64

void yLib::yLog::_init_thread_mutex(void){

    SECURITY_ATTRIBUTES attr_;
    attr_.bInheritHandle = false;
    _thread_mutex_handle = CreateMutex(\
        NULL, \
        false, \
        "yLib::yLog::_thread_mutex_handle" \
    );

    if ( NULL == _thread_mutex_handle ){//then we can not call yLib::yLog::E yLib::yLog::I yLib::yLog::W yLib::yLog::D , they are not ready 

        HLOCAL LocalAddress = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
        std::cout<< " yLib::yLog::_init_thread_mutex():create mutex failed, error info is " <<(char *)LocalAddress<<std::endl;
    }

}

#elif __linux__ || __linux


#elif __unix__ || __unix

#endif //__unix__ || __unix



void yLib::yLog::SetLog4cppLogLevel(uint16_t log_level){

    
    _c_log4cpp_log_level &= log_level;
}

void yLib::yLog::SetLog4cppLogLevel(std::string &category_name, uint16_t log_level){

    
   TypeSubCategoryMap::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
   if ( _log4cpp_sub_category_map.end() != tmp_iter ){

       tmp_iter->second._c_sub_log4cpp_log_level &= log_level;
   }
   else
   {
       
       E("Category(%s) is not found, Please set it.", category_name.c_str());
   }
   
}

void yLib::yLog::SetyLogLogLevel(uint16_t log_level){

    _c_ylog_log_level &= log_level;
}

void yLib::yLog::SetyLogLogLevel(std::string &category_name, uint16_t log_level){

TypeSubCategoryMap::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
   if ( _log4cpp_sub_category_map.end() != tmp_iter ){

       tmp_iter->second._c_sub_ylog_log_level &= log_level;
   }
   else
   {
       
       E("Category(%s) is not found, Please set it.", category_name.c_str());
   }
}


void yLib::yLog::SetLog4cpp(bool enable_log4cpp, std::string log_path ){
	
	if ( enable_log4cpp && (nullptr == _ptr_log4_category_root) ){//true, enable feature of log4cpp

        _b_enable_log4cpp = enable_log4cpp;
        
        //log4cpp
        log4cpp::PropertyConfigurator::configure(log_path);
        _ptr_log4_category_root = & log4cpp::Category::getRoot(); //rootCategory init by log4cpp::PropertyConfigurator::configure()
    }

	if ( !enable_log4cpp && (nullptr != _ptr_log4_category_root) ){

		log4cpp::Category::shutdown();
        _b_enable_log4cpp = false;
		_ptr_log4_category_root = nullptr;
	}
}

void yLib::yLog::SetLog4cppSubCategory(std::string category_name){

    //now , log4cpp was enabled.
    if ( _b_enable_log4cpp && (nullptr != _ptr_log4_category_root) ){//true, enable feature of log4cpp

        //not found category_name in _log4cpp_sub_category_map
        if ( _log4cpp_sub_category_map.end() == _log4cpp_sub_category_map.find(category_name) ){

            std::pair<std::string,SubCategoryProperty> tmp_value;
            //log4cpp::Category * ptr_sub_category = & log4cpp::Category::getInstance(category_name);
            log4cpp::Category * ptr_sub_category = & _ptr_log4_category_root->getInstance(category_name);
            
            tmp_value.first = category_name;
            tmp_value.second._ptr_sub_category = ptr_sub_category;
            tmp_value.second._c_sub_ylog_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;
            tmp_value.second._c_sub_log4cpp_log_level = yLogLevel::_ENABLE_ALL_LOG_LEVEL_;

            _log4cpp_sub_category_map.insert(tmp_value);
        }
        else{

            W("Warning!!!!, category_name(%s) was inited by other operation.", category_name.c_str());
        }
        
    }
    else{
        
        W("Warning!!!!, yLog's feature about log4cpp was not enabled.");
    }
}

void yLib::yLog::_ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list){

#ifdef _WIN32 || _WIN64

    if ( !_thread_mutex_is_init ){

        _init_thread_mutex();
        _thread_mutex_is_init = true;
    }

    //mutex lock
    if ( _b_enable_feature_ps ){

        //pthread_mutex_lock(&_process_mutex);
        DWORD wait_mutex_ret = WaitForSingleObject(_thread_mutex_handle, INFINITE);
        if ( WAIT_OBJECT_0 != wait_mutex_ret ){//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.

            if ( WAIT_FAILED == wait_mutex_ret ){
                
                HLOCAL LocalAddress = NULL;
		        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			        NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
                std::cout<<"yLib::yLog::_ylog_log_impl(): wait mutex failed, error info is "<<(char *)LocalAddress <<std::endl;
                return ;
            }
        }
    }
    else{ // thread safety

        DWORD wait_mutex_ret = WaitForSingleObject(_thread_mutex_handle, INFINITE);
        if ( WAIT_OBJECT_0 != wait_mutex_ret ){//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.

            if ( WAIT_FAILED == wait_mutex_ret ){
                
                HLOCAL LocalAddress = NULL;
		        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			        NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
                std::cout<<"yLib::yLog::_ylog_log_impl(): wait mutex failed, error info is "<<(char *)LocalAddress <<std::endl;
                return ;
            }
        }
    }

#elif __linux__ || __linux
    //mutex lock
    if ( _b_enable_feature_ps ){

        pthread_mutex_lock(&_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&_thread_mutex);
    }

#elif __unix__ || __unix

#endif //__unix__ || __unix



    memset(_c_ptr_msg_buf, 0, MSG_BUF_SIZE);//set buffer to '0'

    int _n_printed_num = 0; 

    _n_printed_num = vsnprintf(_c_ptr_msg_buf, MSG_BUF_SIZE - 1, fmt, arg_list);

    //std::cout<<"_n_printed_num " << _n_printed_num<<std::endl;
    if ( MSG_BUF_SIZE <= _n_printed_num ){

        //W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE - 1); //bug 
        std::cout<<"Size: "<< _n_printed_num <<" of input-string may be greater than MSG_BUF_SIZE: " << MSG_BUF_SIZE - 1 <<", input-string will be truncated by size of MSG_BUF_SIZE"<<std::endl;
    }

    switch (log_type)
    {
    case ENABLE_DEBUG_LOG_LEVEL:{
        
        /* code */
        if ( log_type == (log_type & _c_ylog_log_level) ){

            std::cout<<"LogDebug:>"<<_c_ptr_msg_buf<<std::endl;
        }
        
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->debug(_c_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_INFO_LOG_LEVEL:{
        
        /* code */
        if ( log_type == (log_type & _c_ylog_log_level) ){ //enable info ylog

            std::cout<<"LogInfo :>"<<_c_ptr_msg_buf<<std::endl;
        }
        
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->info(_c_ptr_msg_buf);
        }        
        break;
    }
    case ENABLE_WARN_LOG_LEVEL:{

        /* code */
        if ( log_type == (log_type & _c_ylog_log_level) ){

            std::cout<<"LogWarn :>"<<_c_ptr_msg_buf<<std::endl;
        }
        
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->warn(_c_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_ERROR_LOG_LEVEL:{

        /* code */
        if ( log_type == (log_type & _c_ylog_log_level) ){

            std::cout<<"LogError:>"<<_c_ptr_msg_buf<<std::endl;
        }
        
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->error(_c_ptr_msg_buf);
        }
        break;
    }
    default:
    
        E("_ylog_log_impl(): input log type error.");
        break;
    }
#ifdef _WIN32 || _WIN64

    ReleaseMutex(_thread_mutex_handle);

#elif __linux__ || __linux

    //mutex unlock 
    if ( _b_enable_feature_ps ){

        pthread_mutex_unlock(&_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&_thread_mutex);
    }

#elif __unix__ || __unix

#endif //__unix__ || __unix

}

//log info 
void yLib::yLog::I(const std::string fmt, ...){
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt.c_str(), arg);
    
    va_end(arg);
}

void yLib::yLog::I(const char * fmt , ...){
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt, arg);
    
    va_end(arg);
}

//log debug
void yLib::yLog::D(const std::string fmt , ...){
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::D(const char * fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt, arg);

    va_end(arg);
}

//log warning
void yLib::yLog::W(const std::string fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::W(const char * fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt, arg);

    va_end(arg);
}

//log error
void yLib::yLog::E(const std::string fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::E(const char * fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt, arg);

    va_end(arg);
}


//support sub category for log4cpp start -----------------

void yLib::yLog::_ylog_log_impl(uint16_t log_type, const char * fmt, va_list arg_list, std::string & category_name){

#ifdef _WIN32 || _WIN64

    if ( !_thread_mutex_is_init ){

        _init_thread_mutex();
        _thread_mutex_is_init = true;
    }

    //mutex lock
    if ( _b_enable_feature_ps ){

        DWORD wait_mutex_ret = WaitForSingleObject(_thread_mutex_handle, INFINITE);
        if ( WAIT_OBJECT_0 != wait_mutex_ret ){//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.

            if ( WAIT_FAILED == wait_mutex_ret ){
                
                HLOCAL LocalAddress = NULL;
		        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			        NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
                std::cout<<"yLib::yLog::_ylog_log_impl(): wait mutex failed, error info is "<<(char *)LocalAddress <<std::endl;
                return ;
            }
        }
    }
    else{ // thread safety

        DWORD wait_mutex_ret = WaitForSingleObject(_thread_mutex_handle, INFINITE);
        if ( WAIT_OBJECT_0 != wait_mutex_ret ){//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.

            if ( WAIT_FAILED == wait_mutex_ret ){
                
                HLOCAL LocalAddress = NULL;
		        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			        NULL, GetLastError(), 0, (PTSTR)&LocalAddress, 0, NULL);
                std::cout<<"yLib::yLog::_ylog_log_impl(): wait mutex failed, error info is "<<(char *)LocalAddress <<std::endl;
                return ;
            }
        }
    }

#elif __linux__ || __linux
    //mutex lock
    if ( _b_enable_feature_ps ){

        pthread_mutex_lock(&_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&_thread_mutex);
    }

#elif __unix__ || __unix

#endif //__unix__ || __unix

    memset(_c_ptr_msg_buf, 0, MSG_BUF_SIZE);//set buffer to '0'

    int _n_printed_num = 0; 

    _n_printed_num = vsnprintf(_c_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg_list);

    if ( MSG_BUF_SIZE <= _n_printed_num ){

        //W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE - 1); //bug
        std::cout<<"Size: "<< _n_printed_num <<" of input-string may be greater than MSG_BUF_SIZE: " << MSG_BUF_SIZE - 1 <<", input-string will be truncated by size of MSG_BUF_SIZE"<<std::endl;
    }

    TypeSubCategoryMap::iterator tmp_sub_category_iter = _log4cpp_sub_category_map.find(category_name);//get sub-category by id

    switch (log_type)
    {
    case ENABLE_DEBUG_LOG_LEVEL:{
        
        /* code */
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_ylog_log_level)){

            std::cout<<"LogDebug:>"<<_c_ptr_msg_buf<<std::endl;
        }
        
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                //_ptr_log4_category_root->debug(_c_ptr_msg_buf);
                
                if ( _log4cpp_sub_category_map.end() ==  tmp_sub_category_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_sub_category_iter->second._ptr_sub_category->debug(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    case ENABLE_INFO_LOG_LEVEL:{
        
        /* code */
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_ylog_log_level)){
            
            std::cout<<"LogInfo :>"<<_c_ptr_msg_buf<<std::endl;
        }
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                
                //_ptr_log4_category_root->info(_c_ptr_msg_buf);

                if ( _log4cpp_sub_category_map.end() ==  tmp_sub_category_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_sub_category_iter->second._ptr_sub_category->info(_c_ptr_msg_buf);
                }
            }
                
        }        
        break;
    }
    case ENABLE_WARN_LOG_LEVEL:{

        /* code */
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_ylog_log_level)){

            std::cout<<"LogWarn :>"<<_c_ptr_msg_buf<<std::endl;
        }
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
             
                //_ptr_log4_category_root->warn(_c_ptr_msg_buf);
 
                if ( _log4cpp_sub_category_map.end() ==  tmp_sub_category_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{
                
                    tmp_sub_category_iter->second._ptr_sub_category->warn(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    case ENABLE_ERROR_LOG_LEVEL:{

        /* code */
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_ylog_log_level)){
            
            std::cout<<"LogError:>"<<_c_ptr_msg_buf<<std::endl;
        }
        if (log_type == (log_type & tmp_sub_category_iter->second._c_sub_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                
                //_ptr_log4_category_root->error(_c_ptr_msg_buf);
                if ( _log4cpp_sub_category_map.end() ==  tmp_sub_category_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_sub_category_iter->second._ptr_sub_category->error(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    default:
    
        E("_ylog_log_impl(): input log type error.");
        break;
    }



#ifdef _WIN32 || _WIN64

    ReleaseMutex(_thread_mutex_handle);

#elif __linux__ || __linux

    //mutex unlock 
    if ( _b_enable_feature_ps ){

        pthread_mutex_unlock(&_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&_thread_mutex);
    }

#elif __unix__ || __unix

#endif //__unix__ || __unix
}

//log info 
void yLib::yLog::I(std::string &category_name, const std::string fmt, ...){
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt.c_str(), arg, category_name);
    
    va_end(arg);
}

void yLib::yLog::I(std::string &category_name, const char * fmt , ...){

    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt, arg, category_name);
    
    va_end(arg);
}

//log debug
void yLib::yLog::D(std::string &category_name, const std::string fmt , ...){

    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::D(std::string &category_name, const char * fmt , ...){

    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt, arg, category_name);

    va_end(arg);
}

//log warning
void yLib::yLog::W(std::string &category_name, const std::string fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::W(std::string &category_name, const char * fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt, arg, category_name);

    va_end(arg);
}

//log error
void yLib::yLog::E(std::string &category_name, const std::string fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::E(std::string &category_name, const char * fmt , ...){

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt, arg, category_name);

    va_end(arg);
}
//support sub category for log4cpp end -----------------

// void yLib::yLog::SetLog4cpp(bool enable_log4cpp){

//     m_enable_log4cpp = enable_log4cpp;
// }

void yLib::yLog::SetProcessSafetyFeature(bool enable_feature){

#ifdef _WIN32 || _WIN64

    //You can use a mutex object to protect a shared resource from simultaneous access by multiple threads or processes. 

#elif __linux__ || __linux

    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);

    int ret = 0;
    if ( 0 < (ret = pthread_mutexattr_setpshared(&mtx_attr, PTHREAD_PROCESS_SHARED) ) ){
        
        if ( ENOTSUP == ret ){

            E("pthread_mutexattr_setpshared, pshared is PTHREAD_PROCESS_SHARED but the implementation does not support process-shared mutexes.");
        }
        return;
    }

    if ( enable_feature ){

        pthread_mutex_init(&_process_mutex, & mtx_attr);
        _b_enable_feature_ps = true;
    } 
    else{
        
        if ( _b_enable_feature_ps ){

            _b_enable_feature_ps = false;
            pthread_mutex_destroy(&_process_mutex);
        }
        else
            yLib::yLog::W("Feature of process-safety do not enable.");
    }

    pthread_mutexattr_destroy(&mtx_attr);

#elif __unix__ || __unix

#endif //__unix__ || __unix


}
