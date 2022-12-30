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
 * @Date: 2021-11-20 14:29:50
 * @LastEditTime: 2021-11-21 09:51:43
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\lex\ylexer.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_LEX_YLEXER_HPP__
#define __CORE_YCOMPILER_LEX_YLEXER_HPP__

#include "core/yobject.hpp"
#include "core/ycompiler/basic/yfile_manager.hpp"
#include "ytoken.hpp"

namespace yLib
{
    namespace ycompiler
    {

        class __YLIB_CLASS_DECLSPEC__ yLexer:
        YLIB_PUBLIC_INHERIT_YOBJECT
        {
            private:
            const char * buf_start;
            const char * buf_end;
            const char * buf_cur_ptr;
            yFileManager * file_mgr;

            public:
            yLexer() = delete;
            yLexer(yFileManager * file_mgr);
            ~yLexer();
            
            yFileManager * GetFileManager(void){return file_mgr;}
            bool BackToPos(uint64_t pos);

            bool GetNextChar(char &c);

            bool HasNextChar(void);
            bool HasNextChar(const char * ptr);

            bool IsEof(void);
            bool IsEof(const char * ptr);


            void UpdateToken(yToken &token, const char *tok_end, tok::yTokenKind kind);

            bool GetEndOfFileToken(yToken &token, const char * cur_ptr);

            bool IsNumericChar(const char * cur_ptr);

            bool GetNumericConstant(yToken &token, const char * cur_ptr);

            bool IsIdentifierBodyChar(const char * cur_ptr);
            bool GetIdentifier(yToken &token, const char * cur_ptr);

            bool GetCharConstant(yToken &token, const char * cur_ptr);

            bool GetStringLiteral(yToken &token, const char * cur_ptr);

            bool TryNextToken(yToken & token);
            bool NextToken(yToken & token);
        };
    } // namespace ycompiler
} // namespace yLib


#endif //__CORE_YCOMPILER_LEX_YLEXER_HPP__