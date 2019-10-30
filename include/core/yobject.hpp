/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-28 17:17:51
 * @Description: 
 */

#ifndef _YLIB_CORE_YOBJECT_HPP_
#define _YLIB_CORE_YOBJECT_HPP_

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
        yObject(yObject & object) = delete;
        yObject & operator=(yObject & object) = delete;
        yObject(yObject && object) = delete;
        yObject & operator=(yObject && object) = delete;
        ~yObject(){}

        
        std::string _object_name;
        
    };

} // yLib






#endif //_YLIB_CORE_YOBJECT_H_