/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-18 15:42:09
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:13:17
 * @FilePath: \yLib\src\yhttp.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "utility/yhttp.hpp"
#include "utility/ylog.hpp"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include "curl/curl.h"

#ifdef __cplusplus
}
#endif //__cplusplus


YLIB_IMPLEMENT_CLASSINFO_CONTENT(yHttp)


static void __little_endian_uint32_to_buf__(uint8_t * buf_,uint32_t len_){

    memset(buf_, 0x00, 4);
    buf_[0] |= (uint8_t)(0xFF & (uint8_t)(len_ & 0x000000FF));
    buf_[1] |= (uint8_t)(0xFF & (uint8_t)((len_ & 0x0000FF00) >> 8));
    buf_[2] |= (uint8_t)(0xFF & (uint8_t)((len_ & 0x00FF0000) >> 16));;
    buf_[3] |= (uint8_t)(0xFF & (uint8_t)((len_ & 0xFF000000) >> 24));;
}

static void __little_endian_buf_to_uint32__(uint8_t * buf_,uint32_t * len_){

    uint32_t tmp_len_ = 0;
    
    tmp_len_ |= (0x000000FF & buf_[0]);
    tmp_len_ |= ((0x000000FF & buf_[1]) << 8);
    tmp_len_ |= ((0x000000FF & buf_[2]) << 16);
    tmp_len_ |= ((0x000000FF & buf_[3]) << 24);

    * len_ = tmp_len_;
}


static size_t write_callback(char *buffer_, size_t size_, size_t nmemb_, void *userdata_){
    
    
    uint8_t ** _data_buf = (uint8_t **)userdata_;
    uint32_t _buffer_len = 0;
    uint32_t _buffer_used_len = 0;

    //yLib::yLog::I("buffer_ %x, size_ %d, nmemb_ %d, userdata_ %x", buffer_, size_, nmemb_, _data_buf);

    __little_endian_buf_to_uint32__(*_data_buf, &_buffer_len);
    __little_endian_buf_to_uint32__(*_data_buf + 4, &_buffer_used_len);

    //yLib::yLog::I("_buffer_len %d, _buffer_used_len %d", _buffer_len, _buffer_used_len);
    if ( (_buffer_len - _buffer_used_len) >= size_* nmemb_ ){
        
        memcpy(*_data_buf + 8 + _buffer_used_len, buffer_, size_*nmemb_);
        _buffer_used_len += size_*nmemb_;
        __little_endian_uint32_to_buf__(*_data_buf + 4, _buffer_used_len);
    }
    else{

        uint32_t _now_not_used_len = _buffer_len - _buffer_used_len;
        uint32_t _alloc_4k_count = (size_* nmemb_ / 4096) + 1;

        uint8_t * _new_buf_addr = (uint8_t *)realloc(*_data_buf, _buffer_len + _alloc_4k_count * 4096);
        if (NULL == _new_buf_addr){

            yLib::yLog::E("Realloc mem failed.");
            return 0;
        }
        else
        {
            *_data_buf = _new_buf_addr;
            *(uint8_t **)userdata_ = _new_buf_addr;
            __little_endian_uint32_to_buf__(*_data_buf, _buffer_len + _alloc_4k_count * 4096);
        }
        

        memcpy((*(uint8_t **)userdata_) + 8 + _buffer_used_len, buffer_, size_*nmemb_);
        _buffer_used_len += size_*nmemb_;
        __little_endian_uint32_to_buf__(*_data_buf + 4, _buffer_used_len);
    }

    return size_* nmemb_;
}


static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)  
{  

    return 0;  
}  


yLib::yHttp::yHttp() noexcept MACRO_INIT_YOBJECT_PROPERTY(yHttp) {

    CURLcode _curl_init_code = curl_global_init(CURL_GLOBAL_ALL);

    if (CURLE_OK == _curl_init_code)
        yhttp_is_init_ok = true;
    else
    {
        yhttp_is_init_ok = false;
        yLib::yLog::E("Error info is %s. Please contact developer.", std::string(curl_easy_strerror((CURLcode)_curl_init_code)).c_str());
    }
}

