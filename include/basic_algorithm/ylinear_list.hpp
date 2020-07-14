/*
 * @Author: Sky
 * @Date: 2020-05-22 09:54:19
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 17:41:40
 * @Description: 
 */

#ifndef __BASIC_ALGORITHM_YSEQUENCE_LIST_HPP__
#define __BASIC_ALGORITHM_YSEQUENCE_LIST_HPP__

#include <memory>

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
        size_t sq_size = 0;
        size_t sq_capacity = 1;

        void * sq_data_head = nullptr;


        /**
         * @description: find a num(index of 2) is greater than size_
         * @param {type} 
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

        int8_t check_capacity_and_alloc_mem_by_size(size_t size_){

            Alloc _alloc;
            size_t _tmp_sq_capacity = 0;
            void * _tmp_sq_data_head = nullptr;
            if (size_ > (sq_capacity/sizeof(T))){//mem is not enough

                
                _tmp_sq_capacity = sq_find_2index_greater_than_size(size_ * sizeof(T));
                _tmp_sq_data_head = _alloc.allocate(_tmp_sq_capacity);
                
                //call construct func
                for (int i_ = 0; i_ < _tmp_sq_capacity; i_++ ){

                    T _tmp_data;
                    _alloc.construct( ((T *)_tmp_sq_data_head) + i_,  _tmp_data);
                }

                memcpy(_tmp_sq_data_head, sq_data_head, sq_size * sizeof(T));


                //call destory func
                for (int i_ = 0; i_ < _tmp_sq_capacity; i_++ ){

                    _alloc.destroy( ((T *)_tmp_sq_data_head) + i_);
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

            check_capacity_and_alloc_mem_by_size(element_num_);
        }
        ~yLinearList(){

            Alloc _alloc;
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
            
            if (pos_ < 0 )
                return -1;

            check_capacity_and_alloc_mem_by_size(sq_size + 1);

            if (pos_ >= sq_size){//at tail
                
                ((T*)sq_data_head)[sq_size] = value_;
                sq_size ++;
                return 0;
            }
            

            for (int64_t _i = sq_size - 1; _i != -1; _i --){

                if (pos_ != _i)
                    ((T*)sq_data_head)[_i + 1] = ((T*)sq_data_head)[_i ];
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

        //fix a c++'s error at line(178-184)
        const yLinearList<T, Alloc> & operator=(const yLinearList<T, Alloc> & sq_) const{

            return sq_;
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

            check_capacity_and_alloc_mem_by_size(sq0_.sq_size + sq1_.sq_size);
            uint64_t _cur_idx = 0;
            uint64_t _bak_sq_smaller_pos = 0;
            const yLinearList<T, Alloc> & sq_greater = sq0_;
            const yLinearList<T, Alloc> & sq_smaller = sq1_;
            if ( sq0_.sq_size < sq1_.sq_size){

                sq_greater = sq1_;
                sq_smaller = sq0_;
            }

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
                for (int j_ = _bak_sq_smaller_pos; j_ < sq_smaller.sq_get_size(); j_ ++, _cur_idx++)
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
