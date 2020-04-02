/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-30 11:30:09
 * @Description: 
 */

#include "yconfig.hpp"
#include <libconfig.h++>

using namespace yLib;
yLib::yConfig::yConfig() 
MACRO_INIT_YOBJECT_PROPERTY(yConfig), 
_libconfig_config(*new libconfig::Config()){

}
yLib::yConfig::~yConfig(){

    delete & _libconfig_config;
}
int yLib::yConfig::yConfigReadFile(const std::string & file_path){

    try{

        _libconfig_config.readFile(file_path.c_str());
       
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

        _libconfig_config.writeFile(file_path.c_str());
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

yLib::yConfigValue yLib::yConfig::yConfigGetValue(const char * node_path) {

    libconfig::Setting & root = _libconfig_config.getRoot();
    libconfig::Setting * setting_value = nullptr;
    yConfigValue tmpValue;
    try
    {
        setting_value = &root.lookup(node_path);
    }
    catch(...)//SettingNotFoundException 
    {

        yLib::yLog::E("node(%s) is not found", node_path);
        return std::move(tmpValue);//gcc enable RVO(return value optimization) defaultly.
    }
    
    switch(setting_value->getType()){

        case libconfig::Setting::Type::TypeInt:{

            tmpValue._value_containter._n_value_int64 = (int)*setting_value;
            tmpValue._cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
            tmpValue._current_data_type_ = yLib::yConfigValueType::INT_TYPE;
            break;
		}
        case libconfig::Setting::Type::TypeBoolean:{

            tmpValue._value_containter._b_value_bool = (bool)*setting_value;
            tmpValue._cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
            tmpValue._current_data_type_ = yLib::yConfigValueType::BOOL_TYPE;
            break;
		}
        case libconfig::Setting::Type::TypeFloat:{

            tmpValue._value_containter._f_value_float = (float)*setting_value;
            tmpValue._cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
            tmpValue._current_data_type_ = yLib::yConfigValueType::FLOAT_TYPE;
            break;
		}
        case libconfig::Setting::Type::TypeString:{

            tmpValue._value_containter._str_value_string = (const char * )*setting_value;
            tmpValue._cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
            tmpValue._current_data_type_ = yLib::yConfigValueType::STRING_TYPE;
            break;
		}
        default :{
        
            yLib::yLog::E("yConfigValueType: Invalid setting_value type error");
            throw "yConfigValueType Error";
        }
            
    }

    return std::move(tmpValue);////gcc enable RVO(return value optimization) defaultly.
}
yLib::yConfigValue yLib::yConfig::yConfigGetValue(const std::string & node_path){
    
    return yConfigGetValue(node_path.c_str()); 
}


int yLib::yConfig::yConfigSetValue(const char * node_path, const yLib::yConfigValue & value){

    if ( !_libconfig_config.exists(node_path) ){//node_path not found
        
        yLib::yLog::E("node_path is not found");
        return -1;
    }

    libconfig::Setting & root = _libconfig_config.getRoot();
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

    libconfig::Setting & root = _libconfig_config.getRoot();

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







yLib::yConfigValue::yConfigValue(int64_t value_) noexcept{
    
    _cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE ;
    _value_containter._n_value_int64 = value_;
}
yLib::yConfigValue::yConfigValue(uint64_t value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::UINT64_YBASICVALUE_TYPE ;
    _value_containter._n_value_uint64 = value_;
}
yLib::yConfigValue::yConfigValue(bool value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE ;
    _value_containter._b_value_bool = value_;
}
yLib::yConfigValue::yConfigValue(float value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE ;
    _value_containter._f_value_float = value_;
}
yLib::yConfigValue::yConfigValue(double value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE ;
    _value_containter._f_value_double = value_;
}
yLib::yConfigValue::yConfigValue(std::string & value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE ;
    _value_containter._str_value_string = value_;
}
yLib::yConfigValue::yConfigValue(const char * value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE ;
    _value_containter._str_value_string = value_;
}









yLib::yConfigValue::yConfigValue(const yConfigValue & config_value) noexcept
: yBasicValue()
{

    _object_name = "yConfigValue";

    this->_current_data_type_ = config_value._current_data_type_;

    switch(this->_current_data_type_){

        case yLib::yConfigValueType::INT_TYPE:{
            
            this->_value_containter._n_value_int64 = config_value._value_containter._n_value_int64;
            this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::BOOL_TYPE:{

            this->_value_containter._b_value_bool = config_value._value_containter._b_value_bool;
            this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::FLOAT_TYPE:{

            this->_value_containter._f_value_float = config_value._value_containter._f_value_float;
            this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::STRING_TYPE:{

            this->_value_containter._str_value_string = config_value._value_containter._str_value_string;
            this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::GROUP_TYPE:{
            //not support type
            break;
        }
        case yLib::yConfigValueType::NONE_TYPE:{
            //no value
            break;
        }
        default :{
            yLib::yLog::E("Copy-Construct of yConfigValue failed: data type error.");
            break;
        }
    }
}


yLib::yConfigValue & yLib::yConfigValue::operator=(const yConfigValue & config_value) noexcept{

    _object_name = "yConfigValue";

    this->_current_data_type_ = config_value._current_data_type_;

    switch(this->_current_data_type_){

        case yLib::yConfigValueType::INT_TYPE:{
            
            this->_value_containter._n_value_int64 = config_value._value_containter._n_value_int64;
            this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::BOOL_TYPE:{

            this->_value_containter._b_value_bool = config_value._value_containter._b_value_bool;
            this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::FLOAT_TYPE:{

            this->_value_containter._f_value_float = config_value._value_containter._f_value_float;
            this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::STRING_TYPE:{

            this->_value_containter._str_value_string = config_value._value_containter._str_value_string;
            this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::GROUP_TYPE:{
            //not support type
            break;
        }
        case yLib::yConfigValueType::NONE_TYPE:{
            //no value
            break;
        }
        default :{
            yLib::yLog::E("Copy-Construct of yConfigValue failed: data type error.");
            break;
        }
    }
    return *this;
}

yLib::yConfigValue::yConfigValue(yConfigValue && config_value) noexcept 
: yBasicValue()
{
    _object_name = "yConfigValue";

    this->_current_data_type_ = config_value._current_data_type_;

    switch(this->_current_data_type_){

        case yLib::yConfigValueType::INT_TYPE:{
            
            this->_value_containter._n_value_int64 = config_value._value_containter._n_value_int64;
            this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::BOOL_TYPE:{

            this->_value_containter._b_value_bool = config_value._value_containter._b_value_bool;
            this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::FLOAT_TYPE:{

            this->_value_containter._f_value_float = config_value._value_containter._f_value_float;
            this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::STRING_TYPE:{

            this->_value_containter._str_value_string = config_value._value_containter._str_value_string;
            this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::GROUP_TYPE:{
            //not support type
            break;
        }
        case yLib::yConfigValueType::NONE_TYPE:{
            //no value
            break;
        }
        default :{
            yLib::yLog::E("Move-Construct of yConfigValue failed: data type error.");
            break;
        }
    }
}
yLib::yConfigValue & yLib::yConfigValue::operator=(yConfigValue && config_value) noexcept{

    _object_name = "yConfigValue";

    this->_current_data_type_ = config_value._current_data_type_;

    switch(this->_current_data_type_){

        case yLib::yConfigValueType::INT_TYPE:{
            
            this->_value_containter._n_value_int64 = config_value._value_containter._n_value_int64;
            this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::BOOL_TYPE:{

            this->_value_containter._b_value_bool = config_value._value_containter._b_value_bool;
            this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::FLOAT_TYPE:{

            this->_value_containter._f_value_float = config_value._value_containter._f_value_float;
            this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::STRING_TYPE:{

            this->_value_containter._str_value_string = config_value._value_containter._str_value_string;
            this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
            break;
        }
        case yLib::yConfigValueType::GROUP_TYPE:{
            //not support type
            break;
        }
        case yLib::yConfigValueType::NONE_TYPE:{
            //no value
            break;
        }
        default :{
            yLib::yLog::E("Move-Construct of yConfigValue failed: data type error.");
            break;
        }
    }

    return *this;
}
/* merge to ybasicvalue
yLib::yConfigValue::operator int() const{

   return this->_n_value_int;
}
yLib::yConfigValue::operator bool() const{

    return this->_b_value_bool;
}
yLib::yConfigValue::operator float() const{

    return this->_f_value_float;
}
yLib::yConfigValue::operator std::string() const{

    return this->_str_value_string;
}
*/
// yLib::yConfigValue::operator const char *() const{

//     return (char *)this->value_string.c_str();
// }

yLib::yConfigValueType yLib::yConfigValue::GetyConfigValueType(void) const {
	
	return _current_data_type_;
}

yLib::yConfigValue & yLib::yConfigValue::operator=(int value){

    this->_current_data_type_ = yConfigValueType::INT_TYPE;
    this->_value_containter._n_value_int64 = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(bool value){

    this->_current_data_type_ = yConfigValueType::BOOL_TYPE;
    this->_value_containter._b_value_bool = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(float value){

    this->_current_data_type_ = yConfigValueType::FLOAT_TYPE;
    this->_value_containter._f_value_float = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(std::string & value){

    this->_current_data_type_ = yConfigValueType::STRING_TYPE;
    this->_value_containter._str_value_string = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(std::string && value){

    this->_current_data_type_ = yConfigValueType::STRING_TYPE;
    this->_value_containter._str_value_string = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(const char *  value){

    this->_current_data_type_ = yConfigValueType::STRING_TYPE;
    this->_value_containter._str_value_string = value;
    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    return (*this);
}