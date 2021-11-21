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
 * @Date: 2021-11-20 09:22:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-27 10:49:38
 * @Description: 
 * @FilePath: /yLib/src/core/ycompiler/basic/ycompiler_instance.cpp
 * https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/basic/ycompiler_instance.hpp"
#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yCompilerInstance::yCompilerInstance()
:file_mgr(nullptr),
lex(nullptr),
parser(nullptr)
{

}

yCompilerInstance::~yCompilerInstance(){
    
}

bool yCompilerInstance::ExecuteAction(yAction &act){

    return act.Execute();
}

void yCompilerInstance::SetFileManager(yFileManager * file_mgr){

    this->file_mgr = std::unique_ptr<yFileManager>(file_mgr);
}


yFileManager * yCompilerInstance::GetFileManger(void){

    return file_mgr.get();
}

void yCompilerInstance::SetLexer(yLexer * lexer){

    lex = std::unique_ptr<yLexer>(lexer);
}
yLexer * yCompilerInstance::GetLexer(void){

    return lex.get();
}

void yCompilerInstance::SetParser(yParser * parser)
{
    this->parser = std::unique_ptr<yParser>(parser);
}

yParser * yCompilerInstance::GetParser(void)
{
    return parser.get();
}
