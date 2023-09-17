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
 * @Date: 2022-12-04 13:50:41
 * @LastEditTime: 2022-12-04 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\tools\yutils.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#include "core/ycompiler/tools/yutils.hpp"

#include "core/ycompiler/tools/yconfig_parse_action.hpp"
#include "core/ycompiler/ast/yast_consumer.hpp"
#include "core/ycompiler/frontend/ycompiler_instance.hpp"
#include "core/ycompiler/frontend/yfrontend_action.hpp"

#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/ast/ydecl_group.hpp"


#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

std::unique_ptr<yFrontendAction> yLib::ycompiler::CreateFrontendAction(yCompilerInstance & ci)
{

    switch(ci.GetCompilerInvocationHelper().getFrontendOpts().act_type){

        case ActionKind::PARSE_YCONFIG_AST:{

            return std::unique_ptr<yFrontendAction>(new yConfigParseAction(&ci));
            break;
        }
        
        default:{

            break;
        }

    }

    return std::unique_ptr<yFrontendAction>(new yDoNothingFrontendAction());
}


bool yLib::ycompiler::ExecuteCompiler(yCompilerInstance & ci)
{
    return true;
}

bool yLib::ycompiler::ParseAST(yCompilerInstance &ci)
{
    std::unique_ptr<ycompiler::yLexer> _lexer = \
        std::unique_ptr<ycompiler::yLexer>(new ycompiler::yLexer(ci));
    ci.SetLexer(std::move(_lexer));

    std::unique_ptr<ycompiler::ySema> _sema = \
        std::unique_ptr<ycompiler::ySema>(new ycompiler::ySema(ci));
    ci.SetSema(std::move(_sema));

    std::unique_ptr<ycompiler::yDiagnosticsIDHandle> _diag_id = \
        std::unique_ptr<ycompiler::yDiagnosticsIDHandle>(new ycompiler::yDiagnosticsIDHandle());
    std::unique_ptr<ycompiler::yDiagnosticOptions> _diag_opts = \
        std::unique_ptr<ycompiler::yDiagnosticOptions>(new ycompiler::yDiagnosticOptions());
    std::unique_ptr<ycompiler::yDiagnosticConsumer> _diag_consumer = \
        std::unique_ptr<ycompiler::yDiagnosticConsumer>(new ycompiler::yDiagnosticConsumer());
    std::unique_ptr<ycompiler::yDiagnosticsEngine> _diag_engine = \
        std::unique_ptr<ycompiler::yDiagnosticsEngine>(new ycompiler::yDiagnosticsEngine(std::move(_diag_id), std::move(_diag_opts), std::move(_diag_consumer), ci.GetSourceManager()));

    ci.SetDiagnosticsEngine(std::move(_diag_engine));


    std::unique_ptr<ycompiler::yParser> _parser = \
        std::unique_ptr<ycompiler::yParser>(new ycompiler::yConfigParser(ci));
    ci.SetParser(std::move(_parser));


    yASTConsumer & _consumer = ci.GetSema().GetASTConsumer();


    // static_cast<yConfigParser&>(ci.GetParser()).Initialize();
    ci.GetParser().Initialize();
    std::unique_ptr<yDeclGroup> _decl_group = std::unique_ptr<yDeclGroup>(new yDeclGroup());
    

    for (bool _is_eof = ci.GetParser().ParseFirstTopLevelDecl(*_decl_group.get()); \
        !_is_eof; _is_eof = ci.GetParser().ParseTopLevelDecl(*_decl_group.get(), false)){
        
        if (ci.GetParser().has_error())
            return false;

        //add decl group to _consumer
        if (_decl_group != nullptr && !_consumer.HandleTopLevelDecl(*_decl_group.get()))
            return false;

        _decl_group = std::unique_ptr<yDeclGroup>(new yDeclGroup());//reset decl_group
    } 

    _consumer.HandleTranslationUnit(ci.GetSema().GetASTContext());

    return true;
}
