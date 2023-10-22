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
 * @Date: 2021-11-20 15:22:33
 * @LastEditTime: 2021-11-20 15:23:41
 * @LastEditors: Sky
 * @Description: 
 * @FilePath: \yLib\src\core\ycompiler\lex\ylexer.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ycompiler/frontend/ycompiler_instance.hpp"

#include "core/ycompiler/lexer/ylexer.hpp"
#include "core/ycompiler/basic/ydiagnostics.hpp"
#include "core/ylog/ylog.hpp"


using namespace yLib::ycompiler;
using namespace yLib;

yLexer::yLexer(yCompilerInstance & ci)
:file_mgr(ci.GetFileManger()),ci(ci)
{
    yFileEntry *  _file_entry = ci.GetFileManger().GetFileEntry(ci.GetSourceManager().getMainFileID());
    yMemoryBuffer * _file_buffer = ci.GetFileManger().GetFileMemroyBuffer(_file_entry->get_file_path());

    buf_start = buf_cur_ptr = (char * )_file_buffer->get_buf_start();
    buf_end = (char * )_file_buffer->get_buf_end();
}
yLexer::~yLexer(){

}
bool yLexer::SetFileBuffer(char * buf_start, char * buf_end)
{
    buf_cur_ptr = buf_start = buf_start;
    buf_end = buf_end;
    return true;
}

bool yLexer::BackToPos(uint64_t pos)
{

    if (((uint64_t)buf_cur_ptr - (uint64_t)buf_start) < pos){

        yLog::E("yLexer", "BackToPos failed. the pos is invalid %lu", pos);
        return false;
    }

    buf_cur_ptr -= pos;
    return true;
}

bool yLexer::HasNextChar(const char * ptr){

    if (ptr == buf_end)
        return false;
    
    return true;
}

bool yLexer::HasNextChar(void){

    return HasNextChar(buf_cur_ptr);
}

bool yLexer::GetNextChar(char &c){
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    if (!HasNextChar())
        return false;

    c = *buf_cur_ptr;

    buf_cur_ptr++;

    return true;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
}

bool yLexer::TryNextToken(yToken & token){

    bool _ret0 = NextToken(token);//get next token

    //back to start of this token
    bool _ret1 = BackToPos(token.token_data_len);

    return _ret0&&_ret1;
}

void yLexer::UpdateToken(yToken &token, const char *tok_end, tok::yTokenKind kind){

    token.kind = kind;
    token.token_data_len = tok_end - buf_cur_ptr;
    token.file_id = ci.GetSourceManager().getMainFileID();
    token.offset = token.loc = buf_cur_ptr - buf_start;

    if (tok::identifier == kind){

        std::string _identifier = std::string(buf_cur_ptr, token.token_data_len);
        token.token_data = (void *)new yIdentifierInfo(_identifier, kind);
    }
    else{
        //the 'true' or 'false' will be recognized identifier firstly, we should check it
        if (tok::kw_true == kind || tok::kw_false == kind){

            delete (yIdentifierInfo *)token.token_data;
            token.token_data = 0;
        }
        
        token.token_data = (void *)buf_cur_ptr;
    }

    buf_cur_ptr = tok_end;//update buf_cur_ptr
}

bool yLexer::GetEndOfFileToken(yToken &token, const char * cur_ptr){

    UpdateToken(token, buf_end, tok::eof);
    return true;
}

bool yLexer::IsNumericChar(const char * cur_ptr){

    if ( *cur_ptr >= '0' && *cur_ptr <= '9') return true;

    return false;
}

bool yLexer::IsEof(void){

    return IsEof(buf_cur_ptr);
}

bool yLexer::IsEof(const char * ptr){

    if (ptr == (buf_end - 1))
        return true;
    
    return false;
}

