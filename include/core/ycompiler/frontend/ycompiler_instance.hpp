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
 * @LastEditTime: 2021-11-27 10:59:49
 * @Description: 
 * @FilePath: /yLib/include/core/ycompiler/frontend/ycompiler_instance.hpp
 * https://github.com/flyinskyin2013/yLib
 */



#ifndef __CORE_YCOMPILER_BASIC_YCOMPILER_INSTANCE_HPP__
#define __CORE_YCOMPILER_BASIC_YCOMPILER_INSTANCE_HPP__

#include "yfrontend_action.hpp"
#include "ycompiler_invocation_helper.hpp"


#include "core/ycompiler/basic/yfile_manager.hpp"
#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/lexer/ylexer.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"
#include "core/ycompiler/lexer/ypreprocessor.hpp"

#include <memory>

namespace yLib
{
    namespace ycompiler
    {

        /// CompilerInstance - Helper class for managing a single instance of the ycompiler.
        ///
        /// The CompilerInstance serves two purposes:
        ///  (1) It manages the various objects which are necessary to run the compiler,
        ///      for example the preprocessor, the target information, and the AST
        ///      context.
        ///  (2) It provides utility routines for constructing and manipulating the
        ///      common Clang objects.

        class yCompilerInstance{
            private:
            std::unique_ptr<yFileManager> file_mgr;
            std::unique_ptr<yLexer> lexer;
            std::unique_ptr<yParser> parser;
            std::unique_ptr<yDiagnosticsEngine> diagnostics_engine;
            std::unique_ptr<yCompilerInvocationHelper> invocation_helper;
            //yASTContext, yASTConsumer in ySema
            std::unique_ptr<ySema> sema;
            // std::unique_ptr<yPreprocessor> preprocessor;

            public:
            yCompilerInstance();
            ~yCompilerInstance();
            bool ExecuteAction(yFrontendAction &act);

            void SetFileManager(std::unique_ptr<yFileManager> && file_mgr);
            yFileManager & GetFileManger(void);

            void SetLexer(std::unique_ptr<yLexer> && lexer);
            yLexer & GetLexer(void);

            void SetParser(std::unique_ptr<yParser> && parser);
            yParser & GetParser(void);

            void SetDiagnosticsEngine(std::unique_ptr<yDiagnosticsEngine> && diagnostic_engine);
            yDiagnosticsEngine & GetDiagnosticsEngine(void);

            void SetCompilerInvocationHelper(std::unique_ptr<yCompilerInvocationHelper> && invocation_helper);
            yCompilerInvocationHelper & GetCompilerInvocationHelper(void);

            void SetSema(std::unique_ptr<ySema> && sema);
            ySema & GetSema(void);
        };
    } // namespace ycompiler
} // namespace yLib


#endif //__CORE_YCOMPILER_BASIC_YCOMPILER_INSTANCE_HPP__