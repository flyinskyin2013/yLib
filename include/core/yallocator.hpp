/*
 * @Author: Sky
 * @Date: 2020-05-22 10:00:54
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 15:23:04
 * @Description: 
 */

#ifndef __YLIB_CORE_YALLOCATOR_HPP__
#define __YLIB_CORE_YALLOCATOR_HPP__

#include "yobject.hpp"

#include <new>
#include <cstddef>

namespace yLib{

    //new operator: allocate memory and construct object
    //operator new: allocate memory
    //placement new: construct object on an exsit memory

    /**
     *  @class yBasicAllocator
     *  @brief This is a basic mem-allocator in yLib.
     */
    template<class T>
    class yBasicAllocator MACRO_PUBLIC_INHERIT_YOBJECT
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

        /**
         *  @fn      pointer allocate(size_type n, const void * hint=static_cast<const void*>(0))
         *  @brief   Allocate memory.
         *  @param   n the num of sizeof(T) will be allocted.
         *  @param   hint not use.
         *  @warning 
         *  @return Return a pointer that size is n*sizeof(T) bytes.
         */
        pointer allocate(size_type n, const void * hint=static_cast<const void*>(0)){
            
            //malloc
            return static_cast<pointer>(::operator new( (size_t)(sizeof(T) * n) ));
        }

        /**
         *  @fn      void deallocate(pointer p, size_type n)
         *  @brief   free memory.
         *  @param   p the pointer will be freed.
         *  @param   n not use.
         *  @warning 
         *  @return none
         */
        void deallocate(pointer p, size_type n) {

            //free
            ::operator delete(p);
        }

        /**
         *  @fn      void construct(pointer p, const_reference value)
         *  @brief   copy-construct a obj on exsited memory.
         *  @param   p the pointer will be constructed.
         *  @param   value the default val for constructed obj.
         *  @warning 
         *  @return none
         */
        void construct(pointer p, const_reference value) {

            new ((void *)p) T(value);//inplace new
        }

        /**
         *  @fn      void construct(pointer p, const_rreference value)
         *  @brief   move-construct a obj on exsited memory.
         *  @param   p the pointer will be constructed.
         *  @param   value the default val for constructed obj.
         *  @warning 
         *  @return none
         */
        void construct(pointer p, const_rreference value) {

            new ((void *)p) T(value);
        }

        /**
         *  @fn      void destroy(pointer p)
         *  @brief   destructor.
         *  @param   p the pointer will be destructed.
         *  @warning 
         *  @return none
         */
        void destroy(pointer p) {
            
            p->~T();
        }

        /**
         *  @fn      size_type max_size() const
         *  @brief   calculate the max-num of T.
         *  @warning 
         *  @return the max-num of T.
         */
        size_type max_size() const{

            return UINTMAX_MAX/sizeof(T);
        }

        /**
         *  @fn      pointer address(reference x) const
         *  @brief   calculate the addr of reference(x).
         *  @param   x the obj will be calculated.
         *  @warning 
         *  @return the reference(x)'s addr
         *  @retval 
         *  @bug 
         */
        pointer address(reference x) const{

            return (pointer)&x;
        }

        /**
         *  @fn      const_pointer address(const_reference x) const
         *  @brief   calculate the const addr of reference(x)..
         *  @param   x the obj will be calculated.
         *  @warning 
         *  @return the reference(x)'s const addr
         */
        const_pointer address(const_reference x) const{

            return (const_pointer)&x;
        }

    };

}


#endif //__YLIB_CORE_YALLOCATOR_HPP__