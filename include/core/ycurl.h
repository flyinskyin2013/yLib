#ifndef _YCURL_H_
#define _YCURL_H_

#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include "ycommon.h"
#include "ylog.h"

namespace yLib{


typedef struct _ycurl_param_{

    std::string ip_addr;
    int port;
    //only support http
    std::string protocol_type;
}yCurlParam;

#define YCURL_RECBUF_MAX_LEN 4*1024

class yCurl{

    public:

    yCurl();
    ~yCurl();
    //http://xxx.xxx.xxx.xxx:port/interface?user_params
    int yCurl_Get(std::string & interface, std::string user_params);
    int yCurl_Post_Json(std::string & interface, std::string json_datas);
    int yCurl_Set_Param(std::string & ip_addr, int port = 80, std::string protocol_type = "http");
    bool m_bGlobalInit;

    protected:

    private:

    yCurlParam m_param;
    CURL *m_ptr_ycurl=NULL;
    CURLcode m_ycurl_ret;
    char * m_ptr_recbuf=NULL;
};

}

#endif //_YCURL_H_