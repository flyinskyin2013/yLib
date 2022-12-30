/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: your name
 * @Date: 2021-11-27 14:16:42
 * @LastEditTime: 2021-11-27 14:16:43
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\sema\ysema.hpp
 */

#ifndef __CORE_YCOMPILER_PARSE_YSEMA_HPP__
#define __CORE_YCOMPILER_PARSE_YSEMA_HPP__

#include "core/yobject.hpp"

#include "core/ycompiler/ast/yast_context.hpp"
#include "core/ycompiler/ast/yast_consumer.hpp"

#include "core/ycompiler/lexer/ypreprocessor.hpp"

#include <cstdint>

namespace yLib
{
    namespace ycompiler
    {
        class yConfigParser;

        //semantic analysis
        class __YLIB_CLASS_DECLSPEC__ ySema:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            private:
            yConfigParser *parser;
            yPreprocessor *preprocessor;
            yASTContext *ast_ctx;
            yASTConsumer *ast_consumer;

            public:
            ySema() = delete;
            ySema(yPreprocessor &preprocessor, yASTContext &ast_ctx, yASTConsumer &ast_consumer);
            ySema(yConfigParser * parser):parser(parser){}
            ~ySema(){}

            /**
             *  @fn      int8_t ParseNumberConstantExpression(uint64_t & num)
             *  @brief   parse num-constant-expr
             *  @param   num the num parsed.
             *  @return the num type or ret-status
             *  @retval -1 error
             *  @retval 1 int-constant
             *  @retval 2 double-constant
             * 
             */
            int8_t ParseNumberConstantExpression(uint64_t & num);


            yPreprocessor &GetPreprocessor(){return *preprocessor;}
            yASTContext &GetASTContext(){return *ast_ctx;}
            yASTConsumer &GetASTConsumer(){return *ast_consumer;}
        };
    }
}


#endif //__CORE_YCOMPILER_PARSE_YSEMA_HPP__