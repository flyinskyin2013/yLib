/*
 * @Author: Sky
 * @Date: 2020-05-22 10:00:54
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:31:39
 * @Description: 
 */

#ifndef __YLIB_CORE_YALLOCATOR_HPP__
#define __YLIB_CORE_YALLOCATOR_HPP__

#include "yobject.hpp"

#include <new>

namespace yLib{

    //new operator: allocate memory and construct object
    //operator new: allocate memory
    //placement new: construct object on an exsit memory


    template<class T>
    class yBasicAllocator MACRO_PUBLIC_INHERIT_YOBJECT
    {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        
        template<class U>
        struct rebind
        {
            typedef yBasicAllocator<U> other;
        };


    private:
        /* data */

    
    public:


        pointer allocate(size_type n, const void * hint=0){

            pointer _alloc_addr = (pointer)::operator new( (size_t)(sizeof(T) * n) );
            return _alloc_addr;
        }

        void deallocate(pointer p, size_type n) {

            ::operator delete(p);
        }

        void construct(pointer p, const_reference value) {

            new((void *)p) T(value);
        }

        void destroy(pointer p) {
            
            p->~T();
        }

        size_type max_size() const{

            return UINTMAX_MAX/sizeof(T);
        }
        pointer address(reference x) const{

            return (pointer)&x;
        }
        const_pointer address(const_reference x) const{

            return (const_pointer)&x;
        }

    };

}


#endif //__YLIB_CORE_YALLOCATOR_HPP__