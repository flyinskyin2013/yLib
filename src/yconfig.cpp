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
int yLib::yConfig::yConfigGetIntValue(const char * node_path){

    libconfig::Setting & root = m_config.getRoot();
    return (int) root.lookup(node_path);
}
bool yLib::yConfig::yConfigGetBoolValue(const char * node_path){

    libconfig::Setting & root = m_config.getRoot();
    return (bool) root.lookup(node_path);
}
float yLib::yConfig::yConfigGetFloatValue(const char * node_path){

    libconfig::Setting & root = m_config.getRoot();
    return (float) root.lookup(node_path);
}
std::string yLib::yConfig::yConfigGetStringValue(const char * node_path){
        
    libconfig::Setting & root = m_config.getRoot();
    return (const char *) root.lookup(node_path);
}