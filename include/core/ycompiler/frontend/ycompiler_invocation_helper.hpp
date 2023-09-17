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
 * @Date: 2022-12-04 13:53:41
 * @LastEditTime: 2022-12-04 15:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\frontend\ycompiler_invocation_helper.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_FRONTEND_YCOMPILER_INVOCATION_HELPER_HPP__
#define __CORE_YCOMPILER_FRONTEND_YCOMPILER_INVOCATION_HELPER_HPP__

#include "core/ycompiler/frontend/yfrontend_action.hpp"
#include <memory>

namespace yLib
{
    namespace ycompiler
    {
        class yCompilerInstance;

        // Helper class for holding the data necessary to invoke the compiler.
        class yCompilerInvocationHelper{
            private:
            /// Options controlling the frontend itself.
            yFrontendOptions frontend_opts;

            yCompilerInstance & ci;
            public:
            yCompilerInvocationHelper() = delete;
            yCompilerInvocationHelper(yCompilerInstance & ci);
            ~yCompilerInvocationHelper();

            yFrontendOptions &getFrontendOpts() { return frontend_opts; }
            const yFrontendOptions &getFrontendOpts() const { return frontend_opts; }
        };
    } // namespace ycompiler
} // namespace yLib

#endif //__CORE_YCOMPILER_FRONTEND_YCOMPILER_INVOCATION_HELPER_HPP__