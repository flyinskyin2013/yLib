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

#include "core/ycompiler/parser/yconfig_parser.hpp"
#include "core/ycompiler/parser/yraii_objects_for_parser.hpp"

#include "core/ycompiler/basic/ydiagnostics.hpp"

#include "core/ycompiler/sema/ysema.hpp"

#include "core/ycompiler/ast/ydecl_group.hpp"

#include "core/ylog.hpp"

using namespace yLib::ycompiler;
using namespace yLib;

yConfigParser::yConfigParser(yCompilerInstance & ci)
:ci(ci), 
lexer(ci.GetLexer()), 
sema(ci.GetSema()), 
// preprocessor(sema.GetPreprocessor()), //the initial order is not ci,lexer,sema,preprocessor,diag_engine
diag_engine(ci.GetDiagnosticsEngine()){}

yConfigParser::~yConfigParser(){}

bool yConfigParser::ParseAST(void){

    return ParseDecl();
}

bool yConfigParser::ParseDecl(void){

    return ParserTranslationUnit();
}

void * yConfigParser::GetASTData(){

    return &root_object;
}
//yConfigParser Notes
// G=(Vt, Vn, P, S)
/* ref: n1256.pdf 


Syntax-in-c99:
identifier:
    ref: n1256.pdf 6.4.2 Identifiers

floating-constant:
    ref: 6.4.4.1 Integer constants

integer-constant:
    ref: 6.4.4.2 Floating constants

string-literal:
    ref: n1256.pdf 6.4.5 String literals


yConfig Language Define
EBNF:
Syntax1:
translation-unit:
    object-list


Syntax2:
object-list:
    object
    object-list ';'
    object-list ';' object

Syntax3:
object:    
    identifier ':' '{' declaration-list '}'

Syntax4:
declaration-list:
    declaration-item
    declaration-item ';'
    object
    object ';'
    declaration-list ';' declaration-item
    declaration-list ';' object

Syntax5:
declaration-item:
    identifier '=' value-item 

Syntax6:
value-item:
    floating-constant
    integer-constant
    string-literal
    bool-constant

Syntax7:
bool-constant:
    'true'
    'false'
*/
bool yConfigParser::ParserTranslationUnit(void){

    return ParserObjectList();
}

bool yConfigParser::ParserObjectList(void){

    lexer.NextToken(cur_token);//get first token

    if (cur_token.kind != tok::identifier && cur_token.kind != tok::comment){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_obj_or_comment);
        return false;
    }

    while(cur_token.kind == tok::identifier || cur_token.kind == tok::comment){

        if (cur_token.kind == tok::comment){//comment token

            yConfigDeclComment* _comment = new (std::nothrow) yConfigDeclComment();
            _comment->comment = std::string((const char *)cur_token.token_data, cur_token.token_data_len);
            std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _comment);

            root_object.comment_vec.push_back((yConfigDecl *) _comment);

            root_object.decl_order_vec.push_back(std::move(_decl));//

            lexer.NextToken(cur_token);//consumer token
            continue;//skip
        }

        //create object decl
        yConfigDeclObject* _object = new (std::nothrow) yConfigDeclObject();
 
        std::string _obj_name((const char *)cur_token.token_data, cur_token.token_data_len);
        std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _object); 
        root_object.decl_order_vec.push_back(std::move(_decl));  
        
        if (root_object.decl_map.count(_obj_name) > 0){//exsit object name error

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_objname_exsit);
            return false;
        }
        else{

            root_object.decl_map.insert(std::make_pair(_obj_name, _object));//insert a object to root object
            if (!ParserObject(*_object, _obj_name)){//object token

                LOGE("yConfigParser")<<"parse a object failed.";
                return false;
            }
        }
    }

    if (cur_token.kind != tok::eof){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_obj_or_comment);
        return false;
    }

    return true;
}

