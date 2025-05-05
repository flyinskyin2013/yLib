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


// import ycompiler
#include "core/ycompiler/frontend/ycompiler_instance.hpp"
#include "core/ycompiler/tools/yconfig_parse_action.hpp"
#include "core/ycompiler/basic/yfile_manager.hpp"
#include "core/ycompiler/parser/yconfig_parser.hpp"

#include "core/ycompiler/tools/yutils.hpp"

#include <memory>


#define GetCompilerInstanceRef(unique_ptr) (*((ycompiler::yCompilerInstance*)(unique_ptr.get())))
#define GetCompilerInstanceRefByRaw(raw_ptr) (*((ycompiler::yCompilerInstance*)(raw_ptr)))

static void compiler_instance_deleter(void * ci)
{

    delete ((yLib::ycompiler::yCompilerInstance*)ci);
}

using namespace yLib;
using namespace yLib::ycompiler;

static yLib::yConfigValue g_yconfig_valuedefault_obj;

yLib::yConfig::yConfig() 
MACRO_INIT_YOBJECT_PROPERTY(yConfig), 
compiler_instance((void *)new ycompiler::yCompilerInstance(), compiler_instance_deleter)
{}

yLib::yConfig::~yConfig()
{}

int8_t yLib::yConfig::ReadFile(const std::string &file_path){

    if (!GetCompilerInstanceRef(compiler_instance).GetFileManger().open_and_cache_file(file_path)){

        yLib::yLog::E("yConfig", "open cfg file(%s) failed\n", file_path.c_str());
        return -1;
    }

    // Create and execute the frontend action.
    // set action type
    GetCompilerInstanceRef(compiler_instance).GetCompilerInvocationHelper().getFrontendOpts().act_type = ycompiler::ActionKind::PARSE_YCONFIG_AST;

    std::unique_ptr<yFrontendAction> _act(CreateFrontendAction(GetCompilerInstanceRef(compiler_instance)));

    if (!GetCompilerInstanceRef(compiler_instance).ExecuteAction(*_act.get())){

        LOGE("yConfig")<<"Run yconfig parse action failed.";
        return -1;
    }

    return 0;
}



int8_t yLib::yConfig::WriteFile(const std::string &file_path_){


    return 0;
}

