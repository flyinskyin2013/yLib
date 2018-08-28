#ifndef _YCONFIG_H_
#define _YCONFIG_H_

#include <string>
#include <libconfig/libconfig.h++>
#include "ylog.h"

namespace yLib{

    //typedef libconfig::Setting yConfigSetting;
    class yConfigSetting;//declare

    class yConfig{
        public:
            yConfig(){}
            ~yConfig(){}
            int yConfigReadFile(const std::string & file_path);
            int yConfigWriteFile(const std::string & file_path);
           // yConfigSetting & operator [](const char * setting_name);
            int yConfigGetRootSetting(void);

            yConfigSetting * m_root_setting = nullptr;
        private:
        libconfig::Config m_config;
        
        
    };

    class yConfigSetting{
        public:
        yConfigSetting(const libconfig::Setting * root_setting);
        ~yConfigSetting(){}

    };

}



#endif //_YCONFIG_H_