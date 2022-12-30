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
 * @Date: 2021-11-20 12:08:28
 * @LastEditTime: 2021-11-21 10:05:02
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\frontend\yfrontend_action.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#ifndef __CORE_YCOMPILER_BASIC_YFRONTEND_ACTION_HPP__
#define __CORE_YCOMPILER_BASIC_YFRONTEND_ACTION_HPP__

#include "core/yobject.hpp"

namespace yLib
{
    namespace ycompiler
    {
        class yCompilerInstance;

        class __YLIB_CLASS_DECLSPEC__ yFrontendAction:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            protected:
            yCompilerInstance *ci = nullptr;
            public:
            yFrontendAction();
            virtual ~yFrontendAction();
            virtual bool Execute(void) = 0;

            yCompilerInstance & GetCompilerInstance(void);
            void SetCompilerInstance(yCompilerInstance * ins);

        };

        class yDoNothingFrontendAction:public yFrontendAction{

            public:
            yDoNothingFrontendAction(){}
            ~yDoNothingFrontendAction(){}
            bool Execute(void){ return true;}
        };

        enum ActionKind {
            //do nothing
            DO_NOTHING,

            /// Parse ASTs for yconfig
            PARSE_YCONFIG_AST,
        };
        
        class yFrontendOptions {

            public:

            ActionKind act_type = ActionKind::DO_NOTHING;
        };
    } // namespace ycompiler
} // namespace yLib



#endif //__CORE_YCOMPILER_BASIC_YFRONTEND_ACTION_HPP__