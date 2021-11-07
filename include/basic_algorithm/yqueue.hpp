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
 * @Date: 2020-07-01 15:13:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 10:36:20
 * @Description: 
 */ 
#ifndef __BASIC_ALGORITHM_YQUEUE_HPP__
#define __BASIC_ALGORITHM_YQUEUE_HPP__

#include "core/yobject.hpp"
#include "core/yallocator.hpp"

namespace yLib{

    template<class T, class Alloc=std::allocator<T>>
    class yQueue MACRO_PUBLIC_INHERIT_YOBJECT
    {
        private:
        uint64_t capacity = 0;
        uint64_t cur_size = 0;
        T * queue_head = nullptr;
        T * begin_pos = nullptr;
        T * end_pos = nullptr;
        
        T NullObj;

        public:
        /**
         * @description: create a stack with 32 * sizeof(T)-bytes's capacity.
         * @param {type} 
         * @return {type} 
         */
        yQueue(void){

            Alloc _alloc;
            queue_head = _alloc.allocate(32);

            begin_pos = end_pos = queue_head;
            capacity = 32;
            cur_size = 0;
        }
        /**
         * @description: 
         * @param {type} size_ is the current capacity of stack.
         * @return {type} 
         */
        yQueue(uint64_t size_){

            Alloc _alloc;
            queue_head = _alloc.allocate(size_);

            begin_pos = end_pos = queue_head;
            capacity = size_;
            cur_size = 0;
        }

        ~yQueue(void){

            Alloc _alloc;

            if (0 == cur_size){

                _alloc.deallocate(queue_head, capacity);//free old mem
                queue_head = nullptr;
                begin_pos = end_pos = queue_head;
            }
            else{//there are elements.

                // begin_pos     end_pos
                //    |             |
                //----@@@@@@@@@@@@@@-----
                if (end_pos > begin_pos){

                    for (int _i = 0; _i < cur_size; _i ++){

                        _alloc.destroy(begin_pos + _i);
                    }
                }
                else{//end_pos <= begin_pos

                    //  end_pos begin_pos
                    //     |    |
                    //@@@@@-----@@@@@@@@@@@@@@@@@@@@
                    //begin_pos to tail
                    for (int _i = 0; begin_pos + _i != queue_head + capacity - 1; _i ++){

                        _alloc.destroy(begin_pos + _i);
                    }
                    //head to end_pos
                    for (int _i = 0; queue_head + _i != end_pos; _i ++){

                        _alloc.destroy(queue_head + _i);
                    }
                }
                
                _alloc.deallocate(queue_head, capacity);//free old mem

                queue_head = nullptr;

                end_pos = begin_pos = queue_head;

                cur_size = 0;

                capacity = 0;
            }
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
            _alloc.construct(end_pos, lval_);
            
            if (queue_head + capacity - 1 == end_pos)
                end_pos = queue_head;
            else
                end_pos ++;
                
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
            _alloc.construct(end_pos, rval_);
            
            if (queue_head + capacity - 1 == end_pos)
                end_pos = queue_head;
            else
                end_pos ++;
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

            return *(begin_pos);
        }

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        T& tail(void) const{

            if (0 == cur_size)
                return const_cast<T&>(NullObj);

            return *(end_pos - 1);
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
            _alloc.destroy(begin_pos);

            if (queue_head + capacity - 1 == begin_pos)
                begin_pos = queue_head;
            else
                begin_pos ++;
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

            if (0 == cur_size){

                _alloc.deallocate(queue_head, capacity);//free old mem
                queue_head = _tmp_head;
                begin_pos = end_pos = queue_head;
            }
            else{//there are elements.

                // begin_pos     end_pos
                //    |             |
                //----@@@@@@@@@@@@@@-----
                if (end_pos > begin_pos){

                    for (int _i = 0; _i < cur_size; _i ++){

                        _alloc.construct(_tmp_head + _i, std::move(*(begin_pos + _i)));
                        _alloc.destroy(begin_pos + _i);
                    }
                }
                else{//end_pos <= begin_pos

                    //  end_pos begin_pos
                    //     |    |
                    //@@@@@-----@@@@@@@@@@@@@@@@@@@@
                    //begin_pos to tail
                    for (int _i = 0; begin_pos + _i != queue_head + capacity - 1; _i ++){

                        _alloc.construct(_tmp_head + _i, std::move(*(begin_pos + _i)));
                        _alloc.destroy(begin_pos + _i);
                    }
                    //head to end_pos
                    for (int _i = 0; queue_head + _i != end_pos; _i ++){

                        _alloc.construct(_tmp_head + _i, std::move(*(queue_head + _i)));
                        _alloc.destroy(queue_head + _i);
                    }
                }



                _alloc.deallocate(queue_head, capacity);//free old mem

                queue_head = _tmp_head;

                begin_pos = queue_head;

                end_pos = queue_head + cur_size;
            }
            
            capacity = new_cap_;
            return capacity;
        }
    };
}

#endif //__BASIC_ALGORITHM_YQUEUE_HPP__