bool yConfigParser::ParserDeclareItem(yConfigDeclItem & item, std::string &item_name){

    if (cur_token.kind != tok::identifier){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_identifier);
        return false;
    }
    std::string _identifier_str((const char *)cur_token.token_data, cur_token.token_data_len);

    lexer.NextToken(cur_token);//next tok, skip identifier
    if (cur_token.kind != tok::equal){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_equal);
        return false;
    }            

    /*
    Syntax6:
    value-item:
        floating-constant
        integer-constant
        string-literal
        bool-constant
    */
    lexer.NextToken(cur_token);//next tok, skip equal
    if (cur_token.kind == tok::numeric_constant || cur_token.kind == tok::plus || cur_token.kind == tok::minus){

        uint64_t _num_val;
        int8_t _call_ret = sema.ParseNumberConstantExpression(_num_val);
        if (-1 == _call_ret){

            return false;
        }
        else if (1 == _call_ret){//int

            int64_t _int64_t_val = 0;
            ::memcpy((char *)&_int64_t_val, (char *)&_num_val, sizeof(int64_t));
            item.SetInt64t(_int64_t_val);
        }
        else{//float

            double _double_val = 0;
            ::memcpy((char *)&_double_val, (char *)&_num_val, sizeof(double));
            item.SetDouble(_double_val);
        }

        lexer.NextToken(cur_token);//skip numeric_constant
    }
    else if (cur_token.kind == tok::string_literal){

        std::string _str(((char *)cur_token.token_data) + 1, cur_token.token_data_len - 2);//get rid of two '"'
        item.SetStringLiteral(_str);
        lexer.NextToken(cur_token);//skip string_literal
    }
    else if (cur_token.kind == tok::identifier){//bool-constant only true or false

        std::string _str((char *)cur_token.token_data, cur_token.token_data_len);
        if (_str == "true" || _str == "false"){
            
            if (_str == "true")
                item.SetBool(true);
            else
                item.SetBool(false);
        }
        else{

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_true_or_false);
            return false;
        }
        
        lexer.NextToken(cur_token);//skip bool val
    }
    else{

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_valid_item_val);
        return false;
    }

    if (cur_token.kind == tok::semi){

        lexer.NextToken(cur_token);//next token, skip semi
    }
    else{

        lexer.TryNextToken(next_token);//try get next token
        if (next_token.kind == tok::identifier){//two object must split by ;

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_semi);
            return false;
        }
    }

    return true;
}
bool yConfigParser::ParserDeclareList(yConfigDeclObject & cur_obj, std::string &obj_name){
    
    while(cur_token.kind == tok::identifier || cur_token.kind == tok::comment){

        if (cur_token.kind == tok::comment){//comment token

            yConfigDeclComment* _comment = new (std::nothrow) yConfigDeclComment();
            _comment->comment = std::string((const char *)cur_token.token_data, cur_token.token_data_len);
            std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _comment);

            cur_obj.comment_vec.push_back((yConfigDecl *) _comment);

            cur_obj.decl_order_vec.push_back(std::move(_decl));//

            lexer.NextToken(cur_token);//consumer token
            continue;//skip
        }

        if (cur_token.kind != tok::identifier){

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_identifier);
            return false;
        }

        std::string _identifier_name((const char *)cur_token.token_data, cur_token.token_data_len);

        lexer.TryNextToken(next_token);//try next token
        if (next_token.kind == tok::colon || next_token.kind == tok::equal){

            if (next_token.kind == tok::colon){//object

                yConfigDeclObject* _object = new (std::nothrow) yConfigDeclObject();
                 
                std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _object);    
                cur_obj.decl_order_vec.push_back(std::move(_decl));    

                if (cur_obj.decl_map.count(_identifier_name) > 0){//exsit object name error

                    ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_exsit_identifier);
                    return false;
                }
                else{           

                    cur_obj.decl_map.insert(std::make_pair(_identifier_name, _object));//insert a object 
                    if (!ParserObject(*_object, _identifier_name)){

                        LOGE("yConfigParser")<<"parse a object failed.";
                        return false;
                    }
                }
            }
            else{//declare item

                yConfigDeclItem* _item = new (std::nothrow) yConfigDeclItem();
                
                std::unique_ptr<yConfigDecl> _decl = std::unique_ptr<yConfigDecl>((yConfigDecl *) _item);    

                cur_obj.decl_order_vec.push_back(std::move(_decl));  
                if (cur_obj.decl_map.count(_identifier_name) > 0){//exsit object name error

                    ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_exsit_identifier);
                    return false;
                }
                else{   
                    
                    cur_obj.decl_map.insert(std::make_pair(_identifier_name, _item));//insert a item 
                    if (!ParserDeclareItem(*_item, _identifier_name)){

                        LOGE("yConfigParser")<<"parse a item failed.";
                        return false;
                    }
                }
            }
        }
        else{

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_colon_or_equal);
            return false;                
        }
    }

    return true;
}

