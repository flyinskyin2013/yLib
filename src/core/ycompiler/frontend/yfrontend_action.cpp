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
 * @Date: 2021-11-20 13:22:06
 * @LastEditTime: 2021-11-20 13:23:15
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\frontend\yaction.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/frontend/yfrontend_action.hpp"
#include "core/ycompiler/frontend/ycompiler_instance.hpp"

#include "core/ylog/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yFrontendAction::yFrontendAction()
{

}

yFrontendAction::~yFrontendAction(){
    
}

yCompilerInstance & yFrontendAction::GetCompilerInstance(void){

    return *ci;
}
void yFrontendAction::SetCompilerInstance(yCompilerInstance * ins){

    ci = ins;
}

bool yFrontendAction::BeginSourceFile(yCompilerInstance &CI, yFileID fid)
{
    CI.GetSourceManager().setMainFileID(fid);
    return true;
}

/// Perform any per-file post processing, deallocate per-file
/// objects, and run statistics and output file cleanup code.
void yFrontendAction::EndSourceFile()
{

}