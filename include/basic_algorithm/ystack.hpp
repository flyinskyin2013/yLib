/*
 * @Author: Sky
 * @Date: 2020-07-01 15:13:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 10:35:45
 * @Description: 
 */ 

#ifndef __BASIC_ALGORITHM_YSTACK_HPP__
#define __BASIC_ALGORITHM_YSTACK_HPP__

#include "core/yobject.hpp"
#include "core/yallocator.hpp"

namespace yLib{

    template<class T, class Alloc=std::allocator<T>>
    class yStack MACRO_PUBLIC_INHERIT_YOBJECT
    {
        private:
        uint64_t capacity = 0;
        uint64_t cur_size = 0;
        T * stack_head = nullptr;

        T NullObj;
        public:
        /**
         * @description: create a stack with 32 * sizeof(T)-bytes's capacity.
         * @param {type} 
         * @return {type} 
         */
        yStack(void){

            Alloc _alloc;
            stack_head = _alloc.allocate(32);

            // T _tmp_val;
            // for (int _i = 0; _i < 32; _i++){

            //     _alloc.construct( stack_head + _i , _tmp_val);
            // }

            capacity = 32;
            cur_size = 0;
        }
        /**
         * @description: 
         * @param {type} size_ is the current capacity of stack.
         * @return {type} 
         */
        yStack(uint64_t size_){

            Alloc _alloc;
            stack_head = _alloc.allocate(size_);

            // T _tmp_val;
            // for (int _i = 0; _i < size_; _i++){

            //     _alloc.construct( stack_head + _i , _tmp_val);
            // }

            capacity = size_;
            cur_size = 0;
        }

        ~yStack(void){

            Alloc _alloc;

            for (int _i = 0; _i < cur_size; _i++){

                _alloc.destroy( stack_head + _i );
            }
            _alloc.deallocate(stack_head, capacity);

            stack_head = nullptr;
            cur_size = 0;
            capacity = 0;
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int8_t push(T & lval_){
            
            if (cur_size == capacity)
                return -1;
            
            Alloc _alloc;
            _alloc.construct(stack_head + cur_size, lval_);

            cur_size ++;
            return 0;
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int8_t push(T && rval_){

            if (cur_size == capacity)
                return -1;

            Alloc _alloc;
            _alloc.construct(stack_head + cur_size, rval_);
            cur_size ++;
            return 0;
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        T& front(void) const{

            if (0 == cur_size)
                return const_cast<T&>(NullObj);

            return *(stack_head + cur_size - 1);
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        T& tail(void) const{

            if (0 == cur_size)
                return const_cast<T&>(NullObj);

            return *(stack_head);
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int8_t pop(void){
            
            if (0 == cur_size)
                return -1;
            
            Alloc _alloc;
            _alloc.destroy(stack_head + cur_size - 1);
            cur_size --;
            return 0;
        }

        /**
         * @description: get the current size of stack.
         * @param {type} 
         * @return {type} 
         */
        uint64_t get_size(void) const{

            return cur_size;
        }

        /**
         * @description: get the capacity of stack.
         * @param {type} 
         * @return {type} 
         */
        uint64_t get_capacity(void) const{

            return capacity;
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int8_t resize_capacity(uint64_t new_cap_){

            Alloc _alloc;
            
            T* _tmp_head = _alloc.allocate(new_cap_);
            for (int _i = 0; _i < cur_size; _i ++){

                _alloc.construct(_tmp_head + _i, std::move(*(stack_head + _i)));
                _alloc.destroy(stack_head + _i);
            }

            _alloc.deallocate(stack_head, capacity);//free old mem

            stack_head = _tmp_head;

            capacity = new_cap_;

            return capacity;
        }
    };
}

#endif //__BASIC_ALGORITHM_YSTACK_HPP__