bool yLexer::GetNumericConstant(yToken &token, const char * cur_ptr){

    if (*cur_ptr == 'x' || *cur_ptr == 'x')//hex
        cur_ptr++;

    while(IsNumericChar(cur_ptr)) //
        cur_ptr++;
    
    if (*cur_ptr == '.'){//float 
        
        cur_ptr++;
        while(IsNumericChar(cur_ptr)) //
            cur_ptr++;
    }

    // scientific notation, 1.234567e+1
    if ( *cur_ptr == 'e' || *cur_ptr == 'E'){ //skip e

        if (cur_ptr[1] == '+' || cur_ptr[1] == '-')//skip sign
            cur_ptr++;

        cur_ptr++;
        while(IsNumericChar(cur_ptr)) //
            cur_ptr++;
    }

    if (*cur_ptr == 'f' || *cur_ptr == 'F')//skip suffix
        cur_ptr++;

    UpdateToken(token, cur_ptr, tok::numeric_constant);
    return true;
}

bool yLexer::IsIdentifierBodyChar(const char * cur_ptr){

    if ( (*cur_ptr >= 'a' && *cur_ptr <= 'z') || \
        (*cur_ptr >= 'A' && *cur_ptr <= 'Z') || \
        (*cur_ptr >= '0' && *cur_ptr <= '9') || \
        *cur_ptr == '_'
    ) return true;

    return false;
}
bool yLexer::GetIdentifier(yToken &token, const char * cur_ptr){

    while(IsIdentifierBodyChar(cur_ptr))
        cur_ptr ++;
    
    //check we get true/false literal
    std::string _val((char *)buf_cur_ptr, cur_ptr - buf_cur_ptr);

    if (_val == "true" || _val == "false"){//kw

        if (_val == "true")
            UpdateToken(token, cur_ptr, tok::kw_true);
        else
            UpdateToken(token, cur_ptr, tok::kw_false);
    }
    else{//identifier

        UpdateToken(token, cur_ptr, tok::identifier);
    }
    return true;
}

#ifndef YLIB_CODECOVERAGE_SKIP_CODE
bool yLexer::GetCharConstant(yToken &token, const char * cur_ptr){

    if (cur_ptr[0] == '\\'){//

        cur_ptr++;//skip '\'
        cur_ptr++;//real char
        if (cur_ptr[0] != '\''){
            
            ySourceLocation _loc(file_id, 0);
            ci.GetDiagnosticsEngine().DiagReport(_loc, diag::lexer_error_charconstant);
            return false;
        }
    }
    else{

        cur_ptr++;
        if (cur_ptr[0] != '\''){
            
            ySourceLocation _loc(file_id, 0);
            ci.GetDiagnosticsEngine().DiagReport(_loc, diag::lexer_error_charconstant);
            return false;
        }            
    }

    UpdateToken(token, cur_ptr, tok::char_constant);  

    return true;
}
#endif //YLIB_CODECOVERAGE_SKIP_CODE

bool yLexer::GetStringLiteral(yToken &token, const char * cur_ptr){

    while(*cur_ptr != '"' && !IsEof(cur_ptr)) cur_ptr++;

    if (IsEof(cur_ptr)){//we get eof before second '"'

        ySourceLocation _loc(file_id, 0);
        ci.GetDiagnosticsEngine().DiagReport(_loc, diag::lexer_error_stringliteral);
        return false;
    }

    cur_ptr++;//move ptr to after second '"'
    UpdateToken(token, cur_ptr, tok::string_literal);  
    return true;
}

