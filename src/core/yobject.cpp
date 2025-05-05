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
 * @Date: 2019-09-20 16:59:48
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-26 16:07:57
 * @Description: 
 */
#include "core/yobject.hpp"
#include "core/ylib_class_info.hpp"

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yObject)


YLIB_NAMESPACE_START




std::string Object::TypeIndex2Key(int32_t tindex)
{
    return get_classinfo_index_map()[tindex]->class_type_key_;
}

int32_t Object::TypeKey2Index(const std::string& key)
{
    return get_classinfo_name_map()[key]->class_type_index_;
}

static bool dfs(ClassInfo* parent, ClassInfo* child)
{
    if (0 == parent->child_.size()){
        if (parent == child)
            return true;
        else
            return false;
    }

    for(auto& _n: parent->child_){
        if (dfs(_n, child))
            return true;
    }

    if (parent == child)
        return true;
    else
        return false;
}

bool Object::classof(int32_t parent_tindex, uint32_t child_tindex)
{
    ClassInfo* _parent = get_classinfo_index_map()[parent_tindex];
    ClassInfo* _child = get_classinfo_index_map()[child_tindex];
    // printf("Object::classof p = %s, c = %s\n", _parent->class_name_, _child->class_name_);
    // TODO add search cache
    return dfs(_parent, _child);
}


inline void Object::inc_ref()
{
    ref_counter_.fetch_add(1, std::memory_order_relaxed);
}

inline void Object::dec_ref()
{
    if (1 == ref_counter_.fetch_sub(1, std::memory_order_release)){
        std::atomic_thread_fence(std::memory_order_acquire);
        if (nullptr != this->type_deleter_){
            (this->type_deleter_)(this);
        }
    }
}

inline int32_t Object::ref_count() const
{
    return ref_counter_.load(std::memory_order_relaxed);
}



YLIB_NAMESPACE_END