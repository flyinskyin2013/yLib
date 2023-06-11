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
 * @Date: 2020-05-22 09:54:19
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-22 18:25:44
 * @Description: 
 */

#ifndef __BASIC_ALGORITHM_YSEQUENCE_LIST_HPP__
#define __BASIC_ALGORITHM_YSEQUENCE_LIST_HPP__

#include <memory>
#include <cmath>

#include "core/yobject.hpp"
#include "core/yallocator.hpp"

namespace yLib{

    /**
     * @description: A implement of linear list,class T must have operator== , operator=, operator> or operator<
     * @param {type} 
     * @return: 
     */
    template<class T, class Alloc=std::allocator<T>>
    class yLinearList MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        //unit is element_num.
        size_t sq_size = 0;

        //unit is byte.
        size_t sq_capacity = 0;

        //unit is byte.
        size_t sq_element_size = 0;

        void * sq_data_head = nullptr;


        /**
         * @description: find a num(index of 2) is greater than size_
         * @param size_ is num of bytes.
         * @return: 
         */
        size_t sq_find_2index_greater_than_size(size_t size_){
            
            size_t _num_found = 1;

            while (_num_found < size_)
            {
                _num_found = _num_found << 1;
            }

            return _num_found;
        }
        
        /**
         * @description: 
         * @param {type} element_num_ is num of elment;
         * @return: 
         */
        int8_t check_capacity_and_alloc_mem_by_element_num(size_t element_num_){

            Alloc _alloc;
            size_t _tmp_sq_capacity = 0;
            void * _tmp_sq_data_head = nullptr;
            if (element_num_ > (sq_capacity/sizeof(T))){//mem that is not used is not enough

                
                _tmp_sq_capacity = sq_find_2index_greater_than_size( element_num_ * sizeof(T));
                _tmp_sq_data_head = _alloc.allocate(std::ceil(_tmp_sq_capacity/sizeof(T)));//_tmp_sq_capacity/sizeof(T) + 1 
                
                //call construct func
                for (size_t i_ = 0; i_ < sq_size; i_++ ){

                    _alloc.construct( ((T *)_tmp_sq_data_head) + i_,  *(((T*)sq_data_head) + i_));//copy or move construct
                }


                //call destory func

                for (size_t i_ = 0; i_ < sq_size; i_++ ){

                    _alloc.destroy( ((T *)sq_data_head) + i_);
                }
                //free old mem-space
                _alloc.deallocate((T *)sq_data_head, sq_capacity);

                //new capacity , new head
                sq_data_head = _tmp_sq_data_head;
                sq_capacity = _tmp_sq_capacity;
            }

            return 0;
        }
    public:
        yLinearList() = delete;
        explicit yLinearList(size_t element_num_) MACRO_INIT_YOBJECT_PROPERTY(yLinearList) {

            sq_element_size = sizeof(T);
            check_capacity_and_alloc_mem_by_element_num(element_num_);
        }
        ~yLinearList(){

            Alloc _alloc;
            for(size_t _i = 0; _i < sq_size; _i++)
                _alloc.destroy( ((T*)sq_data_head) + _i);
            _alloc.deallocate((T *)sq_data_head, sq_capacity);
        }
        
        /**
         * @description: insert value to linearlist, class T must have operator=
         * @param :  
         * @return: 
         * @analyse:    
         *          Space-Complexity: O(1)
         * 
         *          Worst time-Complexity: O(n)
         *          Best time-Complexity: O(1)
         *          Average time-Complexity: O(n)
         */
        int8_t sq_insert(size_t pos_, T & value_){
            
            check_capacity_and_alloc_mem_by_element_num(sq_size + 1);

            Alloc _alloc;
            if (pos_ >= sq_size){//at tail
                

                //((T*)sq_data_head)[sq_size] = value_;
                _alloc.construct( ((T*)sq_data_head) + sq_size, value_);
                sq_size ++;
                return 0;
            }
            
            _alloc.construct( ((T*)sq_data_head) + sq_size, value_);//construct a obj at tail.
            for (uint64_t _i = sq_size - 1; _i >= 0; _i --){

                if (pos_ != _i){
                
                    ((T*)sq_data_head)[_i + 1] = ((T*)sq_data_head)[_i ];
                }
                else{
                
                    ((T*)sq_data_head)[_i ] = value_;
                    sq_size ++;
                    return 0;
                }
                
            }


            return 0;
        }

        /**
         * @description: delete value from linearlist, class T must have operator== and operator=
         * @param {type} 
         * @return: 
         * @analyse:    
         *          Space-Complexity: O(1)
         * 
         *          Worst time-Complexity: O(n)
         *          Best time-Complexity: O(n)
         *          Average time-Complexity: O(n)         
         */
        int8_t sq_delete(T & value_){

            Alloc _alloc;
            
            
            bool _is_get_pos = false;
            for (uint32_t _i = 0; _i <= sq_size - 2; _i ++){

                if (value_ == ((T*)sq_data_head)[_i])
                    _is_get_pos = true;

                if (_is_get_pos)
                        ((T*)sq_data_head)[_i] = ((T*)sq_data_head)[_i + 1];
                else
                    continue;
                
            }

            if (value_ == ((T*)sq_data_head)[sq_size - 1] || true == _is_get_pos){
            
                _alloc.destroy(((T*)sq_data_head) + sq_size - 1);

                sq_size --;
                return 0;
            }
            else
                return -1;
        }