yLib::yHttp::~yHttp() noexcept{

    if (yhttp_is_init_ok)
        curl_global_cleanup();
}

static int8_t __yhttp_check_protocol_type__(const yLib::yHttpRequestParam &request_param_){

    //check protocol type
    if ( "http" != request_param_.protocol_type ){

        yLib::yLog::E("Invalid protocol_type. yHttp only support http-protocol.");
        return -1;
    }
    return 0;
}

static int8_t __yhttp_create_http_handle__(CURL ** ptr_http_handle){

    //create http handle
    if ( !( *ptr_http_handle = curl_easy_init() ) ){

        yLib::yLog::E("create http handle failed");
        return -1;
    }
    return 0;
}

static int8_t __yhttp_parse_set_http_header__(CURL * ptr_http_handle, const yLib::yHttpRequestParam &request_param_, curl_slist *http_headers){

    for (auto iter_ = request_param_.http_header_vec.begin(); iter_ != request_param_.http_header_vec.end(); iter_ ++){

        http_headers=curl_slist_append(http_headers, iter_->c_str());
    }
    // headers=curl_slist_append(headers, "Content-Type:application/json");
    // headers=curl_slist_append(headers, "Accept:application/json");
    // headers = curl_slist_append(headers, "charset:utf-8");
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_HTTPHEADER, http_headers) ){

        yLib::yLog::E("set http header info failed.");
        return -1;
    }

    return 0;
}

static int8_t __yhttp_parse_set_http_url__(CURL * ptr_http_handle, const yLib::yHttpRequestParam &request_param_){


    //protocol
    std::string request_url = request_param_.protocol_type + "://";

    //user information
    if (request_param_.username != "" || request_param_.password != "")
        request_url +=  request_param_.username + ":" + request_param_.password + "@";

    //host
    request_url += request_param_.host + ":";

    //port 
    request_url += std::to_string(request_param_.port);

    //request path
    request_url += request_param_.path;
    
    //request kv param
    if (request_param_.query_kv_vec.size() > 0){
        request_url += "?";
        std::string parse_kv_str = "";
        for (auto iter_ = request_param_.query_kv_vec.begin();iter_ != request_param_.query_kv_vec.end(); iter_++){

            parse_kv_str += iter_->key + "=" + iter_->value + "&";
        }
        request_url += parse_kv_str.substr(0, parse_kv_str.length() - 1);
    }
    
    //fragment
    if ("" != request_param_.fregment )
        request_url += "#" + request_param_.fregment;
    
    //yLib::yLog::I(request_url);
    CURLcode curl_setopt_code = curl_easy_setopt(ptr_http_handle, CURLOPT_URL, request_url.c_str());
    if ( CURLE_OK != curl_setopt_code ){

        yLib::yLog::E("set http url info failed. error info is %s", std::string(curl_easy_strerror((CURLcode)curl_setopt_code)).c_str());
        return -1;
    }
    return 0;
}

static int8_t __yhttp_set_response_callback_buffer__(CURL * ptr_http_handle, uint8_t **data_buffer_ptr_, uint32_t buffer_len_){

    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_WRITEFUNCTION, write_callback) ){

        yLib::yLog::E("set http write function failed.");
        return -1;
    }
    //yLib::yLog::I("data_buffer_ptr_ %x, buffer_len_ %d", data_buffer_ptr_, buffer_len_);
    __little_endian_uint32_to_buf__(*data_buffer_ptr_, buffer_len_);//all len
    __little_endian_uint32_to_buf__(*data_buffer_ptr_ + 4, 0);//read len
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_WRITEDATA, (void *)data_buffer_ptr_) ){

        yLib::yLog::E("set http write data ptr failed.");
        return -1;
    }
    return 0;
}


