#ifndef _YCONFIG_H_
#define _YCONFIG_H_

#include <string>
#include <libconfig/libconfig.h++>
#include "ylog.h"

namespace yLib{

    class yConfig{
        public:
            yConfig(){}
            ~yConfig(){}
            int yConfigReadFile(const std::string & file_path);
            int yConfigWriteFile(const std::string & file_path);
           // yConfigSetting & operator [](const char * setting_name);
            //int yConfigGetRootSetting(void);
            int yConfigGetIntValue(const char * node_path);
            bool yConfigGetBoolValue(const char * node_path);
            float yConfigGetFloatValue(const char * node_path);
            std::string yConfigGetStringValue(const char * node_path);
            
            // operator bool ();
            // operator int ();
            // operator std::string ();
            // operator float ();
            
        private:
            libconfig::Config m_config;



    };


}



#endif //_YCONFIG_H_