bool yConfigParser::ParserObject(yConfigDeclObject & cur_obj, std::string &obj_name){

    if (cur_token.kind != tok::identifier){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_obj_start_of_identifier);
        return false;
    }
    lexer.NextToken(cur_token);//next token, skip identifier
    if (cur_token.kind != tok::colon){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_obj_second_field);
        return false;
    }

    lexer.NextToken(cur_token);//next token, skip colon
    if (cur_token.kind != tok::l_brace){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_obj_third_field);
        return false;
    }
    
    lexer.NextToken(cur_token);//next token, object body, skip l_brace
    if (cur_token.kind != tok::r_brace)//not a empty body
        if (!ParserDeclareList(cur_obj, obj_name))
            return false;
    
    //we get new token in ParserObject or ParserDeclareList
    if (cur_token.kind != tok::r_brace){

        ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_obj_final_field);
        return false;
    }

    lexer.NextToken(cur_token);//next token, skip r_brace
    if (cur_token.kind == tok::semi){

        lexer.NextToken(cur_token);//next token, skip semi
    }
    else{

        lexer.TryNextToken(next_token);//try get next token
        if (next_token.kind == tok::identifier){//two object must split by ;

            ci.GetDiagnosticsEngine().DiagReport(cur_token, diag::yconfigparser_error_expect_semi);
            return false;
        }
    }

    return true;
}




yDiagnosticBuilder yConfigParser::Diag(ySourceLocation loc, unsigned diag_id) {
  return diag_engine.Report(loc, diag_id);
}

yDiagnosticBuilder yConfigParser::Diag(const yToken &cur_token, unsigned diag_id) {
  return Diag(cur_token.getLocation(), diag_id);
}


void yConfigParser::Initialize(void){
    
}
/// Parse the first top-level declaration in a translation unit.
///
///   translation-unit:
/// [C]     external-declaration
/// [C]     translation-unit external-declaration
/// [C++]   top-level-declaration-seq[opt]
/// [C++20] global-module-fragment[opt] module-declaration
///                 top-level-declaration-seq[opt] private-module-fragment[opt]
///
/// Note that in C, it is an error if there is no first declaration.

// In yConfigLanguage.md 2.7
// translation-unit:
//     external-declaration
//     translation-unit external-declaration


bool yConfigParser::ParseFirstTopLevelDecl(yDeclGroup &result){

    // C11 6.9p1 says translation units must have at least one top-level
    // declaration. C++ doesn't have this restriction. We also don't want to
    // complain if we have a precompiled header, although technically if the PCH
    // is empty we should still emit the (pedantic) diagnostic.
    // If the main file is a header, we're only pretending it's a TU; don't warn.
    bool _is_no_top_level_decls = ParseTopLevelDecl(result, true);
    if (_is_no_top_level_decls){

        // Diag(diag::)
        
    }
    return true;
}

/// ParseTopLevelDecl - Parse one top-level declaration, return whatever the
/// action tells us to.  This returns true if the EOF was encountered.
///
///   top-level-declaration:
///           declaration
/// [C++20]   module-import-declaration

// In yConfigLanguage.md 2.7
// external-declaration:
//      declaration  
//      object-definition
bool yConfigParser::ParseTopLevelDecl(yDeclGroup &result, bool is_first_decl){

    result = ParseExternalDeclaration();
    return false;
}

