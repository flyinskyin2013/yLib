/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors: Sky
 * @LastEditTime: 2021-01-07 16:25:26
 * @Description: 
 */

#include "utility/yconfig.hpp"
#include <libconfig.h++>

using namespace yLib;

#define CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(pointer)\
    static_cast<libconfig::Config *>(config_instance)

static yLib::yConfigValue g_yconfig_value_default_obj;

yLib::yConfig::yConfig() 
MACRO_INIT_YOBJECT_PROPERTY(yConfig), 
config_instance(static_cast<void * >(new libconfig::Config()))
{
}
yLib::yConfig::~yConfig()
{
    delete CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance);
}

int8_t yLib::yConfig::ReadFile(const std::string &file_path_){

    try{

        CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->readFile(file_path_.c_str());
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



int8_t yLib::yConfig::WriteFile(const std::string &file_path_){

    try{

       CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->writeFile(file_path_.c_str());
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

yLib::yConfigValue yLib::yConfig::GetValue(const std::string &node_path_) {

    libconfig::Setting & _root = CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->getRoot();
    libconfig::Setting * _setting_value = nullptr;
    yConfigValue _tmpValue;

    try
    {
        _setting_value = &_root.lookup(node_path_.c_str());//search node
    }
    catch(...)//SettingNotFoundException 
    {

        yLib::yLog::E("Node(%s) is not found", node_path_.c_str());
        return std::move(_tmpValue);//gcc enable RVO(return value optimization) defaultly.
    }
    
    switch(_setting_value->getType()){

        case libconfig::Setting::Type::TypeInt:{

            _tmpValue = (int32_t)(*_setting_value);
            break;
		}
        case libconfig::Setting::Type::TypeBoolean:{

            _tmpValue = (bool)(*_setting_value);
            break;
		}
        case libconfig::Setting::Type::TypeFloat:{

            _tmpValue = (float)(*_setting_value);
            break;
		}
        case libconfig::Setting::Type::TypeString:{

            std::string _result_str = _setting_value->operator const char *();
            _tmpValue = _result_str;
            break;
		}
        default :{
        
            yLib::yLog::E("yConfigValueType: Invalid setting_value type(%s)", node_path_.c_str());
            throw "yConfigValueType Error";
        }
            
    }

    return std::move(_tmpValue);////gcc enable RVO(return value optimization) defaultly.
}


int8_t yLib::yConfig::SetValue(const std::string &node_path_, const yConfigValue &value_){

    if ( !CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->exists(node_path_.c_str()) ){//node_path not found
        
        yLib::yLog::E("node_path_(%s) is not found, please add node firstly.", node_path_.c_str());
        return -1;
    }

    libconfig::Setting & _root = CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->getRoot();
    libconfig::Setting & _setting_value = _root.lookup(node_path_.c_str());

    switch(value_.cur_value_type){

        case yLib::yValue::INT32_TYPE:{

            _setting_value = (int32_t)value_;
            break;
        }
        case yLib::yValue::BOOL_TYPE:{
            
            _setting_value = (bool)value_;
            break;
        }
        case yLib::yValue::FLOAT_TYPE:{

            _setting_value = (float)value_;
            break;
        }
        case yLib::yValue::STRING_TYPE:{

            _setting_value = (std::string)value_;
            break;
        }
        default :{

            yLib::yLog::E("Invalid value type.");
            return -1;
            break;
        }
        
    }

    return 0;
}


int8_t yLib::yConfig::AddNode(const std::string & pos_, const std::string & name_, yValue::yValueType type_, const yConfigValue &value_){

    libconfig::Setting & _root = CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->getRoot();

    if ( "." == pos_ || "" == pos_ ){//add node to root

        switch(type_){

            case yLib::yValue::INT32_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeInt) = (int32_t)value_;
                break;
            }
            case yLib::yValue::BOOL_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeBoolean) = (bool)value_;
                break;
            }
            case yLib::yValue::FLOAT_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeFloat) = (float)value_;
                break;
            }
            case yLib::yValue::STRING_TYPE:{
                
                _root.add(name_.c_str(), libconfig::Setting::Type::TypeString) = (std::string)value_;
                break;
            }
            case yLib::yValue::GROUP_TYPE:{
                
                _root.add(name_.c_str(), libconfig::Setting::Type::TypeGroup);
                break;
            }
            default :
            {
                yLib::yLog::E("Invalid value type.");
                return -1;
                break;
            }
        }
    }
    else{//not root node

        libconfig::Setting & _lookup_node = _root.lookup(pos_.c_str());//fix libconfig unexport Setting & Setting::operator=(const std::string &value) on windows
        switch(type_){

            case yLib::yValue::INT32_TYPE:{

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeInt) = (int32_t)value_;//
                break;
            }
            case yLib::yValue::BOOL_TYPE:{

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeBoolean) = (bool)value_;
                break;
            }
            case yLib::yValue::FLOAT_TYPE:{

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeFloat) = (float)value_;
                break;
            }
            case yLib::yValue::STRING_TYPE:{
                
                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeString) = (std::string)value_;
                break;
            }
            case yLib::yValue::GROUP_TYPE:{
                
                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeGroup);
                break;
            }
            default :{

                yLib::yLog::E("Invalid value type.");
                return -1;
                break;
            }
        }
    }
    return 0;
}


