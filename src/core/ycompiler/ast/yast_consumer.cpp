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
 * @Date: 2023-08-26 14:29:50
 * @LastEditTime: 2023-08-26 17:51:43
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\include\core\ycompiler\ast\yast_consumer.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/ast/yast_consumer.hpp"
#include "core/ycompiler/ast/ydecl_group.hpp"
#include "core/ycompiler/ast/ydecl.hpp"


#include "core/ylog/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;


yConfigASTReader::yConfigASTReader(){

    decl_group = std::unique_ptr<yDeclGroup>(new yDeclGroup());
}

/// Initialize - This is called to initialize the consumer, providing the
/// ASTContext.
void yConfigASTReader::Initialize(yASTContext &ast_ctx)
{
    this->ast_ctx = &ast_ctx;
}

static void PrintDecl(yDecl* decl, int level = 0);
inline void PrintStmt(yStmt * stmt, int level = 0);

void get_level_prefix(std::string & prefix, int level)
{
    std::string _indicator = "  ->";
    if (0 == level || 1 == level){

        if (0 == level)
            prefix = "";
        
        if (1 == level)
            prefix = _indicator;
    }
    else{

        for(int _i = 0; _i < level-1; _i++){

            prefix += "    ";
        }

        prefix += _indicator;
    }


}

#ifndef YLIB_CODECOVERAGE_SKIP_CODE
inline void PrintStmt(yStmt * stmt, int level)
{
    std::string _level_prefix = "";
    get_level_prefix(_level_prefix, level);

    switch (stmt->get_stmt_kind())
    {
    case yStmt::NoStmtClass:{
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "No StmtClass").c_str());
        break;
    }
    case yStmt::CompoundStmtClass:{
        
        printf((_level_prefix + "yCompoundStmtClass, have %d stmts.\n").c_str(),  ((yCompoundStmt*)stmt)->get_stmt_vec().size());
        break;
    }
    case yStmt::DeclStmtClass:{
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "yDeclStmtClass").c_str());
        PrintDecl(((yDeclStmt*)stmt)->get_decl(), level + 1);
        break;
    }
    case yStmt::ValueStmtClass:{
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "yValueStmtClass").c_str());
        break;
    }
    case yStmt::ExprClass:{
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "yExprClass").c_str());
        break;
    }
    case yStmt::IntegerLiteralClass:{
        
        yIntegerLiteral * _literal = (yIntegerLiteral *)stmt;
        printf((_level_prefix + "yIntegerLiteralClass, val = %lu\n").c_str(), _literal->get_val());
        break;
    }
    case yStmt::FloatingLiteralClass:{

        yFloatingLiteral * _literal = (yFloatingLiteral *)stmt;
        printf((_level_prefix + "yFloatingLiteralClass, val = %E\n").c_str(), _literal->get_val());
        break;
    }
    case yStmt::StringLiteralClass:{
        
        yStringLiteral * _literal = (yStringLiteral *)stmt;
        printf((_level_prefix + "yStringLiteralClass, val = %s\n").c_str(), _literal->get_val().c_str());
        break;
    }
    case yStmt::CXXBoolLiteralExprClass:{
        
        yCXXBoolLiteralExpr * _literal = (yCXXBoolLiteralExpr *)stmt;
        printf((_level_prefix + "yCXXBoolLiteralExprClass, val = %s\n").c_str(), _literal->get_val()?"true":"false");
        break;
    }
    case yStmt::UnaryOperatorClass:{
        
        yUnaryOperator * _literal = (yUnaryOperator *)stmt;
        printf((_level_prefix + "yUnaryOperatorClass, op = %s\n").c_str(), _literal->get_op_kind() == 0?"+":"-");
        PrintStmt(_literal->get_val(), level + 1);
        break;
    }
    default:
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "Unkown StmtClass").c_str());
        break;
    }
}

static void PrintDecl(yDecl* decl, int level)
{

    std::string _level_prefix = "";
    get_level_prefix(_level_prefix, level);

    if(0 == level){

        printf("\n\n");
    }

    switch (decl->get_decl_kind())
    {
    case yDecl::Kind::ObjectDecl:{

        yObjectDecl * _obj_decl = (yObjectDecl *)decl;
        yCompoundStmt * _compound_stmt = (yCompoundStmt *)_obj_decl->get_body();

        printf((_level_prefix + "Object Decl, name = %s \n").c_str(), _obj_decl->getName().c_str());
        PrintStmt(_compound_stmt, level + 1);

        for (auto _stmt:_compound_stmt->get_stmt_vec()){

            PrintStmt(_stmt, level + 1 + 1);
        }        
        break;
    }
    case yDecl::Kind::VarDecl:{

        yVarDecl * _var_decl = (yVarDecl*) decl;
        printf((_level_prefix + "Var Decl, name = %s \n").c_str(), _var_decl->getName().c_str());
        PrintStmt(_var_decl->get_init().stmt, level +1);
        break;
    }
    default:
        //-Wformat-security, format not a string literal and no format arguments 
        printf("%s\n", (_level_prefix + "UNKOWN decl kind\n").c_str());
        break;
    }


    if(0 == level){

        printf("\n\n");
    }
}

