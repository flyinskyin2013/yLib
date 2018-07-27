#include "ylog.h"

char * yLib::yLog::m_ptr_msg_buf = nullptr;
bool yLib::yLog::m_b_is_class_access = true;

yLib::yLog::yLog(){

    if ( nullptr ==  m_ptr_msg_buf ){
    
        m_ptr_msg_buf = new char[MSG_BUF_SIZE];
        m_b_is_class_access = false;
    }
}

yLib::yLog::~yLog(){

    delete [] m_ptr_msg_buf;
    m_b_is_class_access = true;
    m_ptr_msg_buf = nullptr;
}

void yLib::yLog::I(const char * fmt , ...){

    if ( m_b_is_class_access )
        m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogInfo :>"<<m_ptr_msg_buf<<std::endl;

    if ( m_b_is_class_access ){

        delete [] m_ptr_msg_buf;
        m_ptr_msg_buf = nullptr;
    }
}

void yLib::yLog::D(const char * fmt , ...){

    if ( m_b_is_class_access )
        m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogDebug:>"<<m_ptr_msg_buf<<std::endl;

    if ( m_b_is_class_access ){

        delete [] m_ptr_msg_buf;
        m_ptr_msg_buf = nullptr;
    }
}

void yLib::yLog::W(const char * fmt , ...){

    if ( m_b_is_class_access )
        m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogWarn :>"<<m_ptr_msg_buf<<std::endl;
    if ( m_b_is_class_access ){

        delete [] m_ptr_msg_buf;
        m_ptr_msg_buf = nullptr;
    }
}

void yLib::yLog::E(const char * fmt , ...){

    if ( m_b_is_class_access )
        m_ptr_msg_buf = new char[MSG_BUF_SIZE];

    memset(m_ptr_msg_buf, 0, MSG_BUF_SIZE);

    int ret = 0; 
    
    va_list arg ;

    va_start(arg, fmt);


    ret = vsnprintf(m_ptr_msg_buf, MSG_BUF_SIZE, fmt, arg);


    va_end(arg);
    
    std::cout<<"LogError:>"<<m_ptr_msg_buf<<std::endl;
    if ( m_b_is_class_access ){

        delete [] m_ptr_msg_buf;
        m_ptr_msg_buf = nullptr;
    }
}