/*
 * @Author: Sky
 * @Date: 2021-09-03 16:26:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 13:20:04
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <string>
#include <iostream>


TEST_CASE( "Test yLog for backup-log-file" , "[yLog_backup-log-file]" ){

    SECTION( "yLog yLogBackupTest" ) {

        yLib::yLogTagProperty _prop;
        _prop.is_log_to_stdio = false;
        _prop.is_log_to_file = true;
        _prop.file_param.file_base_name = "yLogBackupTest";
        _prop.file_param.log_file_max_backup_num = 5;
        _prop.file_param.log_file_max_size = 50;
        _prop.file_param.multi_log_file = true;
        yLib::yLog::Init(_prop, "");//set default tag prop

        // 47 + 5
        LOGI()<<"1111";

        LOGI()<<"22222";//yLogBackupTest.log.bak1
        
        LOGW()<<"333333";//yLogBackupTest.log.bak2

        LOGW()<<"4444444";//yLogBackupTest.log.bak3

        LOGD()<<"55555555";//yLogBackupTest.log.bak4

        LOGD()<<"666666666";//yLogBackupTest.log.bak5

        LOGE()<<"7777777777";//yLogBackupTest.log.bak5, rm "1111"

        LOGE()<<"88888888888";//yLogBackupTest.log.bak5, rm "22222"

#ifdef _WIN32
        //avoid log-thread break by other thread
        Sleep(1000 * 2);
#endif //_WIN32
    }
}