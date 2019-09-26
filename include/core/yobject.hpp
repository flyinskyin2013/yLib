/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-23 15:59:14
 * @Description: 
 */

#ifndef _YLIB_CORE_YOBJECT_H_
#define _YLIB_CORE_YOBJECT_H_

#include <string>

namespace yLib
{
    

    class yObject
    {
    private:
        /* data */
        
    public:
        yObject(/* args */){}
        yObject(std::string object_name);
        ~yObject(){}
        std::string _object_name;
        
    };

} // yLib






#endif //_YLIB_CORE_YOBJECT_H_