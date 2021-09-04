/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-18 15:42:22
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 21:42:23
 * @FilePath: \yLib\include\core\yhttp.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#ifndef __YLIB_UTILITY_YHTTP_HPP__
#define __YLIB_UTILITY_YHTTP_HPP__

#include "core/yobject.hpp"


#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

namespace yLib{

    typedef struct _yhttp_request_param_{


        //only support http
        std::string protocol_type="http";

        std::string username="";
        std::string password="";

        std::string host="127.0.0.1";

        int32_t port=80;

        std::string path="/";
        struct __yhttp_request_query_kv__{
            std::string key;
            std::string value;
        };
        std::vector<__yhttp_request_query_kv__> query_kv_vec;
        std::vector<std::string> http_header_vec;

        std::string fregment="";

    }yHttpRequestParam;

    typedef struct _yhttp_response_info_{
        
        long response_code;
        std::string content_type;
        std::vector<uint8_t> response_data_buf;
    }yHttpResponseInfo;

    typedef struct _yhttp_post_multi_part_item_{
        
        static std::string GetJpgFileMIMEType(void){return "image/jpeg";}
        static std::string GetPngFileMIMEType(void){return "image/png";}
        static std::string GetTextFileMIMEType(void){return "text/plain";}
        static std::string GetBinFileMIMEType(void){return "application/octet-stream";}


        std::string content_disposition;
        std::string content_type = "";

        std::string name;
        std::string filename;

        std::vector<uint8_t> data_buf;
    }yHttpPostMultiPartItem;  

    typedef struct _yhttp_post_urlencoded_item_{

        std::string key;
        std::string value;
    }yHttpPostUrlencodedItem;  


    class __YLIB_EXPORT__ yHttp MACRO_PUBLIC_INHERIT_YOBJECT {

        public:
        yHttp() noexcept;
        ~yHttp() noexcept;

        int8_t Get(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_);

        
        //post application/x-www-form-urlencoded
        int8_t PostDefault(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, std::vector<yHttpPostUrlencodedItem> & form_kv_vec_);
        //post application/json
        int8_t PostJson(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, const std::string & json_str_);
        //post multipart/form-data
        int8_t PostMultiPart(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, std::vector<yHttpPostMultiPartItem> & form_multi_part_vec_);

        protected:
        
        private:
        bool yhttp_is_init_ok = false;

        YLIB_DECLARE_CLASSINFO_CONTENT(yHttp);
    };

}



#endif //__YLIB_UTILITY_YHTTP_HPP__