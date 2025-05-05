//ref: https://github.com/llvm/llvm-project/blob/main/llvm/include/llvm/Support/Casting.h

#ifndef __CORE_RTTI_HPP__
#define __CORE_RTTI_HPP__

#include "core/ydefs.hpp"

YLIB_NAMESPACE_START

class yObject;

template <typename To, typename From> inline bool isa(const From *Val) {
    // return To::classof(To::type_index(), From::type_index());
    return To::classof(yLib::yObject::TypeKey2Index(To::static_type_key()), const_cast<From*>(Val)->type_index());
}

template <typename To, typename From> inline To *cast(From *Val) {

    // https://en.cppreference.com/w/cpp/language/static_cast
    // static_cast: 1 downcast/upcast. 7g void* -> obj
    // convert by compiler, compiler knows From-real-type
    return static_cast<To *>((void*)Val);
}

template <typename To, typename From> inline To *dyn_cast(From *Val) {
    return isa<To>(Val) ? cast<To>(Val) : nullptr;
}

YLIB_NAMESPACE_END

#endif //__CORE_RTTI_HPP__