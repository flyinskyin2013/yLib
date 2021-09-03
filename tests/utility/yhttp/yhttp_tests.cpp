/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 15:58:29
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:17:01
 * @FilePath: \yLib\tests\yhttp\yhttp_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yHttp)


TEST_CASE( "Test yHttp apis" , "[yHttp_Apis]" ){

    SECTION("tmp test") {

        yLib::yHttp http_get;
        yLib::yHttp http_post;

        yLib::yHttpRequestParam req_p;
        yLib::yHttpResponseInfo res_p;

        yLib::yHttpPostMultiPartItem _post_item;
        yLib::yHttpRequestParam req_post;
        yLib::yHttpResponseInfo res_post;


        
        req_p.protocol_type = "http";
        req_p.port = 80;
        req_p.host = "httpbin.org";
        req_p.path = "/get";

        REQUIRE(0 == http_get.Get(req_p, res_p));

        yLib::yLog::I("Recv data size %d", res_p.response_data_buf.size());
        res_p.response_data_buf.push_back(0x00);
        yLib::yLog::I("Recv data is %s", &res_p.response_data_buf[0]);


        req_post.protocol_type = "http";
        req_post.port = 80;
        req_post.host = "httpbin.org";
        req_post.path = "/post";

        std::vector<yLib::yHttpPostUrlencodedItem> _item_vec;

        yLib::yHttpPostUrlencodedItem _item;
        _item.key = "abc";
        _item.value = "123"; 

        _item_vec.push_back(_item);

        REQUIRE(0 == http_post.PostDefault(req_post, res_post, _item_vec));
        yLib::yLog::I("PostRecv data size %d", res_post.response_data_buf.size());
        res_post.response_data_buf.push_back(0x00);
        yLib::yLog::I("PostRecv data is %s", &res_post.response_data_buf[0]);


        REQUIRE(0 == http_post.PostJson(req_post, res_post, "{\"test_json\":\"test_json\"}"));
        yLib::yLog::I("PostRecv data size %d", res_post.response_data_buf.size());
        res_post.response_data_buf.push_back(0x00);
        yLib::yLog::I("PostRecv data is %s", &res_post.response_data_buf[0]);

        yLib::yHttpPostMultiPartItem _normal_item;
        yLib::yHttpPostMultiPartItem _bin_file;
        yLib::yHttpPostMultiPartItem _jpg_file;
        yLib::yHttpPostMultiPartItem _png_file;
        yLib::yHttpPostMultiPartItem _text_file;

        // normal item
        _normal_item.data_buf.reserve(4);
        _normal_item.data_buf.push_back(0x31);
        _normal_item.data_buf.push_back(0x32);
        _normal_item.data_buf.push_back(0x33);
        _normal_item.data_buf.push_back(0x34);


        // other item
        _bin_file.name = "binfile";
        _bin_file.filename = "binfile.txt";
        _bin_file.content_type = yLib::yHttpPostMultiPartItem::GetBinFileMIMEType();
        _bin_file.data_buf.reserve(4);
        _bin_file.data_buf.push_back(0x31);
        _bin_file.data_buf.push_back(0x32);
        _bin_file.data_buf.push_back(0x33);
        _bin_file.data_buf.push_back(0x34);

        _jpg_file.name = "jpgfile";
        _jpg_file.filename = "jpgfile.jpg";
        _jpg_file.content_type = yLib::yHttpPostMultiPartItem::GetJpgFileMIMEType();
        _jpg_file.data_buf.reserve(4);
        _jpg_file.data_buf.push_back(0x31);
        _jpg_file.data_buf.push_back(0x32);
        _jpg_file.data_buf.push_back(0x33);
        _jpg_file.data_buf.push_back(0x34);

        _png_file.name = "pngfile";
        _png_file.filename = "pngfile.png";
        _png_file.content_type = yLib::yHttpPostMultiPartItem::GetPngFileMIMEType();
        _png_file.data_buf.reserve(4);
        _png_file.data_buf.push_back(0x31);
        _png_file.data_buf.push_back(0x32);
        _png_file.data_buf.push_back(0x33);
        _png_file.data_buf.push_back(0x34);

        _text_file.name = "txtfile";
        _text_file.filename = "http_test_file.txt";
        _text_file.content_type = yLib::yHttpPostMultiPartItem::GetTextFileMIMEType();
        _text_file.data_buf.reserve(30);
        _text_file.data_buf.resize(20);
        memcpy(_text_file.data_buf.data(), "http_test_file.txt", 19);
        _text_file.data_buf.push_back('\n');



        std::vector<yLib::yHttpPostMultiPartItem> _post_item_vec;
        _post_item_vec.push_back(_normal_item);
        _post_item_vec.push_back(_bin_file);
        _post_item_vec.push_back(_jpg_file);
        _post_item_vec.push_back(_png_file);
        _post_item_vec.push_back(_text_file);


        REQUIRE(0 == http_post.PostMultiPart(req_post, res_post, _post_item_vec));
        yLib::yLog::I("PostRecv data size %d", res_post.response_data_buf.size());
        res_post.response_data_buf.push_back(0x00);
        yLib::yLog::I("PostRecv data is %s", &res_post.response_data_buf[0]);
    }
}