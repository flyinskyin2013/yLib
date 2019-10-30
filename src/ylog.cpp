/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-30 14:19:02
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



char  yLib::yLog::_c_ptr_msg_buf[MSG_BUF_SIZE];
//bool yLib::yLog::m_b_is_class_access = true;
log4cpp::Category * yLib::yLog::_ptr_log4_category_root = nullptr;
std::unordered_map<std::string, log4cpp::Category *> yLib::yLog::_log4cpp_sub_category_map;

bool yLib::yLog::_b_enable_log4cpp = false;
bool yLib::yLog::_b_enable_feature_ps = false;

pthread_mutex_t yLib::yLog::_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t yLib::yLog::_process_mutex;

char yLib::yLog::_c_log4cpp_log_level = ENABLE_ALL_LOG_LEVEL;
char yLib::yLog::_c_ylog_log_level = ENABLE_ALL_LOG_LEVEL;

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

void yLib::yLog::SetLog4cppLogLevel(char log_level){

    
    _c_log4cpp_log_level = log_level;
}

void yLib::yLog::SetyLogLogLevel(char log_level){

    _c_ylog_log_level = log_level;
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

            std::pair<std::string,log4cpp::Category *> tmp_value;
            //log4cpp::Category * ptr_sub_category = & log4cpp::Category::getInstance(category_name);
            log4cpp::Category * ptr_sub_category = & _ptr_log4_category_root->getInstance(category_name);
            
            tmp_value.first = category_name;
            tmp_value.second = ptr_sub_category;
            _log4cpp_sub_category_map.insert(tmp_value);
        }
        else{

            W("Warning!!!!, category_name(%s) was inited by other operation.");
        }
        
    }
    else{
        
        W("Warning!!!!, yLog's feature about log4cpp was not enabled.");
    }
}