/// ParseExternalDeclaration:
///
///       external-declaration: [C99 6.9], declaration: [C++ dcl.dcl]
///         function-definition
///         declaration
/// [GNU]   asm-definition
/// [GNU]   __extension__ external-declaration
/// [OBJC]  objc-class-definition
/// [OBJC]  objc-class-declaration
/// [OBJC]  objc-alias-declaration
/// [OBJC]  objc-protocol-definition
/// [OBJC]  objc-method-definition
/// [OBJC]  @end
/// [C++]   linkage-specification
/// [GNU] asm-definition:
///         simple-asm-expr ';'
/// [C++11] empty-declaration
/// [C++11] attribute-declaration
///
/// [C++11] empty-declaration:
///           ';'
///
/// [C++0x/GNU] 'extern' 'template' declaration
///
/// [Modules-TS] module-import-declaration
///

// In yConfigLanguage.md 2.7
// external-declaration:
//      declaration  
//      object-definition
yDeclGroup yConfigParser::ParseExternalDeclaration(void)
{
    return ParseDeclarationOrObjectDefinition();
}


yDeclGroup yConfigParser::ParseDeclarationOrObjectDefinition(void)
{
    //prepare first token, and second token
    lexer.NextToken(cur_token);
    lexer.TryNextToken(next_token);

    return ParseDeclOrObjectDefInternal();
}

/// Parse either a function-definition or a declaration.  We can't tell which
/// we have until we read up to the compound-statement in function-definition.
/// TemplateParams, if non-NULL, provides the template parameters when we're
/// parsing a C++ template-declaration.
///
///       function-definition: [C99 6.9.1]
///         decl-specs      declarator declaration-list[opt] compound-statement
/// [C90] function-definition: [C99 6.7.1] - implicit int result
/// [C90]   decl-specs[opt] declarator declaration-list[opt] compound-statement
///
///       declaration: [C99 6.7]
///         declaration-specifiers init-declarator-list[opt] ';'
/// [!C99]  init-declarator-list ';'                   [TODO: warn in c99 mode]
/// [OMP]   threadprivate-directive
/// [OMP]   allocate-directive                         [TODO]
///

// In yConfigLanguage.md 2.7.1
// object-definition:
//     declarator : compound-statement
yDeclGroup yConfigParser::ParseDeclOrObjectDefInternal(void)
{
    return ParseDeclGroup();
}



/// ParseDirectDeclarator
///       direct-declarator: [C99 6.7.5]
/// [C99]   identifier
///         '(' declarator ')'
/// [GNU]   '(' attributes declarator ')'
/// [C90]   direct-declarator '[' constant-expression[opt] ']'
/// [C99]   direct-declarator '[' type-qual-list[opt] assignment-expr[opt] ']'
/// [C99]   direct-declarator '[' 'static' type-qual-list[opt] assign-expr ']'
/// [C99]   direct-declarator '[' type-qual-list 'static' assignment-expr ']'
/// [C99]   direct-declarator '[' type-qual-list[opt] '*' ']'
/// [C++11] direct-declarator '[' constant-expression[opt] ']'
///                    attribute-specifier-seq[opt]
///         direct-declarator '(' parameter-type-list ')'
///         direct-declarator '(' identifier-list[opt] ')'
/// [GNU]   direct-declarator '(' parameter-forward-declarations
///                    parameter-type-list[opt] ')'
/// [C++]   direct-declarator '(' parameter-declaration-clause ')'
///                    cv-qualifier-seq[opt] exception-specification[opt]
/// [C++11] direct-declarator '(' parameter-declaration-clause ')'
///                    attribute-specifier-seq[opt] cv-qualifier-seq[opt]
///                    ref-qualifier[opt] exception-specification[opt]
/// [C++]   declarator-id
/// [C++11] declarator-id attribute-specifier-seq[opt]
///
///       declarator-id: [C++ 8]
///         '...'[opt] id-expression
///         '::'[opt] nested-name-specifier[opt] type-name
///
///       id-expression: [C++ 5.1]
///         unqualified-id
///         qualified-id
///
///       unqualified-id: [C++ 5.1]
///         identifier
///         operator-function-id
///         conversion-function-id
///          '~' class-name
///         template-id
///
/// C++17 adds the following, which we also handle here:
///
///       simple-declaration:
///         <decl-spec> '[' identifier-list ']' brace-or-equal-initializer ';'
///
/// Note, any additional constructs added here may need corresponding changes
/// in isConstructorDeclarator.

