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


using namespace yLib;
using namespace yLib::ycompiler;

static yLib::yConfigValue g_yconfig_valuedefault_obj;

yLib::yConfig::yConfig() 
MACRO_INIT_YOBJECT_PROPERTY(yConfig), 
compiler_instance(new (std::nothrow) ycompiler::yCompilerInstance())
{
    file_mgr = ycompiler::yFileManager::GetInstance();//this mgr deleted by ycompilerinstance
}
yLib::yConfig::~yConfig()
{
    if (file_mgr != nullptr)//if we don't move file-mgr ownership, we should delete it by hand.
        delete file_mgr;

    file_mgr = nullptr;
}

int8_t yLib::yConfig::ReadFile(const std::string &file_path){

    if (!file_mgr->InitFileManager(file_path)){

        yLib::yLog::E("yConfig", "open cfg file(%s) failed\n", file_path.c_str());
        return -1;
    }

    compiler_instance->SetFileManager(file_mgr);//we will move file_mgr to unique_ptr , we shouldn't delete it by hand.
    file_mgr = nullptr;//we must set it to nullptr to avoid double-free
    ycompiler::yConfigParseAction act(compiler_instance.get());

    if (!compiler_instance->ExecuteAction(act)){

        LOGE("yConfig")<<"Run yconfig parse action failed.";
        return -1;
    }

    return 0;
}



int8_t yLib::yConfig::WriteFile(const std::string &file_path_){


    return 0;
}

ycompiler::yConfigDecl * yLib::yConfig::LookUp(const std::string &node_path, ycompiler::yConfigDeclObject & decl_obj){

    std::string _tmp_node_path = node_path;
    if (_tmp_node_path != ""){
        
        std::size_t _find_pos = 0;
        if (_tmp_node_path.npos != (_find_pos = _tmp_node_path.find_first_of('.'))){

            std::string _cur_node_name = _tmp_node_path.substr(0, _find_pos);
            if (decl_obj.decl_map.count(_cur_node_name) > 0){//found a decl

                if (decl_obj.decl_map[_cur_node_name]->GetDeclType() == yConfigDecl::OBJECT_TYPE){//this is an obj type

                    std::string _new_node_path = _tmp_node_path.substr(_find_pos + 1);//skip '.'
                    return LookUp(_new_node_path, *((yConfigDeclObject*)decl_obj.decl_map[_cur_node_name]));
                }
                else{//invalid node name

                    return nullptr;
                }
            }
            else{

                return nullptr;
            }
        }
        else{//last name

            if (decl_obj.decl_map.count(_tmp_node_path) > 0){//found a decl

                return decl_obj.decl_map[_tmp_node_path];
            }
            else{

                return nullptr;
            }            
        }
    }
    return nullptr;
}

yLib::yConfigValue yLib::yConfig::GetValue(const std::string &node_path_) {

    yConfigDeclObject & _root_object = *(yConfigDeclObject*)compiler_instance->GetParser()->GetASTData();
    yConfigValue _tmpValue;

    yConfigDecl *_decl = LookUp(node_path_, _root_object);
    if (nullptr == _decl){

        yLib::yLog::E("Node(%s) is not found", node_path_.c_str());
        return std::move(_tmpValue);//gcc enable RVO(return value optimization) defaultly.
    }
  
    switch(_decl->GetDeclType()){

        case yConfigDecl::ITEM_TYPE:{

            yConfigDeclItem * _item = (yConfigDeclItem *)_decl;
            switch (_item->GetItemType())
            {
            case yConfigDeclItem::INT64T_TYPE:{
                /* code */
                _tmpValue = _item->GetInt64t();
                break;
            }
            case yConfigDeclItem::DOUBLE_TYPE:{
                /* code */
                _tmpValue = _item->GetDouble();
                break;
            }
            case yConfigDeclItem::STRING_TYPE:{
                /* code */
                _tmpValue = _item->GetStringLiteral();
                break;
            }
            case yConfigDeclItem::BOOL_TYPE:{
                /* code */
                _tmpValue = _item->GetBool();
                break;
            }
            default:
                yLib::yLog::E("yConfigValueType: Invalid value type(NONE_TYPE)(%s)", node_path_.c_str());
                return std::move(_tmpValue);//gcc enable RVO(return value optimization) defaultly.
                break;
            }
            break;
		}
        case yConfigDecl::OBJECT_TYPE:{

            _tmpValue = (uintptr_t)(_decl);
            break;
		}
        default :{
        
            yLib::yLog::E("yConfigValueType: Invalid value type(%s)", node_path_.c_str());
            return std::move(_tmpValue);//gcc enable RVO(return value optimization) defaultly.
        }  
    }

    return std::move(_tmpValue);////gcc enable RVO(return value optimization) defaultly.
}


int8_t yLib::yConfig::SetValue(const std::string &node_path_, const yConfigValue &value){


    return 0;
}


int8_t yLib::yConfig::AddNode(const std::string & pos_, const std::string & name_, yValue::yValueType type_, const yConfigValue &value){


    return 0;
}


//////////////////////////////////////////////////
///////////////////yConfigValue
//////////////////////////////////////////////////


yConfigValue::yConfigValue(int64_t value) noexcept
:yValue(value)
{

}

yLib::yConfigValue::yConfigValue(bool value) noexcept
:yValue(value)
{

}
yLib::yConfigValue::yConfigValue(double value) noexcept
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

yLib::yConfigValue::yConfigValue(uintptr_t ptr) noexcept
:yValue(yValue::OBJECT_TYPE, (void*)ptr)
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

yLib::yConfigValue & yLib::yConfigValue::operator=(int64_t value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}

yLib::yConfigValue & yLib::yConfigValue::operator=(bool value)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(value), *this);
    return (*this);
}
yLib::yConfigValue & yLib::yConfigValue::operator=(double value)noexcept{

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

yLib::yConfigValue & yLib::yConfigValue::operator=(uintptr_t ptr)noexcept{

    this->CleanAllToDefault();
    yLib::yBasicValue::CopyValueContainer(yValue(yValue::OBJECT_TYPE, (void*)ptr), *this);
    return (*this);
}

/**
 * @fn  operator int64_t() const
 * @brief convert yConfigValue to int64_t
 * @return return a int64_t's val from obj.
 */
yLib::yConfigValue::operator int64_t() const noexcept
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
 * @fn  operator double() const
 * @brief convert yConfigValue to double
 * @return return a double's val from obj.
 */
yLib::yConfigValue::operator double() const noexcept
{

    return value_containter.double_val;
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

yLib::yConfigValue::operator uintptr_t() const noexcept
{

    return value_containter.ptr_val;
}