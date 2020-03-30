/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-19 11:38:48
 * @Description: 
 */

#ifndef _YLIB_CORE_YOBJECT_HPP_
#define _YLIB_CORE_YOBJECT_HPP_

#include <string>

#include "ydefs.hpp"
#include "yversion.hpp"

namespace yLib
{

    class __YLIB_EXPORT__ yObject
    {
    private:
        /* data */
        
    public:
        yObject(/* args */){}
        yObject(std::string object_name);
        yObject(yObject & object) = delete;
        yObject & operator=(yObject & object) = delete;
        yObject(yObject && object) = delete;
        yObject & operator=(yObject && object) = delete;
        ~yObject(){}

        
        std::string _object_name;
        
    };

} // yLib

//define some useful macroes ---------------------------- start
//for yObject
#define MACRO_PUBLIC_INHERIT_YOBJECT :public yLib::yObject

//call the yObject
#define MACRO_INIT_YOBJECT_PROPERTY(object_name) \
    :yObject(std::string(#object_name))




#endif //_YLIB_CORE_YOBJECT_H_