// In yConfigLanguage.md 2.5.2
// declarator:
//     pointeropt direct-declarator
// direct-declarator:
//     identifier
void yConfigParser::ParseDirectDeclarator(yDeclarator &D)
{
    if (cur_token.kind == tok::identifier){

        sema.ActOnStartFunctionDeclarationDeclarator(D);

        sema.ActOnFinishFunctionDeclarationDeclarator(D);
    }
}

/// ParseDeclaratorInternal - Parse a C or C++ declarator. The direct-declarator
/// is parsed by the function passed to it. Pass null, and the direct-declarator
/// isn't parsed at all, making this function effectively parse the C++
/// ptr-operator production.
///
/// If the grammar of this construct is extended, matching changes must also be
/// made to TryParseDeclarator and MightBeDeclarator, and possibly to
/// isConstructorDeclarator.
///
///       declarator: [C99 6.7.5] [C++ 8p4, dcl.decl]
/// [C]     pointer[opt] direct-declarator
/// [C++]   direct-declarator
/// [C++]   ptr-operator declarator
///
///       pointer: [C99 6.7.5]
///         '*' type-qualifier-list[opt]
///         '*' type-qualifier-list[opt] pointer
///
///       ptr-operator:
///         '*' cv-qualifier-seq[opt]
///         '&'
/// [C++0x] '&&'
/// [GNU]   '&' restrict[opt] attributes[opt]
/// [GNU?]  '&&' restrict[opt] attributes[opt]
///         '::'[opt] nested-name-specifier '*' cv-qualifier-seq[opt]

// In yConfigLanguage.md 2.5.2
// declarator:
//     pointeropt direct-declarator
// direct-declarator:
//     identifier
void yConfigParser::ParseDeclaratorInternal(yDeclarator &D,
                               DirectDeclParseFunction DirectDeclParser)
{

    //parse direct-declarator
    (this->*DirectDeclParser)(D);
}

/// ParseDeclarator - Parse and verify a newly-initialized declarator.
///
void yConfigParser::ParseDeclarator(yDeclarator &D)
{
  /// This implements the 'declarator' production in the C grammar, then checks
  /// for well-formedness and issues diagnostics.
  ParseDeclaratorInternal(D, &yConfigParser::ParseDirectDeclarator);
}

/// ParseDeclGroup - Having concluded that this is either a function
/// definition or a group of object declarations, actually parse the
/// result.
yDeclGroup yConfigParser::ParseDeclGroup(void)
{

    //parse first declarator
    yParsingDeclarator D(*this, yDeclaratorContext::File);
    ParseDeclarator(D);

    yDecl * _the_decl = ParseObjectDefinition(D);

    yDeclGroup * _the_decl_group = yDeclGroup::Create(ci.GetSema().GetASTContext(), &_the_decl, 1);

    return *_the_decl_group;
}

/// ParseFunctionDefinition - We parsed and verified that the specified
/// Declarator is well formed.  If this is a K&R-style function, read the
/// parameters declaration-list, then start the compound-statement.
///
///       function-definition: [C99 6.9.1]
///         decl-specs      declarator declaration-list[opt] compound-statement
/// [C90] function-definition: [C99 6.7.1] - implicit int result
/// [C90]   decl-specs[opt] declarator declaration-list[opt] compound-statement
/// [C++] function-definition: [C++ 8.4]
///         decl-specifier-seq[opt] declarator ctor-initializer[opt]
///         function-body
/// [C++] function-definition: [C++ 8.4]
///         decl-specifier-seq[opt] declarator function-try-block
///

// In yConfigLanguage.md 2.7.1
// object-definition:
//     declarator : compound-statement
yDecl* yConfigParser::ParseObjectDefinition(yDeclarator &D)
{
    ySourceLocation _decl_start_loc = cur_token.getLocation();
    ySourceLocation _decl_end_loc;

    yDeclGroup _decl_group = ParseDeclaration(D, _decl_end_loc);


    return nullptr;
}

yDeclGroup yConfigParser::ParseDeclaration(yDeclarator &D, ySourceLocation& decl_end_loc)
{

    return yDeclGroup();
}