//////////////////////////////////////////////////
///////////////////yConfigValue
//////////////////////////////////////////////////
yConfigValue::yConfigValue() noexcept 
:yValue()
{
    object_name = YLIB_STR(yConfigValue);
}

yConfigValue::yConfigValue(int32_t value_) noexcept
:yValue(value_)
{
    object_name = YLIB_STR(yConfigValue);
}

yLib::yConfigValue::yConfigValue(bool value_) noexcept
:yValue(value_)
{
    object_name = YLIB_STR(yConfigValue);
}
yLib::yConfigValue::yConfigValue(float value_) noexcept
:yValue(value_)
{
    object_name = YLIB_STR(yConfigValue);
}

yLib::yConfigValue::yConfigValue(const std::string &value_) noexcept
:yValue(value_)
{
    object_name = YLIB_STR(yConfigValue);
}

yLib::yConfigValue::yConfigValue(const char *value_) noexcept
:yValue(value_)
{
    object_name = YLIB_STR(yConfigValue);
}

yLib::yConfigValue::yConfigValue(const yConfigValue &value_) noexcept
:yBasicValue()
{
    object_name = YLIB_STR(yConfigValue);
    yLib::yBasicValue::CopyValueContainer(value_, *this);
}


yLib::yConfigValue& yLib::yConfigValue::operator=(const yConfigValue & value_) noexcept
{
    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(value_, *this);

    return *this;
}

yLib::yConfigValue::yConfigValue(const yConfigValue &&value_) noexcept 
:yBasicValue()
{
    object_name = YLIB_STR(yConfigValue);
    yLib::yBasicValue::CopyValueContainer(value_, *this);
}
yLib::yConfigValue& yLib::yConfigValue::operator=(const yConfigValue &&value_) noexcept
{
    
    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(value_, *this);

    return *this;
}

yLib::yConfigValue & yLib::yConfigValue::operator=(int32_t value_){

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value_), *this);
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(bool value_){

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value_), *this);
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(float value_){

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value_), *this);
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(const std::string &value_){

    return operator=(value_.c_str());
}

yLib::yConfigValue & yLib::yConfigValue::operator=(const char *value_){

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value_), *this);
    return (*this);
}

/**
 * @fn  operator uint32_t() const
 * @brief convert yConfigValue to uint32_t
 * @return return a uint32_t's val from obj.
 */
yLib::yConfigValue::operator uint32_t() const
{
    return value_containter.uint32_val;
}

/**
 * @fn  operator bool() const
 * @brief convert yConfigValue to bool
 * @return return a bool's val from obj.
 */
yLib::yConfigValue::operator bool() const
{
    return value_containter.bool_val;
}


/**
 * @fn  operator float() const
 * @brief convert yConfigValue to float
 * @return return a float's val from obj.
 */
yLib::yConfigValue::operator float() const
{

    return value_containter.float_val;
}


/**
 * @fn  operator std::string() const
 * @brief convert yConfigValue to std::string
 * @return return a std::string's val from obj.
 */
yLib::yConfigValue::operator std::string() const
{

    return value_containter.string_val;
}