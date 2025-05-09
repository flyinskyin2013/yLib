/*
 * @Author: Sky sky@sky-home.com
 * @Date: 2022-12-30 19:25:05
 * @LastEditors: Sky sky@sky-home.com
 * @LastEditTime: 2023-04-16 15:51:28
 * @FilePath: \yLib\include\core\ycompiler\ast\yast_consumer.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
 * @FilePath: \yLib\include\core\ycompiler\ast\yast_consumer.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_AST_CONSUMER_HPP__
#define __CORE_YCOMPILER_AST_AST_CONSUMER_HPP__


#include <memory>
#include <string>

#include "core/ycompiler/ast/recursive_ast_visitor.hpp"
#include "core/ycompiler/ast/yast_context.hpp"

namespace yLib
{
    namespace ycompiler
    {
        class yDeclGroup;
        class yASTContext;
        class yDecl;

        /// ASTConsumer - This is an abstract interface that should be implemented by
        /// clients that read ASTs.  This abstraction layer allows the client to be
        /// independent of the AST producer (e.g. parser vs AST dump file reader, etc).
        class yASTConsumer{
            public:
            virtual ~yASTConsumer() {}
            /// Initialize - This is called to initialize the consumer, providing the
            /// ASTContext.
            virtual void Initialize(yASTContext &ast_ctx) {}
            /// HandleTopLevelDecl - Handle the specified top-level declaration.  This is
            /// called by the parser to process every top-level Decl*.
            ///
            /// \returns true to continue parsing, or false to abort parsing.
            virtual bool HandleTopLevelDecl(yDeclGroup &decl){ return true;}    

            /// HandleTranslationUnit - This method is called when the ASTs for entire
            /// translation unit have been parsed.
            virtual void HandleTranslationUnit(yASTContext &ast_ctx) {}        
        };


        class yConfigASTReader:public yASTConsumer,
                                public RecursiveASTVisitor<yConfigASTReader>{

            yASTContext * ast_ctx;
            
            std::unique_ptr<yDeclGroup> decl_group;

            public:
            yConfigASTReader();
            /// Initialize - This is called to initialize the consumer, providing the
            /// ASTContext.
            void Initialize(yASTContext &ast_ctx) override;
            /// HandleTopLevelDecl - Handle the specified top-level declaration.  This is
            /// called by the parser to process every top-level Decl*.
            ///
            /// \returns true to continue parsing, or false to abort parsing.
            bool HandleTopLevelDecl(yDeclGroup &decl) override;        

            void SetDeclGroup(std::unique_ptr<yDeclGroup> && decl_group);
            yDeclGroup & GetDeclGroup(void);    

            yDecl * GetDecl(const std::string & path, yDecl * parent);   
        };

        class yConfigASTWriter: public yASTConsumer,
                                public RecursiveASTVisitor<yConfigASTWriter>{
            
            typedef RecursiveASTVisitor<yConfigASTWriter> base_visitor;
        public:
            yConfigASTWriter();

            /// Initialize - This is called to initialize the consumer, providing the
            /// ASTContext.
            void Initialize(yASTContext &context) override;
   
            /// HandleTranslationUnit - This method is called when the ASTs for entire
            /// translation unit have been parsed.  
            void HandleTranslationUnit(yASTContext &context) override;

            bool TraverseDecl(yDecl *decl);
        };

        class ConfigASTPrinter: public yASTConsumer,
                                public RecursiveASTVisitor<ConfigASTPrinter>{
            
            typedef RecursiveASTVisitor<ConfigASTPrinter> base_visitor;
        public:
            ConfigASTPrinter();

            /// Initialize - This is called to initialize the consumer, providing the
            /// ASTContext.
            void Initialize(yASTContext &context) override;
   
            /// HandleTranslationUnit - This method is called when the ASTs for entire
            /// translation unit have been parsed.  
            void HandleTranslationUnit(yASTContext &context) override;

            bool TraverseDecl(yDecl *decl);
        };
        
    } // namespace ycompiler
} // namespace yLib

#endif //__CORE_YCOMPILER_AST_AST_CONSUMER_HPP__