bool yLexer::NextToken(yToken & token){

    LEX_NEXT_TOKEN:
    
    if (!HasNextChar())//has no data to process
        return false;

    const char * _tmp_buf_cur_ptr = buf_cur_ptr;
    const char _tmp_char = *_tmp_buf_cur_ptr;
    _tmp_buf_cur_ptr++;

    token.clean();

    // when every case is completed, the  buf_cur_ptr must point to next pos by UpdateToken or hand
    switch (_tmp_char)
    {
    case 0:{//end of file

        if (_tmp_buf_cur_ptr == buf_end)//buf_end = 
            return GetEndOfFileToken(token, _tmp_buf_cur_ptr);
        /* code */
        break;
    }

    case 26:{// DOS & CP/M EOF: "^Z".

        token.kind = tok::unknown;
        break;
    }
    case '\r':{//CR (carriage return) 回车键
        
        // skip cr
        buf_cur_ptr = _tmp_buf_cur_ptr;
        goto LEX_NEXT_TOKEN;
    
        break;
    }
    case '\n':{// new line,//LF (NL line feed, new line) 换行键

        buf_cur_ptr = _tmp_buf_cur_ptr;
        goto LEX_NEXT_TOKEN;
        break;
    }
    case ' ':
    case '\t':
    case '\f'://换页(FF)，将当前位置移到下页开头, ascii = 0xC
    case '\v'://ascii = 0xB
    {

        buf_cur_ptr = _tmp_buf_cur_ptr;
        goto LEX_NEXT_TOKEN;
        break;
    }
    // C99 6.4.4.1: Integer Constants.
    // C99 6.4.4.2: Floating Constants.
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':{

        return GetNumericConstant(token, _tmp_buf_cur_ptr);
        break;
    }
    // case 'u':{// Identifier (uber) or C11/C++11 UTF-8 or UTF-16 string literal

    //     col_num ++;
    //     goto LEX_NEXT_TOKEN;
    //     break;
    // }
    // case 'U':{// Identifier (Uber) or C11/C++11 UTF-32 string literal

    //     col_num ++;
    //     goto LEX_NEXT_TOKEN;
    //     break;
    // }
    // case 'R':{// Identifier or C++0x raw string literal

    //     col_num ++;
    //     goto LEX_NEXT_TOKEN;
    //     break;
    // }
    // case 'L':{// Identifier (Loony) or wide literal (L'x' or L"xyz").

    //     col_num ++;
    //     goto LEX_NEXT_TOKEN;
    //     break;                
    // }
    // C99 6.4.2: Identifiers.
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L':   /*'L'*/case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R':   /*'R'*/case 'S': case 'T': case 'U':   /*'U'*/
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':   /*'u'*/
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case '_':{
        
        return GetIdentifier(token, _tmp_buf_cur_ptr);
        break;
    }
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case '$':{

        token.kind = tok::unknown;
        break;
    }
    // C99 6.4.4: Character Constants.
    case '\'':{

        return GetCharConstant(token, _tmp_buf_cur_ptr);
        break;
    }
#endif //YLIB_CODECOVERAGE_SKIP_CODE    
    // C99 6.4.5: String Literals.
    case '"':{

        return GetStringLiteral(token, _tmp_buf_cur_ptr);
        break;
    }
    // C99 6.4.6: Punctuators.
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case '?':
        token.kind = tok::question;
        break;
    case '[':
        token.kind = tok::l_square;
        break;
    case ']':
        token.kind = tok::r_square;
        break;
    case '(':
        token.kind = tok::l_paren;
        break;
    case ')':
        token.kind = tok::r_paren;
        break;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
    case '{':
        token.kind = tok::l_brace;
        break;
    case '}':
        token.kind = tok::r_brace;
        break;
    // case '.': this is processed in GetNumericConstant
    //     break;
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case '&':

        if (*_tmp_buf_cur_ptr == '&' || *_tmp_buf_cur_ptr == '='){

            if (*_tmp_buf_cur_ptr == '&') 
                token.kind = tok::ampamp;
            else if (*_tmp_buf_cur_ptr == '=') 
                token.kind = tok::ampequal;     

            _tmp_buf_cur_ptr++;//point to next               
        }
        else 
            token.kind = tok::amp;
        
        break;
    case '*':
        if (*_tmp_buf_cur_ptr == '=') {

            token.kind = tok::starequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::star;

        break;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
    case '+':

        if (*_tmp_buf_cur_ptr == '+' || *_tmp_buf_cur_ptr == '='){
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
            if (*_tmp_buf_cur_ptr == '+') 
                token.kind = tok::plusplus;
            else if (*_tmp_buf_cur_ptr == '=') 
                token.kind = tok::plusequal;
            
            _tmp_buf_cur_ptr++;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
        }
        else 
            token.kind = tok::plus;

        break;
    case '-':

        if (*_tmp_buf_cur_ptr == '-' || *_tmp_buf_cur_ptr == '>' || *_tmp_buf_cur_ptr == '='){
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
            if (*_tmp_buf_cur_ptr == '-')      // --
                token.kind = tok::minusminus;
            else if (*_tmp_buf_cur_ptr == '>')    // ->
                token.kind = tok::arrow;
            else if (*_tmp_buf_cur_ptr == '=')    // -=
                token.kind = tok::minusequal;
            _tmp_buf_cur_ptr++;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
        }
        else 
            token.kind = tok::minus;

        break;
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case '~':
        token.kind = tok::tilde;
        break;
    case '!':
        if (*_tmp_buf_cur_ptr == '=') {

            token.kind = tok::exclaimequal;
            _tmp_buf_cur_ptr ++;
        }
        else 
            token.kind = tok::exclaim;

        
        break;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
    case '/':
        // 6.4.9: Comments
        if (*_tmp_buf_cur_ptr == '/') {         // Line comment.

            while( (*_tmp_buf_cur_ptr != '\n' && *_tmp_buf_cur_ptr != '\r') && !IsEof(_tmp_buf_cur_ptr) )
                _tmp_buf_cur_ptr ++;
            
            if (IsEof(_tmp_buf_cur_ptr))//eof
                return false;

            _tmp_buf_cur_ptr--;
            char _pre_char = *_tmp_buf_cur_ptr;
            _tmp_buf_cur_ptr++;

            if (_pre_char == '\r' ){//\r \n

                _tmp_buf_cur_ptr ++; //skip \n
            }
            
            UpdateToken(token, _tmp_buf_cur_ptr, tok::comment);
            
            return true;
            break;
        }
        else if (*_tmp_buf_cur_ptr == '*') {  // /**/ comment.

            _tmp_buf_cur_ptr ++;//to next pos

            RE_CONTINUE_COMMENT:
            while(*_tmp_buf_cur_ptr != '*'  && !IsEof(_tmp_buf_cur_ptr) )
                _tmp_buf_cur_ptr++;

            if (IsEof(_tmp_buf_cur_ptr))//eof
                return false;

            _tmp_buf_cur_ptr++;//skip '*'
            if (*_tmp_buf_cur_ptr != '/'){
                _tmp_buf_cur_ptr++;
                goto RE_CONTINUE_COMMENT;
            }
            
            _tmp_buf_cur_ptr ++;//skip '/'
            UpdateToken(token, _tmp_buf_cur_ptr, tok::comment);

            return true; // There is a token to return.
        }
        else if (*_tmp_buf_cur_ptr == '=') {

            token.kind = tok::slashequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::slash;
        
        
        break;
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case '%':

        if (*_tmp_buf_cur_ptr == '=') {
            token.kind = tok::percentequal;
            _tmp_buf_cur_ptr++;
        }
        else
            token.kind = tok::percent;
        
        
        break;
    case '<':
        if (*_tmp_buf_cur_ptr == '=')  {
            token.kind = tok::lessequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::less;
        
        break;
    case '>':

        if (*_tmp_buf_cur_ptr == '=') {

            token.kind = tok::greaterequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::greater;

        break;
    case '^':

        if (*_tmp_buf_cur_ptr == '=') {
            
            token.kind = tok::caretequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::caret;
        
        
        break;
    case '|':
        
        if (*_tmp_buf_cur_ptr == '=' || *_tmp_buf_cur_ptr == '|'){

            if (*_tmp_buf_cur_ptr == '=') 
                token.kind = tok::pipeequal;
            else if (*_tmp_buf_cur_ptr == '|') 
                token.kind = tok::pipepipe;
            
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::pipe;

        break;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
    case ':':
        if (*_tmp_buf_cur_ptr == ':') {
            token.kind = tok::coloncolon;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::colon;

        break;
    case ';':
        token.kind = tok::semi;

        break;
    case '=':
        if (*_tmp_buf_cur_ptr == '='){

            token.kind = tok::equalequal;
            _tmp_buf_cur_ptr++;
        }
        else 
            token.kind = tok::equal;

        break;
#ifndef YLIB_CODECOVERAGE_SKIP_CODE
    case ',':
        token.kind = tok::comma;
        break;
    case '#':
        token.kind = tok::hash;
        break;

    case '@':
        token.kind = tok::unknown;
        break;

    // UCNs (C99 6.4.3, C++11 [lex.charset]p2)
    case '\\':
        token.kind = tok::unknown;
        break;
#endif //YLIB_CODECOVERAGE_SKIP_CODE
    default:
        break;
    }

    UpdateToken(token, _tmp_buf_cur_ptr, token.kind);
    return true;
}
