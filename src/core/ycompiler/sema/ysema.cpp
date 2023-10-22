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
 * @Date: 2021-11-27 14:22:41
 * @LastEditTime: 2021-11-27 14:22:42
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\sema\ysema.cpp
 */

#include "core/ycompiler/sema/ysema.hpp"
#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"
#include "core/ycompiler/frontend/ycompiler_instance.hpp"
#include "core/ycompiler/ast/ydeclaration_name.hpp"

#include "core/ycompiler/ast/ydecl_spec.hpp"
#include "core/ycompiler/ast/ydecl.hpp"
#include "core/ycompiler/ast/ydecl_base.hpp"


#include "core/ylog/ylog.hpp"



using namespace yLib::ycompiler;
using namespace yLib;
ySema::ySema(yCompilerInstance & ci)
:ci(ci)
{
    preprocessor = std::unique_ptr<yPreprocessor>(new yPreprocessor());
    ast_ctx = std::unique_ptr<yASTContext>(new yASTContext());

    if (ActionKind::PARSE_YCONFIG_AST == ci.GetCompilerInvocationHelper().getFrontendOpts().act_type ){

        ast_consumer = std::unique_ptr<yASTConsumer>(new yConfigASTReader());
    }
    else
        ast_consumer = std::unique_ptr<yASTConsumer>(new yASTConsumer());
}

yPreprocessor &ySema::GetPreprocessor()
{return *(preprocessor.get());}

yASTContext &ySema::GetASTContext()
{return *(ast_ctx.get());}

yASTConsumer &ySema::GetASTConsumer()
{return *(ast_consumer.get());}

/// This is called before the very first declaration in the translation unit
/// is parsed. Note that the ASTContext may have already injected some
/// declarations.
void ySema::ActOnStartOfTranslationUnit(){}

/// ActOnEndOfTranslationUnit - This is called at the very end of the
/// translation unit when EOF is reached and all but the top-level scope is
/// popped.
void ySema::ActOnEndOfTranslationUnit(){}


/// Called before parsing a function declarator belonging to a function
/// declaration.
void ySema::ActOnStartFunctionDeclarationDeclarator(yDeclarator &D)
{

}

/// Called after parsing a function declarator belonging to a function
/// declaration.
void ySema::ActOnFinishFunctionDeclarationDeclarator(yDeclarator &D)
{

}


yNamedDecl *ySema::ActOnVariableDeclarator(yDeclarator &D)
{
    yDeclarationNameInfo _name_info = GetNameForDeclarator(D);
    // yDeclarationName _name = _name_info.get_name();

    // yIdentifierInfo *_indentifier_info = _name.GetAsIdentifierInfo();

    ySourceLocation _start_loc = _name_info.get_nameloc();
    yVarDecl *NewVD = yVarDecl::Create(D, _start_loc);


    return NewVD;
}
/// Retrieves the declaration name from a parsed unqualified-id.
yDeclarationNameInfo
ySema::GetNameFromUnqualifiedId(yUnqualifiedId &Name) {

    yDeclarationNameInfo _name_info;

    _name_info.set_nameloc(Name.GetStartLocation());

    if (Name.Kind == yUnqualifiedIdKind::IK_Identifier){
        
        yDeclarationName _name(Name.GetIdentifierInfo());
        _name_info.set_name(_name);
    }

    return _name_info;
}
/// GetNameForDeclarator - Determine the full declaration name for the
/// given Declarator.
yDeclarationNameInfo ySema::GetNameForDeclarator(yDeclarator &D) {
  return GetNameFromUnqualifiedId(D.GetIdentifier());
}

yNamedDecl *ySema::HandleDeclarator(yDeclarator &D)
{
    // yDeclarationNameInfo _name_info = GetNameForDeclarator(D);
    // yDeclarationName _name = _name_info.get_name();
    
    yNamedDecl * _name_decl = ActOnVariableDeclarator(D);

    return _name_decl;
}

yDecl *ySema::ActOnDeclarator(yDeclarator &D)
{
    yDecl *Dcl = HandleDeclarator(D);

    return Dcl;
}


yDeclGroup &ySema::FinalizeDeclaratorGroup(yDecl *decl)
{
    std::vector<yDecl*> _decl_vec;

    if (nullptr != decl)
        _decl_vec.push_back(decl);
    
    yASTContext _ctx;

    return *yDeclGroup::Create(_ctx, std::move(_decl_vec));
}

void ySema::AddInitializerToDecl(yDecl *decl, yExpr *init, bool DirectInit)
{
    yVarDecl * _decl = (yVarDecl*)decl;

    _decl->set_init(init);
}

/// FinalizeDeclaration - called by ParseDeclarationAfterDeclarator to perform
/// any semantic actions necessary after any initializer has been attached.
void ySema::FinalizeDeclaration(yDecl *D)
{
    
}


yExpr * ySema::ActOnNumericConstant(yToken & tok)
{

    yExpr * _ret = nullptr;

    yNumericLiteralParser _numeric_literal(tok);

    if (_numeric_literal.isIntegerLiteral()){

        int64_t _int64_val = 0;

        _numeric_literal.GetIntegerValue(_int64_val);

        _ret = yIntegerLiteral::Create(_int64_val, tok.getLocation());
    }

    if (_numeric_literal.isFloatingLiteral()){

        _ret = BuildFloatingLiteral(*this, _numeric_literal, tok.getLocation());
    }
    else{

        //unkown NumericLiteral
    }

    return _ret;
}

yExpr *ySema::BuildFloatingLiteral(ySema &S, yNumericLiteralParser &Literal, ySourceLocation loc)
{
    double _val = 0.f;

    Literal.GetFloatValue(_val);

    return yFloatingLiteral::Create(_val, loc);
}


yStmt* ySema::ActOnCompoundStmt(std::vector<yStmt*> &&stmt_vec)
{
    return yCompoundStmt::Create(std::move(stmt_vec));
}

yDecl *ySema::ActOnFinishObjectBody(yDecl *obj_decl, yStmt *obj_body)
{
    yObjectDecl * _obj_decl = (yObjectDecl *)obj_decl;

    _obj_decl->set_body(obj_body);

    return _obj_decl;
}

yDecl *ySema::ActOnStartOfFunctionDef(yDeclarator &D)
{
    
    return (yDecl *)yObjectDecl::Create(&D.GetIdentifier());
}

yExpr *ySema::ActOnStringLiteral(std::vector<yToken> &str_tok_vec)
{
    ySourceLocation _loc = str_tok_vec[0].getLocation();

    std::string _str = "";
    for (auto _tok:str_tok_vec){

        _str += std::string((char *)_tok.token_data + 1, _tok.token_data_len - 2);
    }

    return yStringLiteral::Create(_str, _loc);
}


yExpr * ySema::ActOnCXXBoolLiteral(yToken & BoolToks)
{
    bool _val = BoolToks.kind == tok::kw_true?true:false;

    return yCXXBoolLiteralExpr::Create(_val, BoolToks.getLocation());
}