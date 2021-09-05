/*
 * @Author: Sky
 * @Date: 2019-09-20 16:59:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 22:53:59
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
    class __YLIB_CLASS_DECLSPEC__ yStaticClass
    {
        private:
        yStaticClass() = default;
    };

    /**
     *  @class yNoDefaultConstructorClass
     *  @brief This class and child-class don't have default constructor.
     */
    class __YLIB_CLASS_DECLSPEC__ yNoDefaultConstructorClass
    {
        private:
        yNoDefaultConstructorClass() = default;
    };
    
    /**
     *  @class yNotCopyable
     *  @brief This class and child-class can't copy.
     */
    class __YLIB_CLASS_DECLSPEC__ yNotCopyable
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
    class __YLIB_CLASS_DECLSPEC__ yNotMoveable
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
    class __YLIB_CLASS_DECLSPEC__ yNotCopyableAndMoveable:public yNotCopyable, public yNotMoveable
    {};

    /**
     *  @class yClassInfo
     *  @brief This class store some class-info for reflex in the future.
     */
    template<typename T>
    class __YLIB_TEAMPLATE_CLASS_EXPORT__ yClassInfo
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

    //GetClassInfo is conflicted with GetClassInfo() on window
    #define YLIB_DECLARE_CLASSINFO_CONTENT(class_name) \
        private: \
        static yClassInfo<class_name> class_info; \
        public: \
        static const yClassInfo<class_name> & yLibGetClassInfo(void) noexcept

    #define YLIB_IMPLEMENT_CLASSINFO_CONTENT(class_name) \
        yLib::yClassInfo<yLib::class_name> yLib::class_name::class_info( \
                                                std::string(#class_name), \
                                                [](){return std::unique_ptr<yLib::class_name>(new (std::nothrow) class_name());}); \
        \
        __YLIB_TEAMPLATE_FUNC_EXPORT__ const yLib::yClassInfo<yLib::class_name> & yLib::class_name::yLibGetClassInfo(void) noexcept {return class_info;} 

    #define YLIB_IMPLEMENT_CLASSINFO_CONTENT_S(class_name) \
        yLib::yClassInfo<yLib::class_name> yLib::class_name::class_info( \
                                                std::string(#class_name), \
                                                nullptr); \
        \
        __YLIB_TEAMPLATE_FUNC_EXPORT__ const yLib::yClassInfo<yLib::class_name> & yLib::class_name::yLibGetClassInfo(void) noexcept {return class_info;} 
 
    /**
     *  @class yObject
     *  @brief This is base-class of the all class in yLib.
     */
    class __YLIB_CLASS_DECLSPEC__ yObject
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