static int8_t __yhttp_start_http_session__(CURL * ptr_http_handle){

    CURLcode _curl_perform_code = curl_easy_perform(ptr_http_handle);
    if ( CURLE_OK !=  _curl_perform_code ){

        std::string _error_msg = "http get failed, error info is "+std::string(curl_easy_strerror((CURLcode)_curl_perform_code));
        yLib::yLog::E(_error_msg.c_str());
        return -1;
    }
    
    return 0;
}


static int8_t __yhttp_process_http_response__(CURL * ptr_http_handle, uint8_t *data_buffer_ptr_, yLib::yHttpResponseInfo & response_info_){


    if ( CURLE_OK != curl_easy_getinfo(ptr_http_handle, CURLINFO_RESPONSE_CODE, &response_info_.response_code)){
        
        yLib::yLog::E("set http get response code failed.");
        return -1;
    }


    char * content_type = NULL;
    if ( CURLE_OK != curl_easy_getinfo(ptr_http_handle, CURLINFO_CONTENT_TYPE, &content_type)){
        
        yLib::yLog::E("set http get content type failed.");
        return -1;
    }
    else
    {
        response_info_.content_type = content_type;
        content_type = NULL;
    }
    


    // char * content_type;
    // if ( CURLE_OK != curl_easy_getinfo(ptr_http_handle, CURLINFO_CONTENT_TYPE, &content_type)){
        
    //     curl_slist_free_all(http_headers);
    //     curl_easy_cleanup(ptr_http_handle);
    //     yLib::yLog::E("set http get content type failed.");
    //     return -1;
    // }

    uint32_t buffer_len_ = 0;
    uint32_t buffer_used_len_ = 0;
    __little_endian_buf_to_uint32__(data_buffer_ptr_, &buffer_len_);
    __little_endian_buf_to_uint32__(data_buffer_ptr_ + 4, &buffer_used_len_);
    response_info_.response_data_buf.resize(buffer_used_len_);
    memcpy(response_info_.response_data_buf.data(), data_buffer_ptr_ + 8, buffer_used_len_);
    return 0;
}



static int8_t __yhttp_cleanup__(CURL * ptr_http_handle_, \
            struct curl_slist *http_headers_,\
            uint8_t *data_buffer_ptr_){
    
    if (NULL != data_buffer_ptr_)
        free(data_buffer_ptr_);

    if (NULL != http_headers_)
        curl_slist_free_all(http_headers_);

    if (NULL != ptr_http_handle_)
        curl_easy_cleanup(ptr_http_handle_);

    return -1;
}

