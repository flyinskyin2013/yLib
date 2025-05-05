#ifndef __YLIB_CORE_YLIB_CLASS_INFO_HPP__
#define __YLIB_CORE_YLIB_CLASS_INFO_HPP__

#include "ydefs.hpp"
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>

YLIB_NAMESPACE_START

    struct ClassInfo{
        std::vector<ClassInfo*> child_;
        const char* class_type_key_;
        const char* class_name_;
        int32_t class_type_index_;
    };
    
    std::vector<ClassInfo*>& get_classinfo_tree_root_vec();
    std::unordered_map<std::string, ClassInfo*>& get_classinfo_name_map();
    std::unordered_map<int32_t, ClassInfo*>& get_classinfo_index_map();
YLIB_NAMESPACE_END

#endif //__YLIB_CORE_YLIB_CLASS_INFO_HPP__