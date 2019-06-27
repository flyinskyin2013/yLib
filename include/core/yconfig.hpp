#ifndef _YCONFIG_H_
#define _YCONFIG_H_

#include <string>
#include "ylog.hpp"


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

    class yConfig{
        public:
            yConfig();
            ~yConfig();
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
            yConfigValue  yConfigGetValue(const char * node_path)throw(char *, const char *);
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
            libconfig::Config &m_config;



    };


    class yConfigValue{

        public:

        yConfigValue(){}
        ~yConfigValue(){}
		
		yConfigValueType GetyConfigValueType(void);

        operator int() const;
        operator bool() const;
        operator float() const;
        operator std::string() const;
        // operator const char * () const;

        yConfigValue & operator=(int value);
        yConfigValue & operator=(bool value);
        yConfigValue & operator=(float value);
        yConfigValue & operator=(std::string & value);
        yConfigValue & operator=(const char * value);

        friend class yConfig;

        private:
        int value_int;
        bool value_bool;
        float value_float;
        std::string value_string;
        yConfigValueType _current_data_type_ = yConfigValueType::NONE_TYPE;
    };
}



#endif //_YCONFIG_H_