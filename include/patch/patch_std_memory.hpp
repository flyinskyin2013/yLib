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
 * @Date: 2023-09-23 10:26:28
 * @LastEditors: Sky
 * @LastEditTime: 2023-09-23 13:56:40
 * @Description: 
 */


#ifndef __YLIB_PATCH_PATCH_STD_MEMORY_HPP__
#define __YLIB_PATCH_PATCH_STD_MEMORY_HPP__

#include <memory>

namespace yLib{

    #if (__cplusplus == 201103L)//patch for cxx 11

        //patch make_unique  start
        //patch for support make_unique in cxx 11
        //https://isocpp.org/files/papers/N3656.txt
        template<class T> struct _Unique_if {
            typedef std::unique_ptr<T> _Single_object;
        };

        template<class T> struct _Unique_if<T[]> {
            typedef std::unique_ptr<T[]> _Unknown_bound;
        };

        template<class T, size_t N> struct _Unique_if<T[N]> {
            typedef void _Known_bound;
        };

        template<class T, class... Args>
            typename _Unique_if<T>::_Single_object
            make_unique(Args&&... args) {
                return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
            }

        template<class T>
            typename _Unique_if<T>::_Unknown_bound
            make_unique(size_t n) {
                /*
                    Defined in header <type_traits>
                    If T is an array of some type X, provides the member typedef type equal to X, otherwise type is T. 
                        Note that if T is a multidimensional array, only the first dimension is removed.
                    The behavior of a program that adds specializations for std::remove_extent is undefined.
                */
                typedef typename remove_extent<T>::type U;
                return std::unique_ptr<T>(new U[n]());
            }

        template<class T, class... Args>
            typename _Unique_if<T>::_Known_bound
            make_unique(Args&&...) = delete; 

        //patch make_unique  end       
    #endif //
}


#endif //__YLIB_PATCH_PATCH_STD_MEMORY_HPP__