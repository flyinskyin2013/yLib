/*
 * @Author: Sky
 * @Date: 2018-10-23 11:13:10
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-20 19:02:52
 * @Description: 
 */

#ifndef _YCURL_H_
#define _YCURL_H_


#include <iostream>
#include <string.h>
#include "ycommon.hpp"
#include "ylog.hpp"


namespace yLib{


typedef struct _ycurl_param_{

    std::string ip_addr;
    int port;
    //only support http
    std::string protocol_type;
}yCurlParam;

#define YCURL_RECBUF_MAX_LEN 4*1024

class yCurl MACRO_PUBLIC_INHERIT_YOBJECT{

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
    void *m_ptr_ycurl=NULL;
    int m_ycurl_ret;
    char * m_ptr_recbuf=NULL;
};

}

#endif //_YCURL_H_