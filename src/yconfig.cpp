#include "yconfig.hpp"
#include <libconfig/libconfig.h++>

using namespace yLib;
yLib::yConfig::yConfig():m_config(* new libconfig::Config()){

}
yLib::yConfig::~yConfig(){

    delete & m_config;
}
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

yLib::yConfigValue yLib::yConfig::yConfigGetValue(const char * node_path){

    libconfig::Setting & root = m_config.getRoot();
    libconfig::Setting & setting_value = root.lookup(node_path);

    yConfigValue tmpValue;
    switch(setting_value.getType()){

        case libconfig::Setting::Type::TypeInt:
        tmpValue.value_int = (int)setting_value;
        break;

        case libconfig::Setting::Type::TypeBoolean:
        tmpValue.value_bool = (bool)setting_value;
        break;

        case libconfig::Setting::Type::TypeFloat:
        tmpValue.value_float = (float)setting_value;
        break;

        case libconfig::Setting::Type::TypeString:
        tmpValue.value_string = (const char *)setting_value;
        break;

        default :
            yLib::yLog::E("yConfigValueType Error");
            
    }
    return std::move(tmpValue);
}
yLib::yConfigValue yLib::yConfig::yConfigGetValue(const std::string & node_path){
    
    return yConfigGetValue(node_path.c_str()); 
}


int yLib::yConfig::yConfigSetValue(const char * node_path, const yLib::yConfigValue & value){

    if ( !m_config.exists(node_path) ){//node_path not found
        
        yLib::yLog::E("node_path is not found");
        return -1;
    }

    libconfig::Setting & root = m_config.getRoot();
    libconfig::Setting & setting_value = root.lookup(node_path);

    switch(value._current_data_type_){

        case yLib::yConfigValueType::INT_TYPE:
        setting_value = (int)value;
        break;

        case yLib::yConfigValueType::BOOL_TYPE:
        setting_value = (bool)value;
        break;

        case yLib::yConfigValueType::FLOAT_TYPE:
        setting_value = (float)value;
        break;

        case yLib::yConfigValueType::STRING_TYPE:
        setting_value = (std::string)value;
        break;

        default :
        yLib::yLog::E("value type not found");
        return -1;
        break;
    }

    return 0;
}
int yLib::yConfig::yConfigSetValue(const std::string & node_path, const yLib::yConfigValue & value){

    return yConfigSetValue(node_path.c_str(), value);
}


int yLib::yConfig::yConfigAddNode(const char * add_node_pos, const char * add_node_name, yConfigValueType add_node_type, const yConfigValue & value){

    libconfig::Setting & root = m_config.getRoot();

    if ( 0 == strcmp(add_node_pos, ".") ){//add node to root

        switch(add_node_type){

            case yLib::yConfigValueType::INT_TYPE:
            root.add( add_node_name, libconfig::Setting::Type::TypeInt) = (int)value;
            break;
            
            case yLib::yConfigValueType::BOOL_TYPE:
            root.add( add_node_name, libconfig::Setting::Type::TypeBoolean) = (bool)value;
            break;

            case yLib::yConfigValueType::FLOAT_TYPE:
            root.add( add_node_name, libconfig::Setting::Type::TypeFloat) = (float)value;
            break;

            case yLib::yConfigValueType::STRING_TYPE:
            root.add( add_node_name, libconfig::Setting::Type::TypeString) = (std::string)value;
            break;

            case yLib::yConfigValueType::GROUP_TYPE:
            root.add( add_node_name, libconfig::Setting::Type::TypeGroup);
            break;

            default :

            yLib::yLog::E("add_node_type not found");
            return -1;
            break;
        }
    }
    else{//not root node

        libconfig::Setting & lookup_node = root.lookup(add_node_pos);
        switch(add_node_type){

            case yLib::yConfigValueType::INT_TYPE:
            lookup_node.add( add_node_name, libconfig::Setting::Type::TypeInt) = (int)value;
            break;
            
            case yLib::yConfigValueType::BOOL_TYPE:
            lookup_node.add( add_node_name, libconfig::Setting::Type::TypeBoolean) = (bool)value;
            break;

            case yLib::yConfigValueType::FLOAT_TYPE:
            lookup_node.add( add_node_name, libconfig::Setting::Type::TypeFloat) = (float)value;
            break;

            case yLib::yConfigValueType::STRING_TYPE:
            lookup_node.add( add_node_name, libconfig::Setting::Type::TypeString) = (std::string)value;
            break;

            case yLib::yConfigValueType::GROUP_TYPE:
            lookup_node.add( add_node_name, libconfig::Setting::Type::TypeGroup);
            break;

            default :

            yLib::yLog::E("add_node_type not found");
            return -1;
            break;
        }
    }
    return 0;
}
int yLib::yConfig::yConfigAddNode(const std::string & add_node_pos, const std::string & add_node_name, yConfigValueType add_node_type, const yConfigValue & value){

    return yConfigAddNode(add_node_pos.c_str(), add_node_name.c_str(), add_node_type, value);
}


int yLib::yConfig::yConfigAddNode(const char * add_node_pos, const char * add_node_name, yConfigValueType add_node_type){

    yConfigValue tmpValue;
    
    return yConfigAddNode(add_node_pos, add_node_name, add_node_type, tmpValue);
}
int yLib::yConfig::yConfigAddNode(const std::string & add_node_pos, const std::string & add_node_name, yConfigValueType add_node_type){

    return yConfigAddNode(add_node_pos.c_str(), add_node_name.c_str(), add_node_type);
}

/*
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
*/

yLib::yConfigValue::operator int() const{

   return this->value_int;
}
yLib::yConfigValue::operator bool() const{

    return this->value_bool;
}
yLib::yConfigValue::operator float() const{

    return this->value_float;
}
yLib::yConfigValue::operator std::string() const{

    return this->value_string;
}
// yLib::yConfigValue::operator const char *() const{

//     return (char *)this->value_string.c_str();
// }


yLib::yConfigValue & yLib::yConfigValue::operator=(int value){

    this->_current_data_type_ = yConfigValueType::INT_TYPE;
    this->value_int = value;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(bool value){

    this->_current_data_type_ = yConfigValueType::BOOL_TYPE;
    this->value_bool = value;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(float value){

    this->_current_data_type_ = yConfigValueType::FLOAT_TYPE;
    this->value_float = value;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(std::string & value){

    this->_current_data_type_ = yConfigValueType::STRING_TYPE;
    this->value_string = value;
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(const char *  value){

    this->_current_data_type_ = yConfigValueType::STRING_TYPE;
    this->value_string = value;
    return (*this);
}