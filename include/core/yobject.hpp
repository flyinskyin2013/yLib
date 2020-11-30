/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 14:44:38
 * @Description: 
 */

#ifndef __YLIB_CORE_YOBJECT_HPP__
#define __YLIB_CORE_YOBJECT_HPP__

#include <string>

#include "ydefs.hpp"
#include "yversion.hpp"

/**
 *  @namespace yLib
 *  @brief     This is a top namespace in yLib.
 */
namespace yLib
{
    /**
     *  @class yObject
     *  @brief This is base-class of the all object in yLib.
     */
    class __YLIB_EXPORT__ yObject
    {
    private:
        /* data */
        
    public:
        /**
         *  @fn    yObject()
         *  @brief Default constructor
         */
        yObject(/* args */){}

        /**
         *  @fn    yObject(std::string object_name_)
         *  @brief Override constructor
         *  @param object_name_ the name of obj.
         */
        yObject(std::string object_name_);

        /**
         *  @fn      yObject(const yObject & object_)
         *  @brief   Copy constructor
         *  @param   object_ exsited obj.
         *  @warning This op is deleted.
         */
        yObject(const yObject & object_) = delete;

        /**
         *  @fn      yObject & operator=(const yObject & object_)
         *  @brief   Assignment constructor
         *  @param   object_ exsited obj.
         *  @warning This op is deleted.
         *  @return The reference of yObject's object.
         */
        yObject & operator=(const yObject & object_) = delete;

        /**
         *  @fn      yObject(const yObject && object_)
         *  @brief   Move constructor
         *  @param   object_ exsited obj.
         *  @warning This op is deleted.
         */
        yObject(const yObject && object_) = delete;

        /**
         *  @fn      yObject & operator=(const yObject && object_)
         *  @brief   Assignment constructor
         *  @param   object_ exsited obj.
         *  @warning This op is deleted.
         *  @return  The reference of yObject's object.
         */
        yObject & operator=(const yObject && object_) = delete;

        /**
         *  @fn    ~yObject()
         *  @brief Default destructor
         */
        ~yObject(){}

        /** @var std::string object_name
            @brief The object-name.
        */
        std::string object_name;
        
    };

} // yLib


//define some useful macroes

/** \def MACRO_PUBLIC_INHERIT_YOBJECT
    \brief The public inherit from yObject.
*/
#define MACRO_PUBLIC_INHERIT_YOBJECT :public yLib::yObject


/** \def MACRO_INIT_YOBJECT_PROPERTY(object_name)
    \brief Set the object-name.
*/
#define MACRO_INIT_YOBJECT_PROPERTY(object_name) \
    :yObject(std::string(#object_name))




#endif //__YLIB_CORE_YOBJECT_HPP__