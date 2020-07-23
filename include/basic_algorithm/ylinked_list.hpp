/*
 * @Author: Sky
 * @Date: 2020-07-01 15:03:19
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-16 16:45:04
 * @Description: 
 */ 

#include "core/yobject.hpp"
#include "core/yallocator.hpp"

#include <cstdint>

namespace yLib{

    template<class T>
    struct yLinkedListNode{

        yLinkedListNode(){}
        explicit yLinkedListNode(const T & val_){node_elemnt = val_;}

        T node_elemnt;

        yLinkedListNode * before = nullptr;
        yLinkedListNode * next = nullptr;
    };
    /**
     * @description: LinkedList implement
     * @param {type} 
     * @return: 
     */
    template<class T, class Alloc = std::allocator<yLinkedListNode<T>>>
    class yLinkedList MACRO_PUBLIC_INHERIT_YOBJECT
    {
        public:
        yLinkedList() noexcept MACRO_INIT_YOBJECT_PROPERTY(yLinkedList){node_count = 0;}

        void insert_head(const T & node_){

            Alloc _alloc;
            yLinkedListNode<T> * _new_node = _alloc.allocate(1);

            
            //set val
            _alloc.construct(_new_node, yLinkedListNode<T>(node_));


            if (nullptr == head){

                head = _new_node;
                tail = _new_node;
            }
            else
            {

                _new_node->next = head;
                head->before = _new_node;

                head = _new_node;
            }
            node_count++;
            
        }
        void insert_tail(const T & node_){

            Alloc _alloc;
            yLinkedListNode<T> * _new_node = _alloc.allocate(1);
            //set val
            _alloc.construct(_new_node, yLinkedListNode<T>(node_));


            if (nullptr == head){

                head = _new_node;
                tail = _new_node;
            }
            else
            {

                _new_node->before = tail;
                tail->next = _new_node;

                tail = _new_node;
            }
            node_count++;
        }

        
        void remove_head(void){
            
            if (nullptr == head)
                return ;
            Alloc _alloc;
            yLinkedListNode<T> * _delete_node = head;
            head = head->next;
            head->before = nullptr;
            _alloc.destroy(_delete_node);
            _alloc.deallocate(_delete_node, 1);
            node_count --;
        }

        void remove_tail(void){
            
            if (nullptr == head)
                return ;
            Alloc _alloc;
            yLinkedListNode<T> * _delete_node = tail;
            tail = tail->before;
            tail->next = nullptr;
            _alloc.destroy(_delete_node);
            _alloc.deallocate(_delete_node, 1);  
            node_count --;          
        }

        bool remove(T & node_){

            if (nullptr == head)
                return false;         

            yLinkedListNode<T> * _cur_node = head;
            while (nullptr != _cur_node)
            {
                if (_cur_node->node_elemnt == node_){
                
                    Alloc _alloc;
                    yLinkedListNode<T> * _delete_node = _cur_node;

                    _cur_node->before->next = _cur_node->next ;
                    _cur_node->next->before = _cur_node->before;

                    _alloc.destroy(_delete_node);
                    _alloc.deallocate(_delete_node, 1);  
                
                    node_count --;
                    return true;
                }
                
                _cur_node = _cur_node->next;
            }
            
            return false;
        }


        void reverse(void){

            if (0 == node_count || 1 == node_count)
                return ;

            yLinkedListNode<T> * _cur_node = head;
            yLinkedListNode<T> * _next_node = _cur_node->next;

            while(_cur_node != tail){

                _cur_node->next = _cur_node->before;
                _cur_node->before = _cur_node->next;

                _cur_node = _next_node;
                _next_node = _next_node->next;
            }

            _cur_node->next = _cur_node->before;
            _cur_node->before = _cur_node->next;     

            tail = head; 
            head = _cur_node; 
                 
        }


        uint64_t get_node_count(void) const{

            return node_count;
        }

        T & get_element(uint64_t idx_) const{

            yLinkedListNode<T> * _cur_node = head;
            while(0 != idx_){

                _cur_node = _cur_node->next;
                idx_ --;
            }
            
            return _cur_node->node_elemnt;
        }

        private:
        yLinkedListNode<T> * head = nullptr;
        yLinkedListNode<T> * tail = nullptr;
        uint64_t node_count;
    };

    
}