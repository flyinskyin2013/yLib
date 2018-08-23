#include "ylog.h"

char  yLib::yLog::m_ptr_msg_buf[] = {'\0'};
//bool yLib::yLog::m_b_is_class_access = true;
log4cpp::Category * yLib::yLog::root = nullptr;
bool yLib::yLog::m_enable_log4cpp = false;
bool yLib::yLog::m_enable_feature_ps = false;
pthread_mutex_t yLib::yLog::m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t yLib::yLog::m_process_mutex;

yLib::yLog::yLog(bool enable_log4cpp , std::string log_path){

    // if ( nullptr ==  m_ptr_msg_buf ){
    
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];
    //     m_b_is_class_access = false;
    // }

    if ( enable_log4cpp ){

        m_enable_log4cpp = enable_log4cpp;
        //log4cpp
        log4cpp::PropertyConfigurator::configure(log_path);
        root = & log4cpp::Category::getRoot();
    }
}

yLib::yLog::~yLog(){

    // delete [] m_ptr_msg_buf;
    // m_b_is_class_access = true;
    // m_ptr_msg_buf = nullptr;

    if ( m_enable_log4cpp ){
        //log4cpp
        log4cpp::Category::shutdown();
        m_enable_log4cpp = false;
    }
}

void yLib::yLog::I(const char * fmt , ...){

    // if ( m_b_is_class_access )
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    if ( m_enable_feature_ps ){

        pthread_mutex_lock(&m_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&m_mutex);
    }

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogInfo :>"<<m_ptr_msg_buf<<std::endl;

    //log4cpp
    if ( m_enable_log4cpp )
        root->info(m_ptr_msg_buf);

    // if ( m_b_is_class_access ){

    //     delete [] m_ptr_msg_buf;
    //     m_ptr_msg_buf = nullptr;
    // }

    if ( m_enable_feature_ps ){

        pthread_mutex_unlock(&m_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&m_mutex);
    }
}

void yLib::yLog::D(const char * fmt , ...){

    // if ( m_b_is_class_access )
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    if ( m_enable_feature_ps ){

        pthread_mutex_lock(&m_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&m_mutex);
    }

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogDebug:>"<<m_ptr_msg_buf<<std::endl;

    //log4cpp
    if ( m_enable_log4cpp )
        root->debug(m_ptr_msg_buf);

    // if ( m_b_is_class_access ){

    //     delete [] m_ptr_msg_buf;
    //     m_ptr_msg_buf = nullptr;
    // }

    if ( m_enable_feature_ps ){

        pthread_mutex_unlock(&m_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&m_mutex);
    }
}

void yLib::yLog::W(const char * fmt , ...){

    // if ( m_b_is_class_access )
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    if ( m_enable_feature_ps ){

        pthread_mutex_lock(&m_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&m_mutex);
    }

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogWarn :>"<<m_ptr_msg_buf<<std::endl;

        //log4cpp
    if ( m_enable_log4cpp )
        root->warn(m_ptr_msg_buf);


    // if ( m_b_is_class_access ){

    //     delete [] m_ptr_msg_buf;
    //     m_ptr_msg_buf = nullptr;
    // }

    if ( m_enable_feature_ps ){

        pthread_mutex_unlock(&m_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&m_mutex);
    }
}

void yLib::yLog::E(const char * fmt , ...){

    // if ( m_b_is_class_access )
    //     m_ptr_msg_buf = new char[MSG_BUF_SIZE];
    if ( m_enable_feature_ps ){

        pthread_mutex_lock(&m_process_mutex);
    }
    else{ // thread safety

        pthread_mutex_lock(&m_mutex);
    }
    

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);

    std::cout<<"LogError:>"<<std::string(m_ptr_msg_buf)<<std::endl;
    //log4cpp
    if ( m_enable_log4cpp )
        root->warn(m_ptr_msg_buf);


    // if ( m_b_is_class_access ){

    //     delete [] m_ptr_msg_buf;
    //     m_ptr_msg_buf = nullptr;
    // }

    if ( m_enable_feature_ps ){

        pthread_mutex_unlock(&m_process_mutex);
    }
    else{//thread safety

        pthread_mutex_unlock(&m_mutex);
    }
    
}

// void yLib::yLog::SetLog4cpp(bool enable_log4cpp){

//     m_enable_log4cpp = enable_log4cpp;
// }

void yLib::yLog::EnableFeatureProcessSafety(bool enable_feature){

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