/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:52
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-10 16:55:59
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YCONFIG_HPP__
#define __YLIB_UTILITY_YCONFIG_HPP__

#include <string>

#include "ylog.hpp"
#include "core/ycommon.hpp"
#include "core/ybasicvalue.hpp"

namespace yLib{

    /**
     *  @class yConfigValue
     *  @brief This is the yConfig's value-container, it inherits from yValue.
     */
    class __YLIB_EXPORT__ yConfigValue : public yBasicValue{
        protected:

        public:
        /**
         * @fn  yConfigValue() noexcept
         * @brief default constructor
         * @param 
         * @return 
         */
        yConfigValue() noexcept ;

        /**
         * @fn  ~yConfigValue() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        ~yConfigValue() noexcept {}

        //other constructor
        /**
         * @fn  explicit yConfigValue(int32_t value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yConfigValue(int32_t value_) noexcept;

        /**
         * @fn  explicit yConfigValue(bool value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yConfigValue(bool value_) noexcept;

        /**
         * @fn  explicit yConfigValue(float value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yConfigValue(float value_) noexcept;

        /**
         * @fn  explicit yConfigValue(const std::string &value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yConfigValue(const std::string &value_) noexcept;
        
        /**
         * @fn  explicit yConfigValue(const char *value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yConfigValue(const char *value_) noexcept;


        /**
         * @fn  yConfigValue(const yConfigValue &value_)
         * @brief copy constructor
         * @param value_ a exsited obj
         * @return 
         */
        yConfigValue(const yConfigValue &value_) noexcept;
        /**
         * @fn  yConfigValue & operator=(const yConfigValue &value_) noexcept
         * @brief assignment constructor
         * @param value_ a exsited obj
         * @return the obj's reference of yConfigValue
         */
        yConfigValue & operator=(const yConfigValue &value_) noexcept;

        /**
         * @fn  yConfigValue(const yConfigValue &&value_) noexcept
         * @brief move constructor
         * @param value_ a exsited obj
         * @return 
         */
        yConfigValue(const yConfigValue &&value_) noexcept;
        /**
         * @fn  yConfigValue & operator=(const yConfigValue &&value_) noexcept
         * @brief assignment constructor
         * @param value_ a exsited obj
         * @return the obj's reference of yConfigValue
         */
        yConfigValue & operator=(const yConfigValue &&value_) noexcept;
        
        /**
         * @fn  yConfigValue & operator=(int32_t value_)
         * @brief convert int32_t to exsited obj of yConfigValue
         * @param value_ int32_t value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(int32_t value_);

        /**
         * @fn  yConfigValue & operator=(bool value_)
         * @brief convert bool to exsited obj of yConfigValue
         * @param value_ bool value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(bool value_);

        /**
         * @fn  yConfigValue & operator=(float value_)
         * @brief convert float to exsited obj of yConfigValue
         * @param value_ bool value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(float value_);

        /**
         * @fn  yConfigValue & operator=(const std::string & value_)
         * @brief convert std::string & to exsited obj of yConfigValue
         * @param value_ std::string value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(const std::string &value_);

        /**
         * @fn  yConfigValue & operator=(const char *value_)
         * @brief convert char * to exsited obj of yConfigValue
         * @param value_ std::string value.
         * @return return obj of yConfigValue
         */    
        yConfigValue & operator=(const char *value_);
            
        /**
         * @fn  operator uint32_t() const
         * @brief convert yConfigValue to uint32_t
         * @return return a uint32_t's val from obj.
         */
        operator uint32_t() const;

        /**
         * @fn  operator bool() const
         * @brief convert yConfigValue to bool
         * @return return a bool's val from obj.
         */
        operator bool() const;


        /**
         * @fn  operator float() const
         * @brief convert yConfigValue to float
         * @return return a float's val from obj.
         */
        operator float() const;


        /**
         * @fn  operator std::string() const
         * @brief convert yConfigValue to std::string
         * @return return a std::string's val from obj.
         */
        operator std::string() const;

        friend class yConfig;

        private:

    };

    /**
     *  @class yConfig
     *  @brief This is a tool to parse a cfg-file that is like c.
     */
    class __YLIB_EXPORT__ yConfig MACRO_PUBLIC_INHERIT_YOBJECT{
        public:
        /**
         * @fn  yConfigValue() noexcept
         * @brief default constructor
         * @param 
         * @return 
         */
        yConfig();

        /**
         * @fn  ~yConfigValue() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        ~yConfig();
        
        /**
         * @fn  yConfig(const yConfig &config_) = delete;
         * @brief copy constructor
         * @param config_ a exsited obj
         * @return 
         * @warning this op is disable.
         */
        yConfig(const yConfig &config_) = delete;
        /**
         * @fn  yConfig & operator=(const yConfig &config_) = delete
         * @brief assignment func
         * @param config_ a exsited obj
         * @return the obj of yConfig
         * @warning this op is disable.
         */
        yConfig & operator=(const yConfig &config_) = delete;


        /**
         * @fn yConfig(const yConfig &&config_) = delete
         * @brief move constructor
         * @param config_ a exsited obj
         * @return 
         * @warning this op is disable.
         */
        yConfig(const yConfig &&config_) = delete;
        /**
         * @fn  yConfig & operator=(const yConfig &&config_) = delete
         * @brief assignment func
         * @param config_ a exsited obj
         * @return the obj of yConfig
         * @warning this op is disable.
         */
        yConfig & operator=(const yConfig &&config_) = delete;

        /**
         * @fn  int8_t ReadFile(const std::string & file_path_)
         * @brief read cfg-file from disk.
         * @param file_path_ the path of cfg-file 
         * @return the op status
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t ReadFile(const std::string & file_path_);

        /**
         * @fn  int8_t WriteFile(const std::string & file_path_)
         * @brief write cfg-file to disk.
         * @param file_path_ the path of cfg-file 
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t WriteFile(const std::string & file_path_);

        /**
         * @fn  yConfigValue GetValue(const std::string &node_path_)
         * @brief get value by node-path.
         * @param node_path_ the path of node.
         * @return the value of node-path.
         * @warning 
         */        
        yConfigValue GetValue(const std::string &node_path_);
        
        //set value interface
        /**
         * @fn  int SetValue(const std::string &node_path_, const yConfigValue & value_)
         * @brief get value by node-path.
         * @param node_path_ the path of node.
         * @param value_ the value to set.
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */        
        int8_t SetValue(const std::string &node_path_, const yConfigValue & value_);

        //add node interface
        //if add_node_pos == '.', we will add node to root
        /**
         * @fn  int SetValue(const std::string &node_path_, const yConfigValue & value_)
         * @brief get value by node-path.
         * @param pos_ the position of node.
         * @param name_ the name of node.
         * @param type_ the type of value.we only support int32_t/bool/float/std::string/group.
         * @param value_ the value to set.
         * @return the status of op.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning if pos_ == '.' or "", we will add node to root
         */      
        int8_t AddNode(const std::string & pos_, const std::string & name_, yValue::yValueType type_, const yConfigValue &value_ = yConfigValue());

        private:
        void * config_instance;

    };


}



#endif //__YLIB_UTILITY_YCONFIG_HPP__