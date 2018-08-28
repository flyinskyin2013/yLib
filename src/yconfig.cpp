#include "yconfig.h"

int yLib::yConfig::yConfigReadFile(const std::string & file_path){

    try{

        m_config.readFile(file_path.c_str());
       
    }
    catch(libconfig::FileIOException e){

        yLib::yLog::E("yConfigReadFile FileIOException : %s", e.what());
        return -1;
    }
    catch(libconfig::ParseException e){

        yLib::yLog::E("yConfigReadFile ParseException : %s", e.what());
        return -1;
    }
    catch(...){

        yLib::yLog::E("yConfigReadFile UNKOWN Exception : %s");
        return -1;
    }
    
    return 0;
}

int yLib::yConfig::yConfigWriteFile(const std::string & file_path){

    try{

        m_config.writeFile(file_path.c_str());
    }
    catch(libconfig::FileIOException e){

        yLib::yLog::E("yConfigWriteFile FileIOException : %s", e.what());
        return -1;
    }
    catch(libconfig::ParseException e){

        yLib::yLog::E("yConfigWriteFile ParseException : %s", e.what());
        return -1;
    }
    catch(...){

        yLib::yLog::E("yConfigWriteFile UNKOWN Exception : %s");
        return -1;
    }
    return 0;
}
yLib::yConfigSetting & yLib::yConfig::yConfigGetRootSetting(void){

   // m_root_setting = yConfigSetting(m_config.getRoot());
    
}
// yLib::yConfigSetting & yLib::yConfig::operator [](const char * setting_name){

//     libconfig::Setting & root = m_config.getRoot();
//     return  root[setting_name];
// }


yLib::yConfigSetting::yConfigSetting(const libconfig::Setting * root_setting){

}