        enum yLinearListOrderType:uint8_t{
            TYPE_DSC_ORDER = 0,
            TYPE_ASC_ORDER = 1
        };

        yLinearList(const yLinearList<T, Alloc> & sq_){

            sq_element_size = sizeof(T);
            check_capacity_and_alloc_mem_by_element_num(sq_.sq_size);
            for(int _i = 0; _i < sq_.sq_size; _i++){

                //call T copy assignment
                ((T*)this->sq_data_head)[_i] = ((T*)sq_.sq_data_head)[_i];
            }            
        }

        //fix a c++'s error at line(178-184)
        yLinearList<T, Alloc> & operator=(const yLinearList<T, Alloc> & sq_) {

            Alloc _alloc;
            for(int _i = 0; _i < sq_size; _i++)
                _alloc.destroy( ((T*)sq_data_head) + _i);

            _alloc.deallocate((T *)sq_data_head, sq_capacity);

            //realloc mem
            sq_element_size = sizeof(T);
            check_capacity_and_alloc_mem_by_element_num(sq_.sq_size);
            for(int _i = 0; _i < sq_.sq_size; _i++){

                //call T copy assignment
                ((T*)this->sq_data_head)[_i] = ((T*)sq_.sq_data_head)[_i];
            }
            return *this;
        }
        /**
         * @description: only support ordered Linearlist(same order-type)(ascending order or descending order)
         *                  
         * @param {type} 
         * @return: 
         * @analyse:    
         *          Space-Complexity: O(1)
         * 
         *          N=sq0_.sq_get_size() 
         *          M=sq1_.sq_get_size() 
         *          Worst time-Complexity: O(M + N)
         *          Best time-Complexity: O(Max(M,N))
         *          Average time-Complexity: O(M + N)        
         */
        int8_t sq_merge(const yLinearList<T, Alloc> & sq0_, const yLinearList<T, Alloc> & sq1_, yLinearListOrderType order_type_){

            check_capacity_and_alloc_mem_by_element_num(sq0_.sq_size + sq1_.sq_size);
            uint64_t _cur_idx = 0;
            uint64_t _bak_sq_smaller_pos = 0;
            const yLinearList<T, Alloc> * _sq_greater_ptr = &sq0_;
            const yLinearList<T, Alloc> * _sq_small_ptr = &sq1_;
            if ( sq0_.sq_size < sq1_.sq_size){

                _sq_greater_ptr = &sq1_;
                _sq_small_ptr = &sq0_;
            }

            const yLinearList<T, Alloc> & sq_greater = *_sq_greater_ptr;
            const yLinearList<T, Alloc> & sq_smaller = *_sq_small_ptr;
            for (uint64_t i_ = 0, j_ = 0; i_ < sq_greater.sq_get_size() ; ){

                if (TYPE_DSC_ORDER == order_type_){//dec

                    if (sq_greater.sq_get_value(i_) >= sq_smaller.sq_get_value(j_)){

                        sq_insert(_cur_idx ,sq_greater.sq_get_value(i_));
                        i_++;
                        _cur_idx ++;
                        //continue;
                    }
                    else{

                        sq_insert(_cur_idx , sq_smaller.sq_get_value(j_));
                        j_++;
                        _cur_idx ++;
                        //continue; 
                    }
                }
                else{//asc

                    if (sq_greater.sq_get_value(i_) <= sq_smaller.sq_get_value(j_)){

                        sq_insert(_cur_idx ,sq_greater.sq_get_value(i_));
                        i_++;
                        _cur_idx ++;
                        //continue;
                    }
                    else{

                        sq_insert(_cur_idx , sq_smaller.sq_get_value(j_));
                        j_++;
                        _cur_idx ++;
                        //continue; 
                    }
                }

                
                _bak_sq_smaller_pos = j_;
            }
            
            if (_bak_sq_smaller_pos < sq_smaller.sq_get_size())
                for (size_t j_ = _bak_sq_smaller_pos; j_ < sq_smaller.sq_get_size(); j_ ++, _cur_idx++)
                    sq_insert(_cur_idx , sq_smaller.sq_get_value(j_));

            return 0;
        }


        T & sq_get_value(uint64_t val_pos_) const{

            return ((T*)sq_data_head)[val_pos_];
        }


        size_t sq_get_size(void) const{

            return sq_size;
        }

        size_t sq_get_capacity(void) const{

            return sq_capacity;
        }
        
    };

}






#endif //__BASIC_ALGORITHM_YSEQUENCE_LIST_HPP__
