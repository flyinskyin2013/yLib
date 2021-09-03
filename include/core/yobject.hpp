/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-30 14:05:25
 * @Description: 
 */

#ifndef __YLIB_CORE_YOBJECT_HPP__
#define __YLIB_CORE_YOBJECT_HPP__

#include <string>
#include <functional>
#include <memory>

#include "ydefs.hpp"

/**
 *  @namespace yLib
 *  @brief     This is a top namespace in yLib.
 */
namespace yLib
{   
    /**
     *  @class yNotCopyable
     *  @brief This class and child-class can't construct, they are static class.
     */
    class yStaticClass
    {
        private:
        yStaticClass() = default;
    };

    /**
     *  @class yNoDefaultConstructorClass
     *  @brief This class and child-class don't have default constructor.
     */
    class yNoDefaultConstructorClass
    {
        private:
        yNoDefaultConstructorClass() = default;
    };
    
    /**
     *  @class yNotCopyable
     *  @brief This class and child-class can't copy.
     */
    class yNotCopyable
    {
        public:
        yNotCopyable() = default;
        ~yNotCopyable() = default;

        yNotCopyable(const yNotCopyable & obj) = delete;
        yNotCopyable & operator=(const yNotCopyable & obj) = delete;

        yNotCopyable(yNotCopyable && obj) = default;
        yNotCopyable & operator=(yNotCopyable && obj) = default;
    };
    
    /**
     *  @class yNotMoveable
     *  @brief This class and child-class can't move.
     */
    class yNotMoveable
    {
        public:
        yNotMoveable() = default;
        ~yNotMoveable() = default;

        yNotMoveable(const yNotMoveable & obj) = default;
        yNotMoveable & operator=(const yNotMoveable & obj) = default;

        yNotMoveable(yNotMoveable && obj) = delete;
        yNotMoveable & operator=(yNotMoveable && obj) = delete;        
    };

    /**
     *  @class yNotCopyableAndMoveable
     *  @brief This class and child-class can't copy and move.
     */
    class yNotCopyableAndMoveable:public yNotCopyable, public yNotMoveable
    {};

    /**
     *  @class yClassInfo
     *  @brief This class store some class-info for reflex in the future.
     */
    template<typename T>
    class yClassInfo
    {
        public:
        yClassInfo() = delete;
        yClassInfo(const std::string & cls_name, const std::function<std::unique_ptr<T>()> & default_creator) noexcept;
        std::string class_name;

        std::function<std::unique_ptr<T>()> default_creator_method;
    };

    template<typename T>
    yClassInfo<T>::yClassInfo(const std::string & cls_name, const std::function<std::unique_ptr<T>()> & default_creator) noexcept
    :class_name(cls_name), default_creator_method(default_creator)
    {}

    #define YLIB_DECLARE_CLASSINFO_CONTENT(class_name) \
        private: \
        static yClassInfo<class_name> class_info; \
        public: \
        static const yClassInfo<class_name> & GetClassInfo(void) noexcept

    #define YLIB_IMPLEMENT_CLASSINFO_CONTENT(class_name) \
        yLib::yClassInfo<yLib::class_name> yLib::class_name::class_info( \
                                                std::string(#class_name), \
                                                [](){return std::unique_ptr<yLib::class_name>(new (std::nothrow) class_name());}); \
        \
        const yLib::yClassInfo<yLib::class_name> & yLib::class_name::GetClassInfo(void) noexcept {return class_info;} 

    #define YLIB_IMPLEMENT_CLASSINFO_CONTENT_S(class_name) \
        yLib::yClassInfo<yLib::class_name> yLib::class_name::class_info( \
                                                std::string(#class_name), \
                                                nullptr); \
        \
        const yLib::yClassInfo<yLib::class_name> & yLib::class_name::GetClassInfo(void) noexcept {return class_info;} 
 
    /**
     *  @class yObject
     *  @brief This is base-class of the all class in yLib.
     */
    class __YLIB_EXPORT__ yObject
    {
    public:

        YLIB_DECLARE_CLASSINFO_CONTENT(yObject);
    };

} // yLib


//define some useful macroes

/** \def MACRO_PUBLIC_INHERIT_YOBJECT
    \brief The public inherit from yObject.
*/
#define MACRO_PUBLIC_INHERIT_YOBJECT :public yLib::yObject


#define YLIB_PUBLIC_INHERIT_YOBJECT public yLib::yObject

#define YLIB_PUBLIC_INHERIT_YSTATICCLASS public yLib::yStaticClass

#define YLIB_PUBLIC_INHERIT_YNODEFAULTCONSTRUCTORCLASS public yLib::yNoDefaultConstructorClass

#define YLIB_PUBLIC_INHERIT_YNOTCOPYABLE public yLib::yNotCopyable

#define YLIB_PUBLIC_INHERIT_YNOTMOVEABLE public yLib::yNotMoveable

#define YLIB_PUBLIC_INHERIT_YNOTCOPYABLEANDMOVEABLE public yLib::yNotCopyableAndMoveable

/** \def MACRO_INIT_YOBJECT_PROPERTY(object_name)
    \brief Set the object-name.
*/
#define MACRO_INIT_YOBJECT_PROPERTY(object_name) \
    :yObject()




#endif //__YLIB_CORE_YOBJECT_HPP__