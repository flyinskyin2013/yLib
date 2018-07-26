#include "ycurl.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
        
    //RecStr.from_bytes(ptr, ptr+size*nmemb-1);
    //RecStr = std::string(ptr,size*nmemb);
    memcpy(userdata,ptr,size*nmemb);
    std::string __msg =  "ycurl get data :"+std::string((char*)userdata);
    yLib::yLog::I( __msg.c_str() );

    return size*nmemb;
}

yLib::yCurl::yCurl(){

    if ( CURLE_OK != (m_ycurl_ret = curl_global_init(CURL_GLOBAL_ALL)) ){

        yLib::yLog::E("curl_global_init call failed ");
        m_bGlobalInit = true;
    } 
    else{

        m_bGlobalInit = false;
    }

    m_param.port=80;
    m_param.protocol_type="http";
    m_param.ip_addr="127.0.0.1";

    m_ptr_recbuf = new char[YCURL_RECBUF_MAX_LEN];

    memset(m_ptr_recbuf, 0, YCURL_RECBUF_MAX_LEN);
}

yLib::yCurl::~yCurl(){

    if ( m_bGlobalInit ){

        curl_global_cleanup();
    }

    delete [] m_ptr_recbuf;
}

int yLib::yCurl::yCurl_Set_Param(std::string & ip_addr, int port, std::string protocol_type){

    m_param.ip_addr = ip_addr;
    m_param.port = port;
    m_param.protocol_type = protocol_type;
    return 0;
}    

int yLib::yCurl::yCurl_Get(std::string & interface, std::string user_params){

    if ( !( m_ptr_ycurl = curl_easy_init() ) ){

        yLib::yLog::E("curl_easy_init call failed");
        return -1;
    }
    struct curl_slist *headers = NULL;

    std::string request_url = m_param.protocol_type + "://" + m_param.ip_addr + ":" + \
                        std::to_string(m_param.port) + "/" + interface + "?" + user_params;
    std::string __msg = "request url is :"+ request_url;
    yLib::yLog::D(__msg.c_str());
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_URL, request_url.c_str());

    curl_easy_setopt(m_ptr_ycurl, CURLOPT_WRITEFUNCTION, write_callback);  
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_WRITEDATA, (void*)m_ptr_recbuf);//设置写数据

    if ( CURLE_OK !=  (m_ycurl_ret = curl_easy_perform(m_ptr_ycurl)) ){

        curl_easy_cleanup(m_ptr_ycurl);
        __msg = "curl_easy_perform call failed, info="+std::string(curl_easy_strerror(m_ycurl_ret));
        yLib::yLog::E(__msg.c_str());
        return -1;
    }
    //curl_slist_free_all();
    return 0;
}

int yLib::yCurl::yCurl_Post_Json(std::string & interface, std::string json_datas){


    if ( !( m_ptr_ycurl = curl_easy_init() ) ){

        yLib::yLog::E("curl_easy_init call failed");
        return -1;
    }

    struct curl_slist *headers = NULL;
    headers=curl_slist_append(headers, "Content-Type:application/json");
    headers=curl_slist_append(headers, "Accept:application/json");
    headers = curl_slist_append(headers, "charset:utf-8");
    
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_HTTPHEADER, headers);
    //headers = curl_slist_append(headers, "Accept: Agent-007");
    //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string request_url = m_param.protocol_type + "://" + m_param.ip_addr + ":" + \
                        std::to_string(m_param.port) + "/" + interface;
    std::string __msg = "request url is :"+ request_url;
    yLib::yLog::D(__msg.c_str());
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_URL, request_url.c_str());
     //curl_easy_setopt(curl,  CURLOPT_CUSTOMREQUEST, "POST");//自定义请求方式
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_POST, 1);//设置为非0表示本次操作为POS

    //curl_easy_setopt(ycurl, CURLOPT_POSTFIELDSIZE, str.size());//设置上传json串长度,这个设置可以忽略
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_POSTFIELDS, json_datas.c_str()); 

    curl_easy_setopt(m_ptr_ycurl, CURLOPT_WRITEFUNCTION, write_callback);//设置回调函数
    curl_easy_setopt(m_ptr_ycurl, CURLOPT_WRITEDATA, (void*)m_ptr_recbuf);//设置写数据

    if ( CURLE_OK !=  (m_ycurl_ret = curl_easy_perform(m_ptr_ycurl)) ){

        curl_easy_cleanup(m_ptr_ycurl);
        curl_slist_free_all(headers);
        __msg = "curl_easy_perform call failed, info="+std::string(curl_easy_strerror(m_ycurl_ret));
        yLib::yLog::E(__msg.c_str());
        return -1;
    }


    curl_easy_cleanup(m_ptr_ycurl);
    curl_slist_free_all(headers);
    return 0;
}