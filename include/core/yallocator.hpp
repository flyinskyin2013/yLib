/*
 * @Author: Sky
 * @Date: 2020-05-22 10:00:54
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 22:54:08
 * @Description: 
 */

#ifndef __YLIB_CORE_YALLOCATOR_HPP__
#define __YLIB_CORE_YALLOCATOR_HPP__

#include "yobject.hpp"

#include <new>
#include <cstddef>
#include <typeinfo>



namespace yLib{

    //new operator: allocate memory and construct object
    //operator new: allocate memory
    //placement new: construct object on an exsit memory

    /**
     *  @class yBasicAllocator
     *  @brief This is a basic mem-allocator in yLib.
     */
    template<class T>
    class __YLIB_TEAMPLATE_FUNC_EXPORT__ yBasicAllocator:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef T&&             rreference;
        typedef const T&&       const_rreference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        
        /**
         *  @struct rebind
         *  @brief The element of yBasicAllocator.
         */
        template<class U>
        struct rebind
        {
            typedef yBasicAllocator<U> other;
        };


    private:
        /* data */

    
    public:
        //For msvc
        //error C2512: 'std::unique_ptr<yLib::yBasicAllocator<T>,std::default_delete<yLib::yBasicAllocator<T>>>': no appropriate default constructor available
        // yBasicAllocator<T>();
        // ~yBasicAllocator<T>();

        /**
         *  @fn      pointer allocate(size_type n, const void * hint=static_cast<const void*>(0)) noexcept
         *  @brief   Allocate memory.
         *  @param   n the num of sizeof(T) will be allocted.
         *  @param   hint not use.
         *  @warning 
         *  @return Return a pointer that size is n*sizeof(T) bytes.
         */
        pointer allocate(size_type n, const void * hint=static_cast<const void*>(0)) noexcept
        {
            
            //malloc
            return static_cast<pointer>(::operator new((size_t)(sizeof(T) * n), std::nothrow));
        }

        /**
         *  @fn      void deallocate(pointer p, size_type n) noexcept
         *  @brief   free memory.
         *  @param   p the pointer will be freed.
         *  @param   n not use.
         *  @warning 
         *  @return none
         */
        void deallocate(pointer p, size_type n) noexcept
        {

            //free
            ::operator delete(p);
        }

        /**
         *  @fn      void construct(pointer p, const_reference value) noexcept
         *  @brief   copy-construct a obj on exsited memory.
         *  @param   p the pointer will be constructed.
         *  @param   value the default val for constructed obj.
         *  @warning 
         *  @return none
         */
        void construct(pointer p, const_reference value) noexcept
        {

            new ((void *)p) T(value);//inplace new
        }

        /**
         *  @fn      void construct(pointer p, const_rreference value) noexcept
         *  @brief   move-construct a obj on exsited memory.
         *  @param   p the pointer will be constructed.
         *  @param   value the default val for constructed obj.
         *  @warning 
         *  @return none
         */
        void construct(pointer p, const_rreference value) noexcept
        {

            new ((void *)p) T(value);
        }

        /**
         *  @fn      void destroy(pointer p)
         *  @brief   destructor.
         *  @param   p the pointer will be destructed.
         *  @warning 
         *  @return none
         */
        void destroy(pointer p) noexcept
        {
            
            p->~T();
        }

        /**
         *  @fn      size_type max_size() const noexcept
         *  @brief   calculate the max-num of T.
         *  @warning 
         *  @return the max-num of T.
         */
        size_type max_size() const  noexcept
        {

            return UINTMAX_MAX/sizeof(T);
        }

        /**
         *  @fn      pointer address(reference x) const noexcept
         *  @brief   calculate the addr of reference(x).
         *  @param   x the obj will be calculated.
         *  @warning 
         *  @return the reference(x)'s addr
         *  @retval 
         *  @bug 
         */
        pointer address(reference x) const noexcept
        {

            return (pointer)&x;
        }

        /**
         *  @fn      const_pointer address(const_reference x) const noexcept
         *  @brief   calculate the const addr of reference(x)..
         *  @param   x the obj will be calculated.
         *  @warning 
         *  @return the reference(x)'s const addr
         */
        const_pointer address(const_reference x) const noexcept
        {

            return (const_pointer)&x;
        }

        YLIB_DECLARE_CLASSINFO_CONTENT(yBasicAllocator<T>);
    };

}

    // template<typename T>   
    // yLib::yBasicAllocator<T>::yBasicAllocator<T>(){}

    // template<typename T>   
    // yLib::yBasicAllocator<T>::~yBasicAllocator<T>(){}

    // abi::__cxa_demangle() convert mangle-str to readable-str for gnu-compiler
    // #include <cxxabi.h>
#ifdef _WIN32

    template<typename T>
    yLib::yClassInfo<yLib::yBasicAllocator<T>> yLib::yBasicAllocator<T>::class_info( \
                                                std::string("yBasicAllocator<"+std::string(typeid(T).name())+">"), \
                                                nullptr); 

#elif __linux__ || __linux
    
    template<typename T>
    yLib::yClassInfo<yLib::yBasicAllocator<T>> yLib::yBasicAllocator<T>::class_info( \
                                                std::string("yBasicAllocator<"+std::string(typeid(T).name())+">"), \
                                                [](){return std::unique_ptr<yLib::yBasicAllocator<T>>(new (std::nothrow) yBasicAllocator<T>());}); 

#endif 

    


    template<typename T>   
    const yLib::yClassInfo<yLib::yBasicAllocator<T>> & yLib::yBasicAllocator<T>::yLibGetClassInfo(void) noexcept {return class_info;} 

#endif //__YLIB_CORE_YALLOCATOR_HPP__