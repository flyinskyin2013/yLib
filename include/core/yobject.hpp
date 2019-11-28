/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2019-11-28 19:05:54
 * @Description: 
 */

#ifndef _YLIB_CORE_YOBJECT_HPP_
#define _YLIB_CORE_YOBJECT_HPP_

#include <string>

namespace yLib
{

#ifdef _WIN32 || _WIN64
    #define __yLib_EXPORT__ __declspec(dllexport)
    #define __yLib_IMPORT__ __declspec(dllimport)
#elif __linux__ || __linux
    #define __yLib_EXPORT__
    #define __yLib_IMPORT__
#elif __unix__ || __unix

#endif //__unix__ || __unix
    class __yLib_EXPORT__ yObject
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






#endif //_YLIB_CORE_YOBJECT_H_