/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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
#include <atomic>
#include <string>

#include "ydefs.hpp"

/**
 *  @namespace yLib
 *  @brief     This is a top namespace in yLib.
 */
namespace yLib
{   
    /**
     *  @class yStaticClass
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
        using RefCounterType = std::atomic<int32_t>;

        // for user deleter
        typedef void(*TypeDeleter)(yObject* self);

        static std::string TypeIndex2Key(int32_t tindex);
        static int32_t TypeKey2Index(const std::string& key);

        static bool classof(yObject * obj) {return true;}

        static bool classof(int32_t parent_tindex, uint32_t child_tindex);

        yObject(){}

        // Override the copy and assign constructors to do nothing.
        // We don't copy ref_counter_/type_index_/type_deleter_
        yObject(const yObject& other){}
        yObject(yObject&& other){}

        yObject& operator=(const yObject& other) {
            return *this;
        }
        yObject& operator=(yObject&& other) {
            return *this;
        }

        YLIB_DECLARE_CLASSINFO_CONTENT(yObject);

    protected:
        RefCounterType ref_counter_{0};
        int32_t type_index_{0};

        TypeDeleter type_deleter_ = nullptr;

        inline void inc_ref();
        inline void dec_ref();
        inline int32_t ref_count() const;

        template <typename>
        friend class ObjectPtr;


    YLIB_DEFINE_CLASS_TYPE_KEY(yLib::yObject);
    };
    

    using Object = yObject;

    template <typename T>
    class ObjectPtr {
    public:
        ObjectPtr() {}
        ObjectPtr(std::nullptr_t) {}
        explicit ObjectPtr(Object* data) : data_(data) {
            if (nullptr != data) {
                data_->inc_ref();
            }
        }

        ObjectPtr(const ObjectPtr<T>& other) 
            : ObjectPtr(other.data_) {}

        template <typename U>
        ObjectPtr(const ObjectPtr<U>& other)
            : ObjectPtr(other.data_) {
            static_assert(std::is_base_of<T, U>::value,
                        "only copy child class ObjectPtr");
        }

        ObjectPtr(ObjectPtr<T>&& other)
            : data_(other.data_) {
            other.data_ = nullptr;
        }
        template <typename Y>
        ObjectPtr(ObjectPtr<Y>&& other)
            : data_(other.data_) {
            static_assert(std::is_base_of<T, Y>::value,
                        "only move child class ObjectPtr");
            other.data_ = nullptr;
        }

        ObjectPtr<T>& operator=(const ObjectPtr<T>& other) {
            
            ObjectPtr(other).swap(*this);
            return *this;
        }
        ObjectPtr<T>& operator=(ObjectPtr<T>&& other) {

            ObjectPtr(std::move(other)).swap(*this);
            return *this;
        }
        ~ObjectPtr() { this->reset(); }
        void reset() {
            if (nullptr != data_) {
                data_->dec_ref();
                data_ = nullptr;
            }
        }

        void swap(ObjectPtr<T>& other) { 
            std::swap(data_, other.data_);
        }

        T* get() const { return static_cast<T*>(data_); }
        T* get_ref() const { return *get(); }
    private:
        Object* data_{nullptr};
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

// rename yLib
namespace ylib = yLib;



#endif //__YLIB_CORE_YOBJECT_HPP__