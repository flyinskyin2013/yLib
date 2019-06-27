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



char  yLib::yLog::m_ptr_msg_buf[] = {'\0'};
//bool yLib::yLog::m_b_is_class_access = true;
log4cpp::Category * yLib::yLog::root = nullptr;
bool yLib::yLog::m_enable_log4cpp = false;
bool yLib::yLog::m_enable_feature_ps = false;
pthread_mutex_t yLib::yLog::m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t yLib::yLog::m_process_mutex;
char yLib::yLog::g_log4cpp_log_level = ENABLE_ALL_LOG_LEVEL;
char yLib::yLog::g_ylog_log_level = ENABLE_ALL_LOG_LEVEL;

yLib::yLog::yLog(){

    // if ( nullptr ==  m_ptr_msg_buf ){
    
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];
    //     m_b_is_class_access = false;
    // }


}

yLib::yLog::~yLog(){

    // delete [] m_ptr_msg_buf;
    // m_b_is_class_access = true;
    // m_ptr_msg_buf = nullptr;

    if ( m_enable_log4cpp ){
        //log4cpp
        log4cpp::Category::shutdown();
        m_enable_log4cpp = false;
	root = nullptr;
    }
}

void yLib::yLog::SetLog4cppLogLevel(char log_level){

    g_log4cpp_log_level = log_level;
}

void yLib::yLog::SetyLogLogLevel(char log_level){

    g_ylog_log_level = log_level;
}

void yLib::yLog::SetLog4cpp(bool enable_log4cpp, std::string log_path ){
	
	if ( enable_log4cpp && (nullptr == root) ){//true, open feature of log4cpp

        m_enable_log4cpp = enable_log4cpp;
        //log4cpp
        log4cpp::PropertyConfigurator::configure(log_path);
        root = & log4cpp::Category::getRoot();
    }

	if ( !enable_log4cpp && (nullptr != root) ){

		
		log4cpp::Category::shutdown();
        m_enable_log4cpp = false;
		root = nullptr;
	}
}

void yLib::yLog::_ylog_log_impl(char log_type, const char * fmt, va_list arg_list){

    //mutex lock
    if ( m_enable_feature_ps ){

        pthread_mutex_lock(&m_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&m_mutex);
    }


    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 

    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg_list);

    if ( MSG_BUF_SIZE -  1 == ret ){

        W("Size of input-string may be greater than MSG_BUF_SIZE:%d, input-string will be truncated by size of MSG_BUF_SIZE", MSG_BUF_SIZE);
    }

    switch (log_type)
    {
    case ENABLE_DEBUG_LOG_LEVEL:{
        /* code */
        std::cout<<"LogDebug:>"<<m_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & g_log4cpp_log_level)){

        //log4cpp
        if ( m_enable_log4cpp )
            root->debug(m_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_INFO_LOG_LEVEL:{
        /* code */
        std::cout<<"LogInfo :>"<<m_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & g_log4cpp_log_level)){

        //log4cpp
        if ( m_enable_log4cpp )
            root->info(m_ptr_msg_buf);
        }        
        break;
    }
    case ENABLE_WARN_LOG_LEVEL:{
        /* code */
        std::cout<<"LogWarn :>"<<m_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & g_log4cpp_log_level)){

        //log4cpp
        if ( m_enable_log4cpp )
            root->warn(m_ptr_msg_buf);
        }
        break;
    }
    case ENABLE_ERROR_LOG_LEVEL:{
        /* code */
        std::cout<<"LogError:>"<<m_ptr_msg_buf<<std::endl;
        if (log_type == (log_type & g_log4cpp_log_level)){

        //log4cpp
        if ( m_enable_log4cpp )
            root->error(m_ptr_msg_buf);
        }
        break;
    }
    default:
        break;
    }



    //mutex unlock 
    if ( m_enable_feature_ps ){

        pthread_mutex_unlock(&m_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&m_mutex);
    }
}


void yLib::yLog::I(const char * fmt , ...){

    if ( ENABLE_INFO_LOG_LEVEL != (ENABLE_INFO_LOG_LEVEL & g_ylog_log_level) ){

        return ;
    }


    
    va_list arg ;

    va_start(arg, fmt);


    _ylog_log_impl(ENABLE_INFO_LOG_LEVEL, fmt, arg);


    va_end(arg);


}

void yLib::yLog::D(const char * fmt , ...){

    if ( ENABLE_DEBUG_LOG_LEVEL != (ENABLE_DEBUG_LOG_LEVEL & g_ylog_log_level) ){

        return ;
    }

    
    va_list arg ;

    va_start(arg, fmt);


    _ylog_log_impl(ENABLE_DEBUG_LOG_LEVEL, fmt, arg);


    va_end(arg);

}

void yLib::yLog::W(const char * fmt , ...){


    if ( ENABLE_WARN_LOG_LEVEL != (ENABLE_WARN_LOG_LEVEL & g_ylog_log_level) ){

        return ;
    }

    
    va_list arg ;

    va_start(arg, fmt);

    _ylog_log_impl(ENABLE_WARN_LOG_LEVEL, fmt, arg);


    va_end(arg);
 
}

void yLib::yLog::E(const char * fmt , ...){

    if ( ENABLE_ERROR_LOG_LEVEL != (ENABLE_ERROR_LOG_LEVEL & g_ylog_log_level) ){

        return ;
    }

    
    va_list arg ;

    va_start(arg, fmt);


    _ylog_log_impl(ENABLE_ERROR_LOG_LEVEL, fmt, arg);


    va_end(arg);

    
}

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

        pthread_mutex_init(&m_process_mutex, & mtx_attr);
        m_enable_feature_ps = true;
    } 
    else{
        if ( m_enable_feature_ps ){

            m_enable_feature_ps = false;
            pthread_mutex_destroy(&m_process_mutex);
        }
        else
            yLib::yLog::W("Feature of process-safety do not enable.");
    }

    pthread_mutexattr_destroy(&mtx_attr);
}
