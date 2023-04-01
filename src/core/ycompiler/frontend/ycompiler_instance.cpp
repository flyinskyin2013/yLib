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
 * @FilePath: /yLib/src/core/ycompiler/frontend/ycompiler_instance.cpp
 * https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/frontend/ycompiler_instance.hpp"
#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yCompilerInstance::yCompilerInstance()
:file_mgr(nullptr),
lexer(nullptr),
parser(nullptr),
diagnostics_engine(nullptr),
invocation_helper(nullptr)
{
    std::unique_ptr<ycompiler::yFileManager> _file_mgr = std::unique_ptr<ycompiler::yFileManager>(ycompiler::yFileManager::GetInstance(*this));
    std::unique_ptr<ycompiler::yLexer> _lexer = \
        std::unique_ptr<ycompiler::yLexer>(new ycompiler::yLexer(*this));

    std::unique_ptr<ycompiler::yCompilerInvocationHelper> _invocation_helper = std::unique_ptr<ycompiler::yCompilerInvocationHelper>(new ycompiler::yCompilerInvocationHelper(*this));


    this->SetFileManager(std::move(_file_mgr));
    this->SetLexer(std::move(_lexer));
    this->SetCompilerInvocationHelper(std::move(_invocation_helper));
}

yCompilerInstance::~yCompilerInstance(){
    
}

bool yCompilerInstance::ExecuteAction(yFrontendAction &act){

    return act.Execute();
}

void yCompilerInstance::SetFileManager(std::unique_ptr<yFileManager> && file_mgr){

    this->file_mgr = std::move(file_mgr);
}


yFileManager & yCompilerInstance::GetFileManger(void){

    return *file_mgr.get();
}

void yCompilerInstance::SetLexer(std::unique_ptr<yLexer> && lexer){

    this->lexer = std::move(lexer);
}
yLexer & yCompilerInstance::GetLexer(void){

    return *lexer.get();
}

void yCompilerInstance::SetParser(std::unique_ptr<yParser> && parser)
{
    this->parser = std::move(parser);
}

yParser & yCompilerInstance::GetParser(void)
{
    return *parser.get();
}

void yCompilerInstance::SetDiagnosticsEngine(std::unique_ptr<yDiagnosticsEngine> && diagnostic_engine)
{
    this->diagnostics_engine = std::move(diagnostics_engine);
}

yDiagnosticsEngine & yCompilerInstance::GetDiagnosticsEngine(void)
{
    return *diagnostics_engine.get();
}

void yCompilerInstance::SetCompilerInvocationHelper(std::unique_ptr<yCompilerInvocationHelper> && invocation_helper)
{
    this->invocation_helper = std::move(invocation_helper);
}


yCompilerInvocationHelper & yCompilerInstance::GetCompilerInvocationHelper(void)
{
    return *invocation_helper.get();
}

void yCompilerInstance::SetSema(std::unique_ptr<ySema> && sema)
{
    this->sema = std::move(sema);
}

ySema & yCompilerInstance::GetSema(void)
{

    return *sema.get();
}