void yLib::yLog::_ylog_log_impl(char log_type, const char * fmt, va_list arg_list){

    //mutex lock
    if ( _b_enable_feature_ps ){

        pthread_mutex_lock(&_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&_thread_mutex);
    }

    memset(_c_ptr_msg_buf, 0, MSG_BUF_SIZE);//set buffer to '0'

    int _n_printed_num = 0; 

    _n_printed_num = vsnprintf(_c_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg_list);

    if ( MSG_BUF_SIZE - 1 == _n_printed_num ){

        W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE - 1);
    }

    switch (log_type)
    {
    case ENABLE_DEBUG_LOG_LEVEL:{
        
        /* code */
        std::cout<<"LogDebug:>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->debug(_c_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_INFO_LOG_LEVEL:{
        
        /* code */
        std::cout<<"LogInfo :>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->info(_c_ptr_msg_buf);
        }        
        break;
    }
    case ENABLE_WARN_LOG_LEVEL:{

        /* code */
        std::cout<<"LogWarn :>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp )
                _ptr_log4_category_root->warn(_c_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_ERROR_LOG_LEVEL:{

        /* code */
        std::cout<<"LogError:>"<<_c_ptr_msg_buf<<std::endl;
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



    //mutex unlock 
    if ( _b_enable_feature_ps ){

        pthread_mutex_unlock(&_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&_thread_mutex);
    }
}





//log info 
void yLib::yLog::I(const std::string fmt, ...){

    if ( ENABLE_INFO_LOG_LEVEL != (ENABLE_INFO_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt.c_str(), arg);
    
    va_end(arg);
}

void yLib::yLog::I(const char * fmt , ...){

    if ( ENABLE_INFO_LOG_LEVEL != (ENABLE_INFO_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt, arg);
    
    va_end(arg);
}

//log debug
void yLib::yLog::D(const std::string fmt , ...){

    if ( ENABLE_DEBUG_LOG_LEVEL != (ENABLE_DEBUG_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::D(const char * fmt , ...){

    if ( ENABLE_DEBUG_LOG_LEVEL != (ENABLE_DEBUG_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt, arg);

    va_end(arg);
}

//log warning
void yLib::yLog::W(const std::string fmt , ...){

    if ( ENABLE_WARN_LOG_LEVEL != (ENABLE_WARN_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::W(const char * fmt , ...){

    if ( ENABLE_WARN_LOG_LEVEL != (ENABLE_WARN_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt, arg);

    va_end(arg);
}

//log error
void yLib::yLog::E(const std::string fmt , ...){

    if ( ENABLE_ERROR_LOG_LEVEL != (ENABLE_ERROR_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt.c_str(), arg);

    va_end(arg);
}
void yLib::yLog::E(const char * fmt , ...){

    if ( ENABLE_ERROR_LOG_LEVEL != (ENABLE_ERROR_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt, arg);

    va_end(arg);
}


//support sub category for log4cpp start -----------------

void yLib::yLog::_ylog_log_impl(char log_type, const char * fmt, va_list arg_list, std::string & category_name){

    //mutex lock
    if ( _b_enable_feature_ps ){

        pthread_mutex_lock(&_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&_thread_mutex);
    }

    memset(_c_ptr_msg_buf, 0, MSG_BUF_SIZE);//set buffer to '0'

    int _n_printed_num = 0; 

    _n_printed_num = vsnprintf(_c_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg_list);

    if ( MSG_BUF_SIZE - 1 == _n_printed_num ){

        W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE - 1);
    }

    switch (log_type)
    {
    case ENABLE_DEBUG_LOG_LEVEL:{
        
        /* code */
        std::cout<<"LogDebug:>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                //_ptr_log4_category_root->debug(_c_ptr_msg_buf);
                std::unordered_map<std::string, log4cpp::Category *>::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
                if ( _log4cpp_sub_category_map.end() ==  tmp_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_iter->second->debug(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    case ENABLE_INFO_LOG_LEVEL:{
        
        /* code */
        std::cout<<"LogInfo :>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                
                //_ptr_log4_category_root->info(_c_ptr_msg_buf);
                std::unordered_map<std::string, log4cpp::Category *>::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
                if ( _log4cpp_sub_category_map.end() ==  tmp_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_iter->second->info(_c_ptr_msg_buf);
                }
            }
                
        }        
        break;
    }
    case ENABLE_WARN_LOG_LEVEL:{

        /* code */
        std::cout<<"LogWarn :>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
             
                //_ptr_log4_category_root->warn(_c_ptr_msg_buf);
                std::unordered_map<std::string, log4cpp::Category *>::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
                if ( _log4cpp_sub_category_map.end() ==  tmp_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{
                
                    tmp_iter->second->warn(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    case ENABLE_ERROR_LOG_LEVEL:{

        /* code */
        std::cout<<"LogError:>"<<_c_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & _c_log4cpp_log_level)){

            //log4cpp
            if ( _b_enable_log4cpp ){
                
                //_ptr_log4_category_root->error(_c_ptr_msg_buf);
                std::unordered_map<std::string, log4cpp::Category *>::iterator tmp_iter = _log4cpp_sub_category_map.find(category_name);
                if ( _log4cpp_sub_category_map.end() ==  tmp_iter){

                    E("Category(%s) is not found, Please set it.", category_name.c_str());
                }
                else{

                    tmp_iter->second->error(_c_ptr_msg_buf);
                }
            }
        }
        break;
    }
    default:
    
        E("_ylog_log_impl(): input log type error.");
        break;
    }



    //mutex unlock 
    if ( _b_enable_feature_ps ){

        pthread_mutex_unlock(&_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&_thread_mutex);
    }
}

//log info 
void yLib::yLog::I(std::string &category_name, const std::string fmt, ...){

    if ( ENABLE_INFO_LOG_LEVEL != (ENABLE_INFO_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt.c_str(), arg, category_name);
    
    va_end(arg);
}

void yLib::yLog::I(std::string &category_name, const char * fmt , ...){

    if ( ENABLE_INFO_LOG_LEVEL != (ENABLE_INFO_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt, arg, category_name);
    
    va_end(arg);
}

//log debug
void yLib::yLog::D(std::string &category_name, const std::string fmt , ...){

    if ( ENABLE_DEBUG_LOG_LEVEL != (ENABLE_DEBUG_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::D(std::string &category_name, const char * fmt , ...){

    if ( ENABLE_DEBUG_LOG_LEVEL != (ENABLE_DEBUG_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }
    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt, arg, category_name);

    va_end(arg);
}

//log warning
void yLib::yLog::W(std::string &category_name, const std::string fmt , ...){

    if ( ENABLE_WARN_LOG_LEVEL != (ENABLE_WARN_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::W(std::string &category_name, const char * fmt , ...){

    if ( ENABLE_WARN_LOG_LEVEL != (ENABLE_WARN_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt, arg, category_name);

    va_end(arg);
}

//log error
void yLib::yLog::E(std::string &category_name, const std::string fmt , ...){

    if ( ENABLE_ERROR_LOG_LEVEL != (ENABLE_ERROR_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt.c_str(), arg, category_name);

    va_end(arg);
}
void yLib::yLog::E(std::string &category_name, const char * fmt , ...){

    if ( ENABLE_ERROR_LOG_LEVEL != (ENABLE_ERROR_LOG_LEVEL & _c_ylog_log_level) ){

        return ;
    }

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
}
