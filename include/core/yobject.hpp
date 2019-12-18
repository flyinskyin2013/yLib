/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2019-12-10 10:16:00
 * @Description: 
 */

#ifndef _YLIB_CORE_YOBJECT_HPP_
#define _YLIB_CORE_YOBJECT_HPP_

#include <string>

namespace yLib
{

#ifdef _WIN32
    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

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