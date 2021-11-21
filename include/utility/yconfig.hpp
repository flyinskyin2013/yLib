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
 * @Date: 2019-07-04 11:28:52
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 14:47:13
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YCONFIG_HPP__
#define __YLIB_UTILITY_YCONFIG_HPP__

#include <string>
#include <memory>

#include "core/ylog.hpp"
#include "core/ycommon.hpp"
#include "core/ybasicvalue.hpp"

// import ycompiler
#include "core/ycompiler/basic/ycompiler_instance.hpp"
#include "core/ycompiler/tools/yconfig_parse_action.hpp"
#include "core/ycompiler/basic/yfile_manager.hpp"
#include "core/ycompiler/parse/yconfig_parser.hpp"

namespace yLib{

    /**
     *  @class yConfigValue
     *  @brief This is the yConfig's value-container, it inherits from yValue.
     */
    class __YLIB_CLASS_DECLSPEC__ yConfigValue : public yBasicValue{
        protected:

        public:
        yConfigValue() noexcept {}

        //other constructor
        /**
         * @fn  explicit yConfigValue(int64_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(int64_t value) noexcept;

        /**
         * @fn  explicit yConfigValue(bool value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(bool value) noexcept;

        /**
         * @fn  explicit yConfigValue(double value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(double value) noexcept;

        /**
         * @fn  explicit yConfigValue(const std::string &value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(const std::string &value) noexcept;
        
        /**
         * @fn  explicit yConfigValue(const char *value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(const char *value) noexcept;

        /**
         * @fn  explicit yConfigValue(uintptr_t ptr) noexcept;
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yConfigValue(uintptr_t ptr) noexcept;

        /**
         * @fn  yConfigValue(const yConfigValue &value)
         * @brief copy constructor
         * @param value a exsited obj
         * @return 
         */
        yConfigValue(const yConfigValue &value) noexcept;
        /**
         * @fn  yConfigValue & operator=(const yConfigValue &value) noexcept
         * @brief assignment constructor
         * @param value a exsited obj
         * @return the obj's reference of yConfigValue
         */
        yConfigValue & operator=(const yConfigValue &value) noexcept;

        /**
         * @fn  yConfigValue(const yConfigValue &&value) noexcept
         * @brief move constructor
         * @param value a exsited obj
         * @return 
         */
        yConfigValue(const yConfigValue &&value) noexcept;
        /**
         * @fn  yConfigValue & operator=(const yConfigValue &&value) noexcept
         * @brief assignment constructor
         * @param value a exsited obj
         * @return the obj's reference of yConfigValue
         */
        yConfigValue & operator=(const yConfigValue &&value) noexcept;
        
        /**
         * @fn  yConfigValue & operator=(int64_t value_)
         * @brief convert int64_t to exsited obj of yConfigValue
         * @param value int64_t value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(int64_t value) noexcept;

        /**
         * @fn  yConfigValue & operator=(bool value)
         * @brief convert bool to exsited obj of yConfigValue
         * @param value bool value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(bool value) noexcept;

        /**
         * @fn  yConfigValue & operator=(double value)
         * @brief convert double to exsited obj of yConfigValue
         * @param value bool value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(double value) noexcept;

        /**
         * @fn  yConfigValue & operator=(const std::string & value)
         * @brief convert std::string & to exsited obj of yConfigValue
         * @param value std::string value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(const std::string &value) noexcept;

        /**
         * @fn  yConfigValue & operator=(const char *value)
         * @brief convert char * to exsited obj of yConfigValue
         * @param value std::string value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(const char *value) noexcept;

        yConfigValue & operator=(uintptr_t ptr) noexcept;
            
        /**
         * @fn  operator int64_t() const
         * @brief convert yConfigValue to int64_t
         * @return return a int64_t's val from obj.
         */
        operator int64_t() const noexcept override;

        /**
         * @fn  operator bool() const
         * @brief convert yConfigValue to bool
         * @return return a bool's val from obj.
         */
        operator bool() const noexcept override;


        /**
         * @fn  operator double() const
         * @brief convert yConfigValue to double
         * @return return a double's val from obj.
         */
        operator double() const noexcept override;


        /**
         * @fn  operator std::string() const
         * @brief convert yConfigValue to std::string
         * @return return a std::string's val from obj.
         */
        operator std::string() const noexcept override;

        operator uintptr_t() const noexcept override;

        friend class yConfig;

        private:

    };

    /**
     *  @class yConfig
     *  @brief This is a tool to parse a cfg-file that is like c.
     */
    class __YLIB_CLASS_DECLSPEC__ yConfig MACRO_PUBLIC_INHERIT_YOBJECT{
        public:
        yConfig();
        ~yConfig();
        /**
         * @fn  yConfig(const yConfig &config) = delete;
         * @brief copy constructor
         * @param config a exsited obj
         * @return 
         * @warning this op is disable.
         */
        yConfig(const yConfig &config) = delete;
        /**
         * @fn  yConfig & operator=(const yConfig &config_) = delete
         * @brief assignment func
         * @param config a exsited obj
         * @return the obj of yConfig
         * @warning this op is disable.
         */
        yConfig & operator=(const yConfig &config) = delete;


        /**
         * @fn yConfig(const yConfig &&config_) = delete
         * @brief move constructor
         * @param config a exsited obj
         * @return 
         * @warning this op is disable.
         */
        yConfig(const yConfig &&config) = delete;
        /**
         * @fn  yConfig & operator=(const yConfig &&config) = delete
         * @brief assignment func
         * @param config a exsited obj
         * @return the obj of yConfig
         * @warning this op is disable.
         */
        yConfig & operator=(const yConfig &&config) = delete;

        /**
         * @fn  int8_t ReadFile(const std::string & file_path)
         * @brief read cfg-file from disk.
         * @param file_path the path of cfg-file 
         * @return the op status
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t ReadFile(const std::string & file_path);

        /**
         * @fn  int8_t WriteFile(const std::string & file_path)
         * @brief write cfg-file to disk.
         * @param file_path the path of cfg-file 
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t WriteFile(const std::string & file_path);

        /**
         * @fn  yConfigValue GetValue(const std::string &node_path)
         * @brief get value by node-path.
         * @param node_path the path of node.
         * @return the value of node-path.
         * @warning 
         */        
        yConfigValue GetValue(const std::string &node_path);
        
        //set value interface
        /**
         * @fn  int SetValue(const std::string &node_path, const yConfigValue & value)
         * @brief get value by node-path.
         * @param node_path the path of node.
         * @param value the value to set.
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */        
        int8_t SetValue(const std::string &node_path, const yConfigValue & value);

        //add node interface
        //if add_node_pos == '.', we will add node to root
        /**
         * @fn  int SetValue(const std::string &node_path_, const yConfigValue & value)
         * @brief get value by node-path.
         * @param pos the position of node.
         * @param name the name of node.
         * @param type the type of value.we only support int32_t/bool/float/std::string/group.
         * @param value the value to set.
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning if pos_ == '.' or "", we will add node to root
         */      
        int8_t AddNode(const std::string & pos, const std::string & name, yValue::yValueType type, const yConfigValue &value = yConfigValue());

        private:
        ycompiler::yConfigDecl *LookUp(const std::string &node_path, ycompiler::yConfigDeclObject & decl_obj);
        std::unique_ptr<ycompiler::yCompilerInstance> compiler_instance;
        ycompiler::yFileManager * file_mgr;
    };


}



#endif //__YLIB_UTILITY_YCONFIG_HPP__