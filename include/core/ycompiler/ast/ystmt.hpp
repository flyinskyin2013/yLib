/*
Copyright (c) 2018 - 2023 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * @Author: Sky
 * @Date: 2023-03-19 13:50:41
 * @LastEditTime: 2023-03-19 19:50:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\ystmt.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __CORE_YCOMPILER_AST_YSTMT_HPP__
#define __CORE_YCOMPILER_AST_YSTMT_HPP__

#include <vector>


namespace yLib
{
    namespace ycompiler
    {
        class yDecl;
        
        /// Stmt - This represents one statement.
        ///
        class yStmt {
        public:
            enum StmtClass {
                NoStmtClass = 0,
                #define STMT(CLASS, PARENT) CLASS##Class,
                #define STMT_RANGE(BASE, FIRST, LAST) \
                    first##BASE##Constant=FIRST##Class, last##BASE##Constant=LAST##Class,
                #define LAST_STMT_RANGE(BASE, FIRST, LAST) \
                    first##BASE##Constant=FIRST##Class, last##BASE##Constant=LAST##Class
                #define ABSTRACT_STMT(STMT) STMT
                #include "stmt_nodes_kinds.def"
            };
        protected:
            StmtClass stmt_kind = StmtClass::NoStmtClass;

        public:
            StmtClass get_stmt_kind(){ return stmt_kind;}
        };

        /// CompoundStmt - This represents a group of statements like { stmt stmt }.
        class yCompoundStmt final : public yStmt{
            private:
            std::vector<yStmt*> stmt_vec;

            yCompoundStmt(std::vector<yStmt*> && stmts):stmt_vec(stmts){
                stmt_kind = yStmt::StmtClass::yCompoundStmtClass;
            }
            //yCompoundStmt(std::vector<yStmt*> & stmts):stmt_vec(stmts){}
            public:
            static yStmt* Create(std::vector<yStmt*> && stmts){

                return (yStmt*) new yCompoundStmt(std::move(stmts));
            }

            std::vector<yStmt*> & get_stmt_vec(void){return stmt_vec;}
        };


        /// DeclStmt - Adaptor class for mixing declarations with statements and
        /// expressions. For example, CompoundStmt mixes statements, expressions
        /// and declarations (variables, types). Another example is ForStmt, where
        /// the first statement can be an expression or a declaration.
        class yDeclStmt : public yStmt {

            yDecl * decl;
            ySourceLocation loc;
            public:
            yDeclStmt(yDecl*decl, ySourceLocation loc):decl(decl), loc(loc){
                stmt_kind = yStmt::StmtClass::yDeclStmtClass;
            }
            yDecl * get_decl(){return decl;}
        };


        /// Represents a statement that could possibly have a value and type. This
        /// covers expression-statements, as well as labels and attributed statements.
        ///
        /// Value statements have a special meaning when they are the last non-null
        /// statement in a GNU statement expression, where they determine the value
        /// of the statement expression.
        class yValueStmt : public yStmt {
            public:
            yValueStmt(){
                stmt_kind = yStmt::StmtClass::yValueStmtClass;
            }
        };
    } // namespace ycompiler
    
} // namespace yLib






#endif //__CORE_YCOMPILER_AST_YSTMT_HPP__