#endif //YLIB_CODECOVERAGE_SKIP_CODE

/// HandleTopLevelDecl - Handle the specified top-level declaration.  This is
/// called by the parser to process every top-level Decl*.
///
/// \returns true to continue parsing, or false to abort parsing.
bool yConfigASTReader::HandleTopLevelDecl(yDeclGroup &decl_group) 
{

    // if (0 == decl_group.get_decl_vec().size()){
    //     yLib::yLog::W("Empty Decl, Maybe a comment\n");
    //     return true;
    // }

    for (auto _decl:decl_group.get_decl_vec()){

        // PrintDecl(_decl);
        this->decl_group->add_decl(_decl);
    }
    return true;
}     

void yConfigASTReader::SetDeclGroup(std::unique_ptr<yDeclGroup> && decl_group)
{
    this->decl_group = std::move(decl_group);
}

yDeclGroup & yConfigASTReader::GetDeclGroup(void)
{
    return *decl_group.get();
}

static yDecl* CheckDecl(const std::string & path, yDecl * decl)
{
    yDecl * _found = nullptr;
    switch (decl->get_decl_kind())
    {
    case yDecl::Kind::ObjectDecl:{
        /* code */
        yObjectDecl * _var_decl = (yObjectDecl *)decl;
        if (path == _var_decl->getName()){

            _found = decl;
            return _found;
        }
        break;
    }
    case yDecl::Kind::VarDecl:{
        /* code */
        yVarDecl * _var_decl = (yVarDecl *)decl;
        if (path == _var_decl->getName()){

            _found = decl;
            return _found;
        }
        break;
    }
    default:
        break;
    }

    return _found;
}

yDecl* yConfigASTReader::GetDecl(const std::string & path, yDecl * parent)
{
    yDecl * _found = nullptr;
    if (parent == nullptr){//get root decl, only yObjectDecl or yVarDecl

        for(yDecl * _decl:decl_group->get_decl_vec()){
            
            _found = CheckDecl(path, _decl);

            if (nullptr != _found)
                return _found;
        }
    }
    else{

        //parent must be yObjectDecl
        /* code */
        yObjectDecl * _obj_decl = (yObjectDecl *)parent;

        yCompoundStmt * _compound_stmt = (yCompoundStmt *)_obj_decl->get_body();

        for (auto _stmt:_compound_stmt->get_stmt_vec()){

            yDeclStmt * _decl_stmt = (yDeclStmt*)_stmt;
            _found = CheckDecl(path, _decl_stmt->get_decl());
            if (nullptr != _found)
                return _found;
        }
    }

    return _found;
}


//
yConfigASTWriter::yConfigASTWriter()
{}

/// Initialize - This is called to initialize the consumer, providing the
/// ASTContext.
void yConfigASTWriter::Initialize(yASTContext &context) 
{

}

/// HandleTranslationUnit - This method is called when the ASTs for entire
/// translation unit have been parsed.  
void yConfigASTWriter::HandleTranslationUnit(yASTContext &context)
{
    TranslationUnitDecl* _tudecl = context.getTranslationUnitDecl();
    TraverseDecl(_tudecl);
}

bool yConfigASTWriter::TraverseDecl(yDecl *decl)
{
    return base_visitor::TraverseDecl(decl);
}



//
ConfigASTPrinter::ConfigASTPrinter()
{}

/// Initialize - This is called to initialize the consumer, providing the
/// ASTContext.
void ConfigASTPrinter::Initialize(yASTContext &context) 
{

}

/// HandleTranslationUnit - This method is called when the ASTs for entire
/// translation unit have been parsed.  
void ConfigASTPrinter::HandleTranslationUnit(yASTContext &context)
{
    TranslationUnitDecl* _tudecl = context.getTranslationUnitDecl();
    TraverseDecl(_tudecl);
}

bool ConfigASTPrinter::TraverseDecl(yDecl *decl)
{
    return base_visitor::TraverseDecl(decl);
}