int8_t yLib::yHttp::Get(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_){

    CURL * ptr_http_handle = NULL;
    struct curl_slist *http_headers = NULL;
    uint8_t *data_buffer_ptr_ = NULL;

    if (0 > __yhttp_check_protocol_type__(request_param_))
        return -1;

    //create http handle
    if (0 > __yhttp_create_http_handle__(&ptr_http_handle))
        return -1;

    //fill http header
    if (request_param_.http_header_vec.size() > 0)
        if (0 > __yhttp_parse_set_http_header__(ptr_http_handle, request_param_, http_headers)){

            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }

    //fill http url
    if (0 > __yhttp_parse_set_http_url__(ptr_http_handle, request_param_)){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //set response callback and buffer
    data_buffer_ptr_ = (uint8_t *)malloc(1024 * sizeof(uint8_t));
    if (0 > __yhttp_set_response_callback_buffer__(ptr_http_handle, &data_buffer_ptr_, 1024 * sizeof(uint8_t))){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }


    //start http and process response
    if (0 > __yhttp_start_http_session__(ptr_http_handle))//start http failed.
    {
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }
    else//start http ok
    {
        if (0 > __yhttp_process_http_response__(ptr_http_handle, data_buffer_ptr_, response_info_)){
            
            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }
    }

    __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
    return 0;
}

//post application/x-www-form-urlencoded
int8_t yLib::yHttp::PostDefault(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, std::vector<yHttpPostUrlencodedItem> & form_kv_vec_){

    CURL * ptr_http_handle = NULL;
    struct curl_slist *http_headers = NULL;
    uint8_t *data_buffer_ptr_ = NULL;

    if (0 > __yhttp_check_protocol_type__(request_param_))
        return -1;

    //create http handle
    if (0 > __yhttp_create_http_handle__(&ptr_http_handle))
        return -1;

    //fill http header
    if (request_param_.http_header_vec.size() > 0)
        if (0 > __yhttp_parse_set_http_header__(ptr_http_handle, request_param_, http_headers)){

            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }

    //fill http url
    if (0 > __yhttp_parse_set_http_url__(ptr_http_handle, request_param_)){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //set this session method is post
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_POST, 1) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        yLib::yLog::E("set http method type(post) failed.");
        return -1;
    }

    std::string _form_kv_str = "";
    for (auto iter_ = form_kv_vec_.begin(); iter_ != form_kv_vec_.end(); iter_ ++){

        _form_kv_str += iter_->key + "=" + iter_->value + "&";
    }
    _form_kv_str = _form_kv_str.substr(0, _form_kv_str.length() - 1);
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_POSTFIELDS, _form_kv_str.c_str()) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        yLib::yLog::E("set http post json failed.");
        return -1;
    }

    //set response callback and buffer
    data_buffer_ptr_ = (uint8_t *)malloc(1024 * sizeof(uint8_t));
    if (0 > __yhttp_set_response_callback_buffer__(ptr_http_handle, &data_buffer_ptr_, 1024 * sizeof(uint8_t))){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //start http and process response
    if (0 > __yhttp_start_http_session__(ptr_http_handle))//start http failed.
    {
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }
    else//start http ok
    {
        if (0 > __yhttp_process_http_response__(ptr_http_handle, data_buffer_ptr_, response_info_)){
            
            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }
    }

    __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
    return 0;    
}
//post application/json
int8_t yLib::yHttp::PostJson(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, const std::string & json_str_){

    CURL * ptr_http_handle = NULL;
    struct curl_slist *http_headers = NULL;
    uint8_t *data_buffer_ptr_ = NULL;

    if (0 > __yhttp_check_protocol_type__(request_param_))
        return -1;

    //create http handle
    if (0 > __yhttp_create_http_handle__(&ptr_http_handle))
        return -1;

    //fill http header
    if (request_param_.http_header_vec.size() > 0)
        if (0 > __yhttp_parse_set_http_header__(ptr_http_handle, request_param_, http_headers)){

            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }

    //fill http url
    if (0 > __yhttp_parse_set_http_url__(ptr_http_handle, request_param_)){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //set this session method is post
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_POST, 1) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        yLib::yLog::E("set http method type(post) failed.");
        return -1;
    }

    
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_POSTFIELDS, json_str_.c_str()) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        yLib::yLog::E("set http post json failed.");
        return -1;
    }

    //set response callback and buffer
    data_buffer_ptr_ = (uint8_t *)malloc(1024 * sizeof(uint8_t));
    if (0 > __yhttp_set_response_callback_buffer__(ptr_http_handle, &data_buffer_ptr_, 1024 * sizeof(uint8_t))){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //start http and process response
    if (0 > __yhttp_start_http_session__(ptr_http_handle))//start http failed.
    {
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }
    else//start http ok
    {
        if (0 > __yhttp_process_http_response__(ptr_http_handle, data_buffer_ptr_, response_info_)){
            
            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }
    }

    __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
    return 0;    
}
//post multipart/form-data
int8_t yLib::yHttp::PostMultiPart(const yHttpRequestParam &request_param_, yHttpResponseInfo & response_info_, std::vector<yHttpPostMultiPartItem> & form_multi_part_vec_){

    CURL * ptr_http_handle = NULL;
    struct curl_slist *http_headers = NULL;
    uint8_t *data_buffer_ptr_ = NULL;

    if (0 > __yhttp_check_protocol_type__(request_param_))
        return -1;

    //create http handle
    if (0 > __yhttp_create_http_handle__(&ptr_http_handle))
        return -1;

    //fill http header
    if (request_param_.http_header_vec.size() > 0)
        if (0 > __yhttp_parse_set_http_header__(ptr_http_handle, request_param_, http_headers)){

            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }

    //fill http url
    if (0 > __yhttp_parse_set_http_url__(ptr_http_handle, request_param_)){
            
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //set this session method is post
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_POST, 1) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        yLib::yLog::E("set http method type(post) failed.");
        return -1;
    }


    struct curl_httppost * _http_post = 0;
    struct curl_httppost * _last_post = 0;

    for(auto iter_ = form_multi_part_vec_.begin(); iter_ != form_multi_part_vec_.end(); iter_++){

        if (iter_->content_type != ""){//file multi part

            //curl_formfree
            CURLFORMcode _curl_form_add_code = curl_formadd(&_http_post, &_last_post, \
                        CURLFORM_COPYNAME, iter_->name.c_str(), \
                        CURLFORM_BUFFER,  iter_->filename.c_str(), \
                        CURLFORM_BUFFERPTR, iter_->data_buf.data(), \
                        CURLFORM_BUFFERLENGTH, iter_->data_buf.size(), \
                        CURLFORM_END);

            if ( CURL_FORMADD_OK !=  _curl_form_add_code){//error deal

                yLib::yLog::E("set http post muilt part data failed. error code %d, multipart idx %d", _curl_form_add_code, form_multi_part_vec_.end()-iter_);
                curl_formfree(_http_post);
                curl_slist_free_all(http_headers);
                curl_easy_cleanup(ptr_http_handle);
                return -1;
            }
        }
        else{//normal multi part

            //curl_formfree
            CURLFORMcode _curl_form_add_code = curl_formadd(&_http_post, &_last_post, \
                        CURLFORM_COPYNAME, iter_->name.c_str(), \
                        CURLFORM_PTRCONTENTS, iter_->data_buf.data(), \
                        CURLFORM_CONTENTSLENGTH, iter_->data_buf.size(), \
                        CURLFORM_END);

            if ( CURL_FORMADD_OK !=  _curl_form_add_code){//error deal

                yLib::yLog::E("set http post muilt part data failed. error code %d, multipart idx %d", _curl_form_add_code, form_multi_part_vec_.end()-iter_);
                curl_formfree(_http_post);
                curl_slist_free_all(http_headers);
                curl_easy_cleanup(ptr_http_handle);
                return -1;
            }
        }


        
    }

    
    if ( CURLE_OK != curl_easy_setopt(ptr_http_handle, CURLOPT_HTTPPOST, _http_post) ){

        curl_slist_free_all(http_headers);
        curl_easy_cleanup(ptr_http_handle);
        curl_formfree(_http_post);
        yLib::yLog::E("set http multi post part failed.");
        return -1;
    }

    //set response callback and buffer
    data_buffer_ptr_ = (uint8_t *)malloc(1024 * sizeof(uint8_t));
    if (0 > __yhttp_set_response_callback_buffer__(ptr_http_handle, &data_buffer_ptr_, 1024 * sizeof(uint8_t))){
        
        curl_formfree(_http_post);
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }

    //start http and process response
    if (0 > __yhttp_start_http_session__(ptr_http_handle))//start http failed.
    {
        curl_formfree(_http_post);
        __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
        return -1;
    }
    else//start http ok
    {
        if (0 > __yhttp_process_http_response__(ptr_http_handle, data_buffer_ptr_, response_info_)){
            
            curl_formfree(_http_post);
            __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
            return -1;
        }
    }
    curl_formfree(_http_post);
    __yhttp_cleanup__(ptr_http_handle, http_headers, data_buffer_ptr_);
    return 0;    
}
