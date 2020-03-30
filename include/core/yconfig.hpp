/*
 * @Author: Sky
 * @Date: 2019-07-04 11:28:52
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-30 11:29:39
 * @Description: 
 */

#ifndef _YLIB_CORE_YCONFIG_HPP_
#define _YLIB_CORE_YCONFIG_HPP_

#include <string>

#include "ylog.hpp"
#include "ycommon.hpp"
#include "ybasicvalue.hpp"

namespace libconfig{

    class Config;
};
namespace yLib{

    class yConfigValue;

    typedef enum _yconfigvaluetype{
        NONE_TYPE = 0,
        INT_TYPE = 1,
        BOOL_TYPE = 2,
        FLOAT_TYPE = 3,
        STRING_TYPE = 4,
        GROUP_TYPE = 5,
        
        
    } yConfigValueType;

    class __YLIB_EXPORT__ yConfig MACRO_PUBLIC_INHERIT_YOBJECT{
        public:
            yConfig();
            ~yConfig();
            yConfig(yConfig & config) = delete;
            yConfig & operator=(yConfig & config) = delete;

            int yConfigReadFile(const std::string & file_path);
            int yConfigWriteFile(const std::string & file_path);

           // yConfigSetting & operator [](const char * setting_name);
            //int yConfigGetRootSetting(void);

            //get value interface
            /* old api
            int yConfigGetIntValue(const char * node_path);
            bool yConfigGetBoolValue(const char * node_path);
            float yConfigGetFloatValue(const char * node_path);
            std::string yConfigGetStringValue(const char * node_path);
            */
            yConfigValue  yConfigGetValue(const char * node_path);
            yConfigValue  yConfigGetValue(const std::string & node_path);
           
            
            //set value interface
            /* old api
            int yConfigSetIntValue(const char * node_path, int value);
            int yConfigSetBoolValue(const char * node_path, bool value);
            int yConfigSetFloatValue(const char * node_path, float value);
            int yConfigSetStringValue(const char * node_path, std::string & value);
            */
            int yConfigSetValue(const char * node_path, const yConfigValue & value);
            int yConfigSetValue(const std::string & node_path, const yConfigValue & value);

            //add node interface
            //if add_node_pos == '.', we will add node to root
            int yConfigAddNode(const char * add_node_pos, const char * add_node_name, yConfigValueType add_node_type, const yConfigValue & value);
            int yConfigAddNode(const std::string & add_node_pos, const std::string & add_node_name, yConfigValueType add_node_type, const yConfigValue & value);

            int yConfigAddNode(const char * add_node_pos, const char * add_node_name, yConfigValueType add_node_type);
            int yConfigAddNode(const std::string & add_node_pos, const std::string & add_node_name, yConfigValueType add_node_type);
            // operator bool ();
            // operator int ();
            // operator std::string ();
            // operator float ();
            
        private:
            libconfig::Config &_libconfig_config;



    };


    class __YLIB_EXPORT__ yConfigValue : public yBasicValue{

        public:
        yConfigValue() noexcept {}
        ~yConfigValue() noexcept {}

        //other constructor
        explicit yConfigValue(int64_t value_) noexcept;
        explicit yConfigValue(uint64_t value_) noexcept;
        explicit yConfigValue(bool value_) noexcept;
        explicit yConfigValue(float value_) noexcept;
        explicit yConfigValue(double value_) noexcept;
        explicit yConfigValue(std::string & value_) noexcept;
        explicit yConfigValue(const char * value_) noexcept;




        yConfigValue(const yConfigValue & config_value) noexcept;
        yConfigValue & operator=(const yConfigValue & config_value) noexcept;
        yConfigValue(yConfigValue && config_value) noexcept;
        yConfigValue & operator=(yConfigValue && config_value) noexcept;
		
		yConfigValueType GetyConfigValueType(void) const;

/* merge to ybasicvalue
        operator int() const;
        operator bool() const;
        operator float() const;
        operator std::string() const;
        // operator const char * () const;
*/
        yConfigValue & operator=(int value);
        yConfigValue & operator=(bool value);
        yConfigValue & operator=(float value);
        yConfigValue & operator=(std::string & value);
        yConfigValue & operator=(std::string && value);
        yConfigValue & operator=(const char * value);

        friend class yConfig;

        private:
/* merge to ybasicvalue        
        int _n_value_int;
        bool _b_value_bool;
        float _f_value_float;
        std::string _str_value_string;
*/
        yConfigValueType _current_data_type_ = yConfigValueType::NONE_TYPE;
    };
}



#endif //_YLIB_CORE_YCONFIG_H_