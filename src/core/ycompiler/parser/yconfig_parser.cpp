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
 * @Date: 2021-11-20 17:37:58
 * @LastEditTime: 2021-11-20 17:37:58
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\parse\yconfig_parser.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/frontend/ycompiler_instance.hpp"
#include "core/ycompiler/ast/ydeclaration_name.hpp"

#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/parser/yraii_objects_for_parser.hpp"

#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ycompiler/sema/ysema.hpp"

#include "core/ycompiler/ast/ydecl_group.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yConfigParser::yConfigParser(yCompilerInstance & ci)
:lexer(ci.GetLexer()), 
sema(ci.GetSema()), 
// preprocessor(sema.GetPreprocessor()), //the initial order is not ci,lexer,sema,preprocessor,diag_engine
diag_engine(ci.GetDiagnosticsEngine()),
ci(ci){}

yConfigParser::~yConfigParser(){}


yDiagnosticBuilder yConfigParser::Diag(ySourceLocation loc, unsigned diag_id) {
  return diag_engine.Report(loc, diag_id);
}

yDiagnosticBuilder yConfigParser::Diag(const yToken &cur_token, unsigned diag_id) {
  return Diag(cur_token.getLocation(), diag_id);
}


void yConfigParser::Initialize(void){
    
}

// In yConfigLanguage.md 2.7
// translation-unit:
//     external-declaration
//     translation-unit external-declaration
//ret: true is_eof, false is_not_eof
bool yConfigParser::ParseFirstTopLevelDecl(yDeclGroup &result){

    sema.ActOnStartOfTranslationUnit();

    //we prepare token
    ci.GetLexer().NextToken(cur_token);
    
    bool _is_no_top_level_decls = ParseTopLevelDecl(result, true);
    if (_is_no_top_level_decls){

        Diag(cur_token, diag::yconfigparser_warn_empty_source);
        // Diag(diag::)
    }

    if (ci.GetLexer().IsEof()){

        return true;
    }

    return false;
}

// In yConfigLanguage.md 2.7
// translation-unit:
//     external-declaration
//     translation-unit external-declaration

// In yConfigLanguage.md 2.7
// external-declaration:
//      object-definition

//ret: true is_eof, false is_not_eof
bool yConfigParser::ParseTopLevelDecl(yDeclGroup &result, bool is_first_decl){

    if (tok::eof == cur_token.kind){

        //we get last top-level-decl before this parse
        return true;
    }

    if (tok::comment == cur_token.kind){

        //we get top-level-comment, skip
        //we eat 'comment'
        ConsumeToken();
        return false;
    }

    result = ParseExternalDeclaration();
    return false;
}

// In yConfigLanguage.md 2.7
// external-declaration:
//      object-definition
yDeclGroup & yConfigParser::ParseExternalDeclaration(void)
{
    return ParseDeclarationOrObjectDefinition();
}

// In yConfigLanguage.md 2.7
// external-declaration:
//      object-definition
yDeclGroup &yConfigParser::ParseDeclarationOrObjectDefinition(void)
{
    
    return ParseDeclOrObjectDefInternal();
}

// In yConfigLanguage.md 2.7.1
// object-definition:
//     declarator : compound-statement
yDeclGroup &yConfigParser::ParseDeclOrObjectDefInternal(void)
{
    // Parse the common declaration-specifiers piece.
    
    // In yconfig, we skip Declaration Specifiers
    // ParseDeclarationSpecifiers()


    return ParseDeclGroup();
}


// In yConfigLanguage.md 2.5.2
// declarator:
//     pointeropt direct-declarator
// direct-declarator:
//     identifier
void yConfigParser::ParseDirectDeclarator(yDeclarator &declarator)
{
    if (cur_token.kind == tok::identifier){

        declarator.SetIdentifier(cur_token.GetIdentifierInfo(), cur_token.getLocation());
        declarator.SetRange(cur_token.getLocation(), cur_token.getLocation());

        yDeclarationName _name(cur_token.GetIdentifierInfo());
        yDeclarationNameInfo _name_info;
        _name_info.set_name(_name);
        _name_info.set_nameloc(cur_token.getLocation());

        ConsumeToken();//eat 'indentifier'

        if (cur_token.kind == tok::colon){//check ':'

            declarator.SetAsObjectDefinitionDeclarator();
        }

        declarator.set_is_valid(true);
    }
    else{//diag, the declarator must start of identifier

        declarator.set_is_valid(false);
        Diag(diag::yconfigparser_error_expect_identifier);
        
    }
}

// In yConfigLanguage.md 2.5.2
// declarator:
//     pointeropt direct-declarator
// direct-declarator:
//     identifier
void yConfigParser::ParseDeclaratorInternal(yDeclarator &declarator,
                               DirectDeclParseFunction DirectDeclParser)
{

    //parse direct-declarator
    (this->*DirectDeclParser)(declarator);
}

/// ParseDeclarator - Parse and verify a newly-initialized declarator.
///
void yConfigParser::ParseDeclarator(yDeclarator &declarator)
{
  /// This implements the 'declarator' production in the C grammar, then checks
  /// for well-formedness and issues diagnostics.
  ParseDeclaratorInternal(declarator, &yConfigParser::ParseDirectDeclarator);
}

/// ParseDeclGroup - Having concluded that this is either a function
/// definition or a group of object declarations, actually parse the
/// result.

// In yConfigLanguage.md 2.7.1
// object-definition:
//     declarator : compound-statement
yDeclGroup & yConfigParser::ParseDeclGroup(void)
{
    //parse first declarator
    yParsingDeclarator _declarator(*this, yDeclaratorContext::File);
    ParseDeclarator(_declarator); //eat 'identifier'

    if (!_declarator.is_valid()){

        return *new yDeclGroup();
    }

    yDecl* _decl = nullptr;
    if (_declarator.IsObjectDefinition()){//parse object defination

        //eat ':{...};'
        _decl = ParseObjectDefinition(_declarator);
    }
    else{//parse declaration, In yConfig, We only have Ojbect-Defination in top-level-decl.

        //eat '= ..xxx;'
        _decl = ParseDeclarationAfterDeclaratorAndAttributes(_declarator);
    }

    if (nullptr == _decl)
        set_paser_error(true);
        
    return sema.FinalizeDeclaratorGroup(_decl);
}


// In yConfigLanguage.md 2.7.1
// object-definition:
//     declarator : compound-statement
yDecl* yConfigParser::ParseObjectDefinition(yDeclarator &D)
{
    yDecl *_func_decl = sema.ActOnStartOfFunctionDef(D);

    yParseScope _scope;
    
    ConsumeToken();//eat ':'

    return ParseFunctionStatementBody(_func_decl, _scope);;
}


// In yConfigLanguage.md 2.5
// declaration:
//      declaration-specifiers init-declarator-listopt ;
// declaration-specifiers:
//      namespace-specifier
// init-declarator-list:
//      init-declarator
//      init-declarator-list , init-declarator
// init-declarator:
//      declarator
//      declarator = initializer

yDecl*  yConfigParser::ParseDeclarationAfterDeclaratorAndAttributes(yDeclarator &D)
{
    enum class InitKind { Uninitialized, Equal, CXXDirect, CXXBraced };
    InitKind TheInitKind;

    if (cur_token.kind == tok::equal){

        TheInitKind = InitKind::Equal;
    }
    else{

        TheInitKind = InitKind::Uninitialized;
    }

    // Inform Sema that we just parsed this declarator.
    // we get VarDecl
    yDecl * _this_decl = sema.ActOnDeclarator(D);

    // Parse declarator '=' initializer.
    if (TheInitKind == InitKind::Equal){

        ConsumeToken();//eat '='

        // we eat the 'initializer'
        yExpr *_expr = ParseInitializer();

        ConsumeToken();//eat ';'

        sema.AddInitializerToDecl(_this_decl, _expr, true);
    }
    else{

        //diag, expect a initializer
    }
    
    sema.FinalizeDeclaration(_this_decl);

    return _this_decl;
}


