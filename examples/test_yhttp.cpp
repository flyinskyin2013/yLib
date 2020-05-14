/*
 * @Author: Sky
 * @Date: 2020-05-13 16:42:36
 * @LastEditors: Sky
 * @LastEditTime: 2020-05-14 11:36:45
 * @Description: 
 */
#include "ylib.hpp"
int main(int argc, char * argv[]){

        yLib::yHttp http;

        yLib::yHttpRequestParam req_p;
        yLib::yHttpResponseInfo res_p;
        
        req_p.protocol_type = "http";
        req_p.port = 5001;
        req_p.host = "192.168.31.43";
        req_p.path = "/optimize/logs";

        // http.yHttpGet(req_p, res_p);

        // yLib::yLog::I("Recv data size %d", res_p.response_data_buf.size());
        // res_p.response_data_buf.push_back(0x00);
        // yLib::yLog::I("Recv data is %s", &res_p.response_data_buf[0]);


        yLib::yHttpPostMultiPartItem car_num;
        yLib::yHttpPostMultiPartItem logfile;

        car_num.name = "carNumber";
        car_num.data_buf.reserve(4);
        car_num.data_buf.push_back(0x31);
        car_num.data_buf.push_back(0x32);
        car_num.data_buf.push_back(0x33);
        car_num.data_buf.push_back(0x34);

        logfile.name = "logfile";
        logfile.filename = "http_test_file.txt";
        logfile.content_type = yLib::yHttpPostMultiPartItem::GetTextFileMIMEType();
        logfile.data_buf.reserve(30);
        logfile.data_buf.resize(20);
        memcpy(logfile.data_buf.data(), "http_test_file.txt", 19);
        
        logfile.data_buf.push_back('\n');
        std::vector<yLib::yHttpPostMultiPartItem> multi_part_vec;
        multi_part_vec.push_back(car_num);
        multi_part_vec.push_back(logfile);
        http.yHttpPostMultiPart(req_p, res_p, multi_part_vec);

        yLib::yLog::I("Recv data size %d", res_p.response_data_buf.size());
        res_p.response_data_buf.push_back(0x00);
        yLib::yLog::I("Recv data is %s", &res_p.response_data_buf[0]);
        
        // REQUIRE_THAT( _exp0.what(), Catch::Equals ( "Basic exception of yLib." ));
        // REQUIRE_THAT( _exp1.what(), Catch::Equals ( "exception 1 msg" ));
}