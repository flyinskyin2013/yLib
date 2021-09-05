/*
 * @Author: Sky
 * @Date: 2021-09-03 15:19:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 13:18:17
 * @Description: 
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <string>
#include <iostream>


TEST_CASE( "Test yLog log-size" , "[yLog_LogSize]" ){

    SECTION( "yLog::Verify multi-log-size" ) {

        // open logfile and calculate size
        std::ifstream _logfile0("LogFile.log");
        std::ifstream _logfile1("LogFile_test_ylog0.log");
        std::ifstream _logfile2("LogFile_test_ylog123.log");

        // check log-file is create ready
        REQUIRE(true == _logfile0.is_open());
        REQUIRE(true == _logfile1.is_open());
        REQUIRE(true == _logfile2.is_open());

        //check log-file
        _logfile0.seekg(0, _logfile0.end);
        uint64_t _logfile0_size = _logfile0.tellg();

        _logfile1.seekg(0, _logfile1.end);
        uint64_t _logfile1_size = _logfile1.tellg();

        _logfile2.seekg(0, _logfile2.end);
        uint64_t _logfile2_size = _logfile2.tellg();

        _logfile0.close();
        _logfile1.close();
        _logfile2.close();

        #ifdef _WIN32
        REQUIRE(_logfile0_size == 66328);
        REQUIRE(_logfile1_size == 32898);
        REQUIRE(_logfile2_size == 32902);//CRLF
        #elif __linux__ || __linux
        REQUIRE(_logfile0_size == 66308);
        REQUIRE(_logfile1_size == 32892);
        REQUIRE(_logfile2_size == 32892 + 4);//LF
        #elif __unix__ || __unix
        #endif //__unix__ || __unix

    }


    SECTION( "yLog::Verify backup-log-file-size" ) {

        int _bak_file_num = 5;
        std::ifstream _in_file[5];

        std::string _str;
        char _str_buf[100];
        std::string _real_str_array[] = {

            "7777777777",
            "666666666",
            "55555555",
            "4444444",
            "333333",
            "88888888888",
        };
                
        for(int _i = 0; _i < _bak_file_num; _i ++){

            _in_file[_i].open(std::string("yLogBackupTest.log.bak") + std::to_string(_i + 1));
            REQUIRE(_in_file[_i].is_open() == true);

            ::memset(_str_buf, 0, 100);
            _in_file[_i].getline(_str_buf, 100);
            _str = _str_buf;
            REQUIRE(_str.length() == _real_str_array[_i].length() + 47);
            REQUIRE(_str.find(_real_str_array[_i]) != std::string::npos);
        }


        std::ifstream _in_file_org;
        _in_file_org.open(std::string("yLogBackupTest.log"));

        REQUIRE(_in_file_org.is_open() == true);

        ::memset(_str_buf, 0, 100);
        _in_file_org.getline(_str_buf, 100);
        _str = _str_buf;
        REQUIRE(_str.length() == _real_str_array[5].length() + 47);
        REQUIRE(_str.find("88888888888") != std::string::npos);

    }
}