static yConfigValue ConvertDeclToConfigValue(yDecl * decl)
{

    yConfigValue _tmp_val;

    if (yDecl::Kind::ObjectDecl == decl->get_decl_kind()){

        _tmp_val = (uintptr_t)decl;
    }
    else if (yDecl::Kind::VarDecl == decl->get_decl_kind()){

        yVarDecl * _var_decl = (yVarDecl * )decl;
        yStmt * _stmt = _var_decl->get_init().stmt;

        switch (_stmt->get_stmt_kind())
        {
        case yStmt::StmtClass::IntegerLiteralClass:{
            /* code */
            _tmp_val = (int64_t)((yIntegerLiteral*)_stmt)->get_val();
            break;
        }
        case yStmt::StmtClass::FloatingLiteralClass:{
            /* code */
            _tmp_val = (double)((yFloatingLiteral*)_stmt)->get_val();
            break;
        }
        case yStmt::StmtClass::CXXBoolLiteralExprClass:{
            /* code */
            _tmp_val = (bool)((yCXXBoolLiteralExpr*)_stmt)->get_val();
            break;
        }
        case yStmt::StmtClass::StringLiteralClass:{
            /* code */
            _tmp_val = (std::string)((yStringLiteral*)_stmt)->get_val();
            break;
        }
        case yStmt::StmtClass::UnaryOperatorClass:{
            /* code */
            //we only support yIntegerLiteral/yFloatingLiteral, we check it in parser

            yUnaryOperator * _unary_op = (yUnaryOperator *)_stmt;
            
            yStmt * _expr_stmt = _unary_op->get_val();

            if (_unary_op->get_op_kind() == UO_INVALID){

                LOGE(yConfig)<<"ConvertDeclToConfigValue(): yUnaryOperator kind invalid";
                return _tmp_val;
            }

            int _sign = _unary_op->get_op_kind() == UO_Plus? 1 : -1;

            if (_expr_stmt->get_stmt_kind() == yStmt::StmtClass::IntegerLiteralClass){

                int64_t _val = ((yIntegerLiteral*)_expr_stmt)->get_val();
                
                if (-1 == _sign)
                    _val *= _sign;

                _tmp_val = _val;
            }
            else{

                double _val = ((yFloatingLiteral*)_expr_stmt)->get_val();
                _val *= _sign;

                _tmp_val = _val;
            }
            break;
        }
        default:
            break;
        }
    }
    if (yConfigValue::yValueType::NONE_TYPE == _tmp_val.GetType())
        yLib::yLog::E("ConvertDeclToConfigValue: expect a ObjectDecl or VarDecl\n");
    return _tmp_val;

}
static yDecl *  LookUp(yCompilerInstance & ci, const std::string &node_path, yDecl * parent = nullptr);
static yDecl *  LookUp(yCompilerInstance & ci, const std::string &node_path, yDecl * parent)
{
    yConfigASTReader &_cfg_ast_reader = (yConfigASTReader &)ci.GetSema().GetASTConsumer();
    std::string _tmp_node_path = node_path;
    yDecl * _tmp_decl = nullptr;


    if (_tmp_node_path != ""){
        
        std::size_t _find_pos = 0;
        if (std::string::npos != (_find_pos = _tmp_node_path.find_first_of('.'))){//not basic-name

            std::string _cur_node_name = _tmp_node_path.substr(0, _find_pos);
            _tmp_decl = _cfg_ast_reader.GetDecl(_cur_node_name, parent);
            if (nullptr != _tmp_decl){//found a decl

                if (_tmp_decl->get_decl_kind() == yDecl::Kind::ObjectDecl){//this is an obj type

                    std::string _new_node_path = _tmp_node_path.substr(_find_pos + 1);//skip '.'
                    
                    return LookUp(ci, _new_node_path, _tmp_decl);
                }
                else{//invalid node name

                    yLib::yLog::E("node(%s) is not object.\n", _cur_node_name.c_str());
                    return nullptr;
                }
            }
            else{
                
                yLib::yLog::E("can't found node(%s).\n", _cur_node_name.c_str());
                return nullptr;
            }
        }
        else{//last name

            _tmp_decl = _cfg_ast_reader.GetDecl(_tmp_node_path, parent);
            if (nullptr != _tmp_decl){//found a decl

                return _tmp_decl;
            }
            else{
                
                yLib::yLog::E("can't found node(%s).\n", _tmp_node_path.c_str());
                return nullptr;
            }            
        }
    }

    return nullptr;
}


yLib::yConfigValue yLib::yConfig::GetValue(const std::string &node_path_) {
    //problem: moving a local object in a return statement prevents copy elision
    //Automatic RVO:
    /*
    //C++11_ISOIEC 14882-2011_n3242 12.8 $32
        in a return statement in a function with a class return type, when the expression is the name of a
        non-volatile automatic object (other than a function or catch-clause parameter) with the same cv unqualified type as the function return type, the copy/move operation can be omitted by constructing
        the automatic object directly into the function’s return value
    */
    yConfigValue _tmpValue = ConvertDeclToConfigValue(LookUp(GetCompilerInstanceRef(compiler_instance), node_path_));
    
    if (yConfigValue::NONE_TYPE == _tmpValue.GetType()){

        yLib::yLog::E("Node(%s) is not found", node_path_.c_str());
        return _tmpValue;//gcc enable RVO(return value optimization) defaultly.
    }

    return _tmpValue;////gcc enable RVO(return value optimization) defaultly.
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