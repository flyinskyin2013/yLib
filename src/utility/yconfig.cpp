/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:53
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 14:51:38
 * @Description: 
 */

#include "utility/yconfig.hpp"
#include <libconfig.h++>

using namespace yLib;

#define CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(pointer)\
    static_cast<libconfig::Config *>(config_instance)

static yLib::yConfigValue g_yconfig_valuedefault_obj;

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


int8_t yLib::yConfig::SetValue(const std::string &node_path_, const yConfigValue &value){

    if ( !CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->exists(node_path_.c_str()) ){//node_path not found
        
        yLib::yLog::E("node_path_(%s) is not found, please add node firstly.", node_path_.c_str());
        return -1;
    }

    libconfig::Setting & _root = CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->getRoot();
    libconfig::Setting & _setting_value = _root.lookup(node_path_.c_str());

    switch(value.cur_value_type){

        case yLib::yValue::INT32_TYPE:{

            _setting_value = (int32_t)value;
            break;
        }
        case yLib::yValue::BOOL_TYPE:{
            
            _setting_value = (bool)value;
            break;
        }
        case yLib::yValue::FLOAT_TYPE:{

            _setting_value = (float)value;
            break;
        }
        case yLib::yValue::STRING_TYPE:{

            _setting_value = (std::string)value;
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


int8_t yLib::yConfig::AddNode(const std::string & pos_, const std::string & name_, yValue::yValueType type_, const yConfigValue &value){

    libconfig::Setting & _root = CONVERT_POINTER_TO_LIBCONFIG_CONFIG_INSTANCE(config_instance)->getRoot();

    if ( "." == pos_ || "" == pos_ ){//add node to root

        switch(type_){

            case yLib::yValue::INT32_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeInt) = (int32_t)value;
                break;
            }
            case yLib::yValue::BOOL_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeBoolean) = (bool)value;
                break;
            }
            case yLib::yValue::FLOAT_TYPE:{

                _root.add(name_.c_str(), libconfig::Setting::Type::TypeFloat) = (float)value;
                break;
            }
            case yLib::yValue::STRING_TYPE:{
                
                _root.add(name_.c_str(), libconfig::Setting::Type::TypeString) = (std::string)value;
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

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeInt) = (int32_t)value;//
                break;
            }
            case yLib::yValue::BOOL_TYPE:{

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeBoolean) = (bool)value;
                break;
            }
            case yLib::yValue::FLOAT_TYPE:{

                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeFloat) = (float)value;
                break;
            }
            case yLib::yValue::STRING_TYPE:{
                
                _lookup_node.add(name_.c_str(), libconfig::Setting::Type::TypeString) = (std::string)value;
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


yConfigValue::yConfigValue(int32_t value) noexcept
:yValue(value)
{

}

yLib::yConfigValue::yConfigValue(bool value) noexcept
:yValue(value)
{

}
yLib::yConfigValue::yConfigValue(float value) noexcept
:yValue(value)
{

}

yLib::yConfigValue::yConfigValue(const std::string &value) noexcept
:yValue(value)
{

}

yLib::yConfigValue::yConfigValue(const char *value) noexcept
:yValue(value)
{

}

yLib::yConfigValue::yConfigValue(const yConfigValue &value) noexcept
:yBasicValue()
{

    yLib::yBasicValue::CopyValueContainer(value, *this);
}


yLib::yConfigValue& yLib::yConfigValue::operator=(const yConfigValue & value) noexcept
{
    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(value, *this);

    return *this;
}

yLib::yConfigValue::yConfigValue(const yConfigValue &&value) noexcept 
:yBasicValue()
{

    yLib::yBasicValue::CopyValueContainer(value, *this);
}
yLib::yConfigValue& yLib::yConfigValue::operator=(const yConfigValue &&value) noexcept
{
    
    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(value, *this);

    return *this;
}

yLib::yConfigValue & yLib::yConfigValue::operator=(int32_t value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(bool value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(float value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(const std::string &value)noexcept{

    return operator=(value.c_str());
}

yLib::yConfigValue & yLib::yConfigValue::operator=(const char *value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}

/**
 * @fn  operator uint32_t() const
 * @brief convert yConfigValue to uint32_t
 * @return return a uint32_t's val from obj.
 */
yLib::yConfigValue::operator uint32_t() const noexcept
{
    return value_containter.int64_val;
}

/**
 * @fn  operator bool() const
 * @brief convert yConfigValue to bool
 * @return return a bool's val from obj.
 */
yLib::yConfigValue::operator bool() const noexcept
{
    return value_containter.bool_val;
}


/**
 * @fn  operator float() const
 * @brief convert yConfigValue to float
 * @return return a float's val from obj.
 */
yLib::yConfigValue::operator float() const noexcept
{

    return value_containter.float_val;
}


/**
 * @fn  operator std::string() const
 * @brief convert yConfigValue to std::string
 * @return return a std::string's val from obj.
 */
yLib::yConfigValue::operator std::string() const noexcept
{

    return value_containter.string_val;
}