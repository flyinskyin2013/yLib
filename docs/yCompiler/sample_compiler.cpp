/*
 * @Author: Sky
 * @Date: 2021-11-20 10:32:54
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-11-20 10:43:27
 * @Description: 
 */

// Ref:
// https://github.com/lotabout/write-a-C-interpreter
// https://lotabout.me/2015/write-a-C-interpreter-1/
#include <fstream>
#include <cstdint>
#include <memory>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>


uint32_t token;                    // current token
uint32_t token_val;                // value of current token (mainly for number)
char *src, *old_src;          // pointer to source code string;
uint32_t poolsize;                 // default size of text/data/stack
uint32_t line;                     // line number
uint32_t *text,                    // text segment
    *old_text,                // for dump text segment
    *stack;                   // stack
char *data;                   // data segment
uint32_t *pc, *bp, *sp, ax, cycle; // virtual machine registers
uint32_t *current_id,              // current parsed ID
    *symbols;                 // symbol table
uint32_t *idmain;                  // the `main` function

// instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };

// tokens and classes (operators last and in precedence order)
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};


// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};

// types of variable/function
enum { CHAR, INT, PTR };

uint32_t basetype;    // the type of a declaration, make it global for convenience
uint32_t expr_type;   // the type of an expression

// function frame
//
// 0: arg 1
// 1: arg 2
// 2: arg 3
// 3: return address
// 4: old bp pointer  <- index_of_bp
// 5: local var 1
// 6: local var 2
uint32_t index_of_bp; // index of bp pointer on stack

void next() {
    char *last_pos;
    uint32_t hash;

    // while (token = *src) {
    //     ++src;

    //     // parse token here
    //     if (token == '\n') {
    //         ++line;
    //     }
    //     else if (token == '#') {
    //         // skip macro, because we will not support it
    //         while (*src != 0 && *src != '\n') {
    //             src++;
    //         }
    //     }
    //     else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {

    //         // parse identifier
    //         last_pos = src - 1;
    //         hash = token;

    //         while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
    //             hash = hash * 147 + *src;
    //             src++;
    //         }

    //         // look for existing identifier, linear search
    //         current_id = symbols;
    //         while (current_id[Token]) {
    //             if (current_id[Hash] == hash && !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
    //                 //found one, return
    //                 token = current_id[Token];
    //                 return;
    //             }
    //             current_id = current_id + IdSize;
    //         }


    //         // store new ID
    //         current_id[Name] = (int)last_pos;
    //         current_id[Hash] = hash;
    //         token = current_id[Token] = Id;
    //         return;
    //     }
    //     else if (token >= '0' && token <= '9') {
    //         // parse number, three kinds: dec(123) hex(0x123) oct(017)
    //         token_val = token - '0';
    //         if (token_val > 0) {
    //             // dec, starts with [1-9]
    //             while (*src >= '0' && *src <= '9') {
    //                 token_val = token_val*10 + *src++ - '0';
    //             }
    //         } else {
    //             // starts with 0
    //             if (*src == 'x' || *src == 'X') {
    //                 //hex
    //                 token = *++src;
    //                 while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' && token <= 'F')) {
    //                     token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
    //                     token = *++src;
    //                 }
    //             } else {
    //                 // oct
    //                 while (*src >= '0' && *src <= '7') {
    //                     token_val = token_val*8 + *src++ - '0';
    //                 }
    //             }
    //         }

    //         token = Num;
    //         return;
    //     }
    //     else if (token == '"' || token == '\'') {
    //         // parse string literal, currently, the only supported escape
    //         // character is '\n', store the string literal into data.
    //         last_pos = data;
    //         while (*src != 0 && *src != token) {
    //             token_val = *src++;
    //             if (token_val == '\\') {
    //                 // escape character
    //                 token_val = *src++;
    //                 if (token_val == 'n') {
    //                     token_val = '\n';
    //                 }
    //             }

    //             if (token == '"') {
    //                 *data++ = token_val;
    //             }
    //         }

    //         src++;
    //         // if it is a single character, return Num token
    //         if (token == '"') {
    //             token_val = (int)last_pos;
    //         } else {
    //             token = Num;
    //         }

    //         return;
    //     }
    //     else if (token == '/') {
    //         if (*src == '/') {
    //             // skip comments
    //             while (*src != 0 && *src != '\n') {
    //                 ++src;
    //             }
    //         } else {
    //             // divide operator
    //             token = Div;
    //             return;
    //         }
    //     }
    //     else if (token == '=') {
    //         // parse '==' and '='
    //         if (*src == '=') {
    //             src ++;
    //             token = Eq;
    //         } else {
    //             token = Assign;
    //         }
    //         return;
    //     }
    //     else if (token == '+') {
    //         // parse '+' and '++'
    //         if (*src == '+') {
    //             src ++;
    //             token = Inc;
    //         } else {
    //             token = Add;
    //         }
    //         return;
    //     }
    //     else if (token == '-') {
    //         // parse '-' and '--'
    //         if (*src == '-') {
    //             src ++;
    //             token = Dec;
    //         } else {
    //             token = Sub;
    //         }
    //         return;
    //     }
    //     else if (token == '!') {
    //         // parse '!='
    //         if (*src == '=') {
    //             src++;
    //             token = Ne;
    //         }
    //         return;
    //     }
    //     else if (token == '<') {
    //         // parse '<=', '<<' or '<'
    //         if (*src == '=') {
    //             src ++;
    //             token = Le;
    //         } else if (*src == '<') {
    //             src ++;
    //             token = Shl;
    //         } else {
    //             token = Lt;
    //         }
    //         return;
    //     }
    //     else if (token == '>') {
    //         // parse '>=', '>>' or '>'
    //         if (*src == '=') {
    //             src ++;
    //             token = Ge;
    //         } else if (*src == '>') {
    //             src ++;
    //             token = Shr;
    //         } else {
    //             token = Gt;
    //         }
    //         return;
    //     }
    //     else if (token == '|') {
    //         // parse '|' or '||'
    //         if (*src == '|') {
    //             src ++;
    //             token = Lor;
    //         } else {
    //             token = Or;
    //         }
    //         return;
    //     }
    //     else if (token == '&') {
    //         // parse '&' and '&&'
    //         if (*src == '&') {
    //             src ++;
    //             token = Lan;
    //         } else {
    //             token = And;
    //         }
    //         return;
    //     }
    //     else if (token == '^') {
    //         token = Xor;
    //         return;
    //     }
    //     else if (token == '%') {
    //         token = Mod;
    //         return;
    //     }
    //     else if (token == '*') {
    //         token = Mul;
    //         return;
    //     }
    //     else if (token == '[') {
    //         token = Brak;
    //         return;
    //     }
    //     else if (token == '?') {
    //         token = Cond;
    //         return;
    //     }
    //     else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':') {
    //         // directly return the character as token;
    //         return;
    //     }
    // }
    return;
}

void match(uint32_t tk) {
    if (token == tk) {
        next();
    } else {
        printf("%d: expected token: %d\n", line, tk);
        exit(-1);
    }
}

void expression(uint32_t level) {
    // expressions have various format.
    // but majorly can be divided into two parts: unit and operator
    // for example `(char) *a[10] = (uint32_t *) func(b > 0 ? 10 : 20);
    // `a[10]` is an unit while `*` is an operator.
    // `func(...)` in total is an unit.
    // so we should first parse those unit and unary operators
    // and then the binary ones
    //
    // also the expression can be in the following types:
    //
    // 1. unit_unary ::= unit | unit unary_op | unary_op unit
    // 2. expr ::= unit_unary (bin_op unit_unary ...)

    // unit_unary()
    // uint32_t *id;
    // uint32_t tmp;
    // uint32_t *addr;
    // {
    //     if (!token) {
    //         printf("%d: unexpected token EOF of expression\n", line);
    //         exit(-1);
    //     }
    //     if (token == Num) {
    //         match(Num);

    //         // emit code
    //         *++text = IMM;
    //         *++text = token_val;
    //         expr_type = INT;
    //     }
    //     else if (token == '"') {
    //         // continous string "abc" "abc"


    //         // emit code
    //         *++text = IMM;
    //         *++text = token_val;

    //         match('"');
    //         // store the rest strings
    //         while (token == '"') {
    //             match('"');
    //         }

    //         // append the end of string character '\0', all the data are default
    //         // to 0, so just move data one position forward.
    //         data = (char *)(((int)data + sizeof(int)) & (-sizeof(int)));
    //         expr_type = PTR;
    //     }
    //     else if (token == Sizeof) {
    //         // sizeof is actually an unary operator
    //         // now only `sizeof(int)`, `sizeof(char)` and `sizeof(*...)` are
    //         // supported.
    //         match(Sizeof);
    //         match('(');
    //         expr_type = INT;

    //         if (token == Int) {
    //             match(Int);
    //         } else if (token == Char) {
    //             match(Char);
    //             expr_type = CHAR;
    //         }

    //         while (token == Mul) {
    //             match(Mul);
    //             expr_type = expr_type + PTR;
    //         }

    //         match(')');

    //         // emit code
    //         *++text = IMM;
    //         *++text = (expr_type == CHAR) ? sizeof(char) : sizeof(int);

    //         expr_type = INT;
    //     }
    //     else if (token == Id) {
    //         // there are several type when occurs to Id
    //         // but this is unit, so it can only be
    //         // 1. function call
    //         // 2. Enum variable
    //         // 3. global/local variable
    //         match(Id);

    //         id = current_id;

    //         if (token == '(') {
    //             // function call
    //             match('(');

    //             // pass in arguments
    //             tmp = 0; // number of arguments
    //             while (token != ')') {
    //                 expression(Assign);
    //                 *++text = PUSH;
    //                 tmp ++;

    //                 if (token == ',') {
    //                     match(',');
    //                 }

    //             }
    //             match(')');

    //             // emit code
    //             if (id[Class] == Sys) {
    //                 // system functions
    //                 *++text = id[Value];
    //             }
    //             else if (id[Class] == Fun) {
    //                 // function call
    //                 *++text = CALL;
    //                 *++text = id[Value];
    //             }
    //             else {
    //                 printf("%d: bad function call\n", line);
    //                 exit(-1);
    //             }

    //             // clean the stack for arguments
    //             if (tmp > 0) {
    //                 *++text = ADJ;
    //                 *++text = tmp;
    //             }
    //             expr_type = id[Type];
    //         }
    //         else if (id[Class] == Num) {
    //             // enum variable
    //             *++text = IMM;
    //             *++text = id[Value];
    //             expr_type = INT;
    //         }
    //         else {
    //             // variable
    //             if (id[Class] == Loc) {
    //                 *++text = LEA;
    //                 *++text = index_of_bp - id[Value];
    //             }
    //             else if (id[Class] == Glo) {
    //                 *++text = IMM;
    //                 *++text = id[Value];
    //             }
    //             else {
    //                 printf("%d: undefined variable\n", line);
    //                 exit(-1);
    //             }

    //             // emit code, default behaviour is to load the value of the
    //             // address which is stored in `ax`
    //             expr_type = id[Type];
    //             *++text = (expr_type == CHAR) ? LC : LI;
    //         }
    //     }
    //     else if (token == '(') {
    //         // cast or parenthesis
    //         match('(');
    //         if (token == Int || token == Char) {
    //             tmp = (token == Char) ? CHAR : INT; // cast type
    //             match(token);
    //             while (token == Mul) {
    //                 match(Mul);
    //                 tmp = tmp + PTR;
    //             }

    //             match(')');

    //             expression(Inc); // cast has precedence as Inc(++)

    //             expr_type  = tmp;
    //         } else {
    //             // normal parenthesis
    //             expression(Assign);
    //             match(')');
    //         }
    //     }
    //     else if (token == Mul) {
    //         // dereference *<addr>
    //         match(Mul);
    //         expression(Inc); // dereference has the same precedence as Inc(++)

    //         if (expr_type >= PTR) {
    //             expr_type = expr_type - PTR;
    //         } else {
    //             printf("%d: bad dereference\n", line);
    //             exit(-1);
    //         }

    //         *++text = (expr_type == CHAR) ? LC : LI;
    //     }
    //     else if (token == And) {
    //         // get the address of
    //         match(And);
    //         expression(Inc); // get the address of
    //         if (*text == LC || *text == LI) {
    //             text --;
    //         } else {
    //             printf("%d: bad address of\n", line);
    //             exit(-1);
    //         }

    //         expr_type = expr_type + PTR;
    //     }
    //     else if (token == '!') {
    //         // not
    //         match('!');
    //         expression(Inc);

    //         // emit code, use <expr> == 0
    //         *++text = PUSH;
    //         *++text = IMM;
    //         *++text = 0;
    //         *++text = EQ;

    //         expr_type = INT;
    //     }
    //     else if (token == '~') {
    //         // bitwise not
    //         match('~');
    //         expression(Inc);

    //         // emit code, use <expr> XOR -1
    //         *++text = PUSH;
    //         *++text = IMM;
    //         *++text = -1;
    //         *++text = XOR;

    //         expr_type = INT;
    //     }
    //     else if (token == Add) {
    //         // +var, do nothing
    //         match(Add);
    //         expression(Inc);

    //         expr_type = INT;
    //     }
    //     else if (token == Sub) {
    //         // -var
    //         match(Sub);

    //         if (token == Num) {
    //             *++text = IMM;
    //             *++text = -token_val;
    //             match(Num);
    //         } else {

    //             *++text = IMM;
    //             *++text = -1;
    //             *++text = PUSH;
    //             expression(Inc);
    //             *++text = MUL;
    //         }

    //         expr_type = INT;
    //     }
    //     else if (token == Inc || token == Dec) {
    //         tmp = token;
    //         match(token);
    //         expression(Inc);
    //         if (*text == LC) {
    //             *text = PUSH;  // to duplicate the address
    //             *++text = LC;
    //         } else if (*text == LI) {
    //             *text = PUSH;
    //             *++text = LI;
    //         } else {
    //             printf("%d: bad lvalue of pre-increment\n", line);
    //             exit(-1);
    //         }
    //         *++text = PUSH;
    //         *++text = IMM;
    //         *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
    //         *++text = (tmp == Inc) ? ADD : SUB;
    //         *++text = (expr_type == CHAR) ? SC : SI;
    //     }
    //     else {
    //         printf("%d: bad expression\n", line);
    //         exit(-1);
    //     }
    // }

    // // binary operator and postfix operators.
    // {
    //     while (token >= level) {
    //         // handle according to current operator's precedence
    //         tmp = expr_type;
    //         if (token == Assign) {
    //             // var = expr;
    //             match(Assign);
    //             if (*text == LC || *text == LI) {
    //                 *text = PUSH; // save the lvalue's pointer
    //             } else {
    //                 printf("%d: bad lvalue in assignment\n", line);
    //                 exit(-1);
    //             }
    //             expression(Assign);

    //             expr_type = tmp;
    //             *++text = (expr_type == CHAR) ? SC : SI;
    //         }
    //         else if (token == Cond) {
    //             // expr ? a : b;
    //             match(Cond);
    //             *++text = JZ;
    //             addr = ++text;
    //             expression(Assign);
    //             if (token == ':') {
    //                 match(':');
    //             } else {
    //                 printf("%d: missing colon in conditional\n", line);
    //                 exit(-1);
    //             }
    //             *addr = (int)(text + 3);
    //             *++text = JMP;
    //             addr = ++text;
    //             expression(Cond);
    //             *addr = (int)(text + 1);
    //         }
    //         else if (token == Lor) {
    //             // logic or
    //             match(Lor);
    //             *++text = JNZ;
    //             addr = ++text;
    //             expression(Lan);
    //             *addr = (int)(text + 1);
    //             expr_type = INT;
    //         }
    //         else if (token == Lan) {
    //             // logic and
    //             match(Lan);
    //             *++text = JZ;
    //             addr = ++text;
    //             expression(Or);
    //             *addr = (int)(text + 1);
    //             expr_type = INT;
    //         }
    //         else if (token == Or) {
    //             // bitwise or
    //             match(Or);
    //             *++text = PUSH;
    //             expression(Xor);
    //             *++text = OR;
    //             expr_type = INT;
    //         }
    //         else if (token == Xor) {
    //             // bitwise xor
    //             match(Xor);
    //             *++text = PUSH;
    //             expression(And);
    //             *++text = XOR;
    //             expr_type = INT;
    //         }
    //         else if (token == And) {
    //             // bitwise and
    //             match(And);
    //             *++text = PUSH;
    //             expression(Eq);
    //             *++text = AND;
    //             expr_type = INT;
    //         }
    //         else if (token == Eq) {
    //             // equal ==
    //             match(Eq);
    //             *++text = PUSH;
    //             expression(Ne);
    //             *++text = EQ;
    //             expr_type = INT;
    //         }
    //         else if (token == Ne) {
    //             // not equal !=
    //             match(Ne);
    //             *++text = PUSH;
    //             expression(Lt);
    //             *++text = NE;
    //             expr_type = INT;
    //         }
    //         else if (token == Lt) {
    //             // less than
    //             match(Lt);
    //             *++text = PUSH;
    //             expression(Shl);
    //             *++text = LT;
    //             expr_type = INT;
    //         }
    //         else if (token == Gt) {
    //             // greater than
    //             match(Gt);
    //             *++text = PUSH;
    //             expression(Shl);
    //             *++text = GT;
    //             expr_type = INT;
    //         }
    //         else if (token == Le) {
    //             // less than or equal to
    //             match(Le);
    //             *++text = PUSH;
    //             expression(Shl);
    //             *++text = LE;
    //             expr_type = INT;
    //         }
    //         else if (token == Ge) {
    //             // greater than or equal to
    //             match(Ge);
    //             *++text = PUSH;
    //             expression(Shl);
    //             *++text = GE;
    //             expr_type = INT;
    //         }
    //         else if (token == Shl) {
    //             // shift left
    //             match(Shl);
    //             *++text = PUSH;
    //             expression(Add);
    //             *++text = SHL;
    //             expr_type = INT;
    //         }
    //         else if (token == Shr) {
    //             // shift right
    //             match(Shr);
    //             *++text = PUSH;
    //             expression(Add);
    //             *++text = SHR;
    //             expr_type = INT;
    //         }
    //         else if (token == Add) {
    //             // add
    //             match(Add);
    //             *++text = PUSH;
    //             expression(Mul);

    //             expr_type = tmp;
    //             if (expr_type > PTR) {
    //                 // pointer type, and not `char *`
    //                 *++text = PUSH;
    //                 *++text = IMM;
    //                 *++text = sizeof(int);
    //                 *++text = MUL;
    //             }
    //             *++text = ADD;
    //         }
    //         else if (token == Sub) {
    //             // sub
    //             match(Sub);
    //             *++text = PUSH;
    //             expression(Mul);
    //             if (tmp > PTR && tmp == expr_type) {
    //                 // pointer subtraction
    //                 *++text = SUB;
    //                 *++text = PUSH;
    //                 *++text = IMM;
    //                 *++text = sizeof(int);
    //                 *++text = DIV;
    //                 expr_type = INT;
    //             } else if (tmp > PTR) {
    //                 // pointer movement
    //                 *++text = PUSH;
    //                 *++text = IMM;
    //                 *++text = sizeof(int);
    //                 *++text = MUL;
    //                 *++text = SUB;
    //                 expr_type = tmp;
    //             } else {
    //                 // numeral subtraction
    //                 *++text = SUB;
    //                 expr_type = tmp;
    //             }
    //         }
    //         else if (token == Mul) {
    //             // multiply
    //             match(Mul);
    //             *++text = PUSH;
    //             expression(Inc);
    //             *++text = MUL;
    //             expr_type = tmp;
    //         }
    //         else if (token == Div) {
    //             // divide
    //             match(Div);
    //             *++text = PUSH;
    //             expression(Inc);
    //             *++text = DIV;
    //             expr_type = tmp;
    //         }
    //         else if (token == Mod) {
    //             // Modulo
    //             match(Mod);
    //             *++text = PUSH;
    //             expression(Inc);
    //             *++text = MOD;
    //             expr_type = tmp;
    //         }
    //         else if (token == Inc || token == Dec) {
    //             // postfix inc(++) and dec(--)
    //             // we will increase the value to the variable and decrease it
    //             // on `ax` to get its original value.
    //             if (*text == LI) {
    //                 *text = PUSH;
    //                 *++text = LI;
    //             }
    //             else if (*text == LC) {
    //                 *text = PUSH;
    //                 *++text = LC;
    //             }
    //             else {
    //                 printf("%d: bad value in increment\n", line);
    //                 exit(-1);
    //             }

    //             *++text = PUSH;
    //             *++text = IMM;
    //             *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
    //             *++text = (token == Inc) ? ADD : SUB;
    //             *++text = (expr_type == CHAR) ? SC : SI;
    //             *++text = PUSH;
    //             *++text = IMM;
    //             *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
    //             *++text = (token == Inc) ? SUB : ADD;
    //             match(token);
    //         }
    //         else if (token == Brak) {
    //             // array access var[xx]
    //             match(Brak);
    //             *++text = PUSH;
    //             expression(Assign);
    //             match(']');

    //             if (tmp > PTR) {
    //                 // pointer, `not char *`
    //                 *++text = PUSH;
    //                 *++text = IMM;
    //                 *++text = sizeof(int);
    //                 *++text = MUL;
    //             }
    //             else if (tmp < PTR) {
    //                 printf("%d: pointer type expected\n", line);
    //                 exit(-1);
    //             }
    //             expr_type = tmp - PTR;
    //             *++text = ADD;
    //             *++text = (expr_type == CHAR) ? LC : LI;
    //         }
    //         else {
    //             printf("%d: compiler error, token = %d\n", line, token);
    //             exit(-1);
    //         }
    //     }
    // }
}

void statement() {
    // there are 6 kinds of statements here:
    // 1. if (...) <statement> [else <statement>]
    // 2. while (...) <statement>
    // 3. { <statement> }
    // 4. return xxx;
    // 5. <empty statement>;
    // 6. expression; (expression end with semicolon)

//     uint32_t *a, *b; // bess for branch control

//     if (token == If) {
//         // if (...) <statement> [else <statement>]
//         //
//         //   if (...)           <cond>
//         //                      JZ a
//         //     <statement>      <statement>
//         //   else:              JMP b
//         // a:                 a:
//         //     <statement>      <statement>
//         // b:                 b:
//         //
//         //
//         match(If);
//         match('(');
//         expression(Assign);  // parse condition
//         match(')');

//         // emit code for if
//         *++text = JZ;
//         b = ++text;

//         statement();         // parse statement
//         if (token == Else) { // parse else
//             match(Else);

//             // emit code for JMP B
//             *b = (int)(text + 3);
//             *++text = JMP;
//             b = ++text;

//             statement();
//         }

//         *b = (int)(text + 1);
//     }
//     else if (token == While) {
//         //
//         // a:                     a:
//         //    while (<cond>)        <cond>
//         //                          JZ b
//         //     <statement>          <statement>
//         //                          JMP a
//         // b:                     b:
//         match(While);

//         a = text + 1;

//         match('(');
//         expression(Assign);
//         match(')');

//         *++text = JZ;
//         b = ++text;

//         statement();

//         *++text = JMP;
//         *++text = (int)a;
//         *b = (int)(text + 1);
//     }
//     else if (token == '{') {
//         // { <statement> ... }
//         match('{');

//         while (token != '}') {
//             statement();
//         }

//         match('}');
//     }
//     else if (token == Return) {
//         // return [expression];
//         match(Return);

//         if (token != ';') {
//             expression(Assign);
//         }

//         match(';');

//         // emit code for return
//         *++text = LEV;
//     }
//     else if (token == ';') {
//         // empty statement
//         match(';');
//     }
//     else {
//         // a = b; or function_call();
//         expression(Assign);
//         match(';');
//     }
}

void function_parameter() {
//     uint32_t type;
//     uint32_t params;
//     params = 0;
//     while (token != ')') {
//         // uint32_t name, ...
//         type = INT;
//         if (token == Int) {
//             match(Int);
//         } else if (token == Char) {
//             type = CHAR;
//             match(Char);
//         }

//         // pointer type
//         while (token == Mul) {
//             match(Mul);
//             type = type + PTR;
//         }

//         // parameter name
//         if (token != Id) {
//             printf("%d: bad parameter declaration\n", line);
//             exit(-1);
//         }
//         if (current_id[Class] == Loc) {
//             printf("%d: duplicate parameter declaration\n", line);
//             exit(-1);
//         }

//         match(Id);
//         // store the local variable
//         current_id[BClass] = current_id[Class]; current_id[Class]  = Loc;
//         current_id[BType]  = current_id[Type];  current_id[Type]   = type;
//         current_id[BValue] = current_id[Value]; current_id[Value]  = params++;   // index of current parameter

//         if (token == ',') {
//             match(',');
//         }
//     }
//     index_of_bp = params+1;
}

void function_body() {
    // type func_name (...) {...}
    //                   -->|   |<--

    // ... {
    // 1. local declarations
    // 2. statements
    // }

    // uint32_t pos_local; // position of local variables on the stack.
    // uint32_t type;
    // pos_local = index_of_bp;

    // while (token == Int || token == Char) {
    //     // local variable declaration, just like global ones.
    //     basetype = (token == Int) ? INT : CHAR;
    //     match(token);

    //     while (token != ';') {
    //         type = basetype;
    //         while (token == Mul) {
    //             match(Mul);
    //             type = type + PTR;
    //         }

    //         if (token != Id) {
    //             // invalid declaration
    //             printf("%d: bad local declaration\n", line);
    //             exit(-1);
    //         }
    //         if (current_id[Class] == Loc) {
    //             // identifier exists
    //             printf("%d: duplicate local declaration\n", line);
    //             exit(-1);
    //         }
    //         match(Id);

    //         // store the local variable
    //         current_id[BClass] = current_id[Class]; current_id[Class]  = Loc;
    //         current_id[BType]  = current_id[Type];  current_id[Type]   = type;
    //         current_id[BValue] = current_id[Value]; current_id[Value]  = ++pos_local;   // index of current parameter

    //         if (token == ',') {
    //             match(',');
    //         }
    //     }
    //     match(';');
    // }

    // // save the stack size for local variables
    // *++text = ENT;
    // *++text = pos_local - index_of_bp;

    // // statements
    // while (token != '}') {
    //     statement();
    // }

    // // emit code for leaving the sub function
    // *++text = LEV;
}

void function_declaration() {
    // type func_name (...) {...}
    //               | this part

    match('(');
    function_parameter();
    match(')');
    match('{');
    function_body();
    //match('}');

    // unwind local variable declarations for all local variables.
    current_id = symbols;
    while (current_id[Token]) {
        if (current_id[Class] == Loc) {
            current_id[Class] = current_id[BClass];
            current_id[Type]  = current_id[BType];
            current_id[Value] = current_id[BValue];
        }
        current_id = current_id + IdSize;
    }
}

void enum_declaration() {
    // parse enum [id] { a = 1, b = 3, ...}
    uint32_t i;
    i = 0;
    while (token != '}') {
        if (token != Id) {
            printf("%d: bad enum identifier %d\n", line, token);
            exit(-1);
        }
        next();
        if (token == Assign) {
            // like {a=10}
            next();
            if (token != Num) {
                printf("%d: bad enum initializer\n", line);
                exit(-1);
            }
            i = token_val;
            next();
        }

        current_id[Class] = Num;
        current_id[Type] = INT;
        current_id[Value] = i++;

        if (token == ',') {
            next();
        }
    }
}

void global_declaration() {
    // global_declaration ::= enum_decl | variable_decl | function_decl
    //
    // enum_decl ::= 'enum' [id] '{' id ['=' 'num'] {',' id ['=' 'num'} '}'
    //
    // variable_decl ::= type {'*'} id { ',' {'*'} id } ';'
    //
    // function_decl ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'


    // uint32_t type; // tmp, actual type for variable
    // uint32_t i; // tmp

    // basetype = INT;

    // // parse enum, this should be treated alone.
    // if (token == Enum) {
    //     // enum [id] { a = 10, b = 20, ... }
    //     match(Enum);
    //     if (token != '{') {
    //         match(Id); // skip the [id] part
    //     }
    //     if (token == '{') {
    //         // parse the assign part
    //         match('{');
    //         enum_declaration();
    //         match('}');
    //     }

    //     match(';');
    //     return;
    // }

    // // parse type information
    // if (token == Int) {
    //     match(Int);
    // }
    // else if (token == Char) {
    //     match(Char);
    //     basetype = CHAR;
    // }

    // // parse the comma seperated variable declaration.
    // while (token != ';' && token != '}') {
    //     type = basetype;
    //     // parse pointer type, note that there may exist `uint32_t ****x;`
    //     while (token == Mul) {
    //         match(Mul);
    //         type = type + PTR;
    //     }

    //     if (token != Id) {
    //         // invalid declaration
    //         printf("%d: bad global declaration\n", line);
    //         exit(-1);
    //     }
    //     if (current_id[Class]) {
    //         // identifier exists
    //         printf("%d: duplicate global declaration\n", line);
    //         exit(-1);
    //     }
    //     match(Id);
    //     current_id[Type] = type;

    //     if (token == '(') {
    //         current_id[Class] = Fun;
    //         current_id[Value] = (int)(text + 1); // the memory address of function
    //         function_declaration();
    //     } else {
    //         // variable declaration
    //         current_id[Class] = Glo; // global variable
    //         current_id[Value] = (int)data; // assign memory address
    //         data = data + sizeof(int);
    //     }

    //     if (token == ',') {
    //         match(',');
    //     }
    // }
    // next();
}

void program() {
    // get next token
    next();
    while (token > 0) {
        global_declaration();
    }
}


uint32_t eval() {
    // uint32_t op, *tmp;
    // while (1) {
    //     op = *pc++; // get next operation code

    //     if (op == IMM)       {ax = *pc++;}                                     // load immediate value to ax
    //     else if (op == LC)   {ax = *(char *)ax;}                               // load character to ax, address in ax
    //     else if (op == LI)   {ax = *(uint32_t *)ax;}                                // load integer to ax, address in ax
    //     else if (op == SC)   {ax = *(char *)*sp++ = ax;}                       // save character to address, value in ax, address on stack
    //     else if (op == SI)   {*(uint32_t *)*sp++ = ax;}                             // save integer to address, value in ax, address on stack
    //     else if (op == PUSH) {*--sp = ax;}                                     // push the value of ax onto the stack
    //     else if (op == JMP)  {pc = (uint32_t *)*pc;}                                // jump to the address
    //     else if (op == JZ)   {pc = ax ? pc + 1 : (uint32_t *)*pc;}                   // jump if ax is zero
    //     else if (op == JNZ)  {pc = ax ? (uint32_t *)*pc : pc + 1;}                   // jump if ax is not zero
    //     else if (op == CALL) {*--sp = (int)(pc+1); pc = (uint32_t *)*pc;}           // call subroutine
    //     //else if (op == RET)  {pc = (uint32_t *)*sp++;}                              // return from subroutine;
    //     else if (op == ENT)  {*--sp = (int)bp; bp = sp; sp = sp - *pc++;}      // make new stack frame
    //     else if (op == ADJ)  {sp = sp + *pc++;}                                // add esp, <size>
    //     else if (op == LEV)  {sp = bp; bp = (uint32_t *)*sp++; pc = (uint32_t *)*sp++;}  // restore call frame and PC
    //     else if (op == LEA)  {ax = (int)(bp + *pc++);}                         // load address for arguments.

    //     else if (op == OR)  ax = *sp++ | ax;
    //     else if (op == XOR) ax = *sp++ ^ ax;
    //     else if (op == AND) ax = *sp++ & ax;
    //     else if (op == EQ)  ax = *sp++ == ax;
    //     else if (op == NE)  ax = *sp++ != ax;
    //     else if (op == LT)  ax = *sp++ < ax;
    //     else if (op == LE)  ax = *sp++ <= ax;
    //     else if (op == GT)  ax = *sp++ >  ax;
    //     else if (op == GE)  ax = *sp++ >= ax;
    //     else if (op == SHL) ax = *sp++ << ax;
    //     else if (op == SHR) ax = *sp++ >> ax;
    //     else if (op == ADD) ax = *sp++ + ax;
    //     else if (op == SUB) ax = *sp++ - ax;
    //     else if (op == MUL) ax = *sp++ * ax;
    //     else if (op == DIV) ax = *sp++ / ax;
    //     else if (op == MOD) ax = *sp++ % ax;


    //     else if (op == EXIT) { printf("exit(%d)\n", *sp); return *sp;}
    //     // else if (op == OPEN) { ax = open((char *)sp[1], sp[0]); }
    //     // else if (op == CLOS) { ax = close(*sp);}
    //     // else if (op == READ) { ax = read(sp[2], (char *)sp[1], *sp); }
    //     else if (op == PRTF) { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
    //     else if (op == MALC) { ax = (int)malloc(*sp);}
    //     else if (op == MSET) { ax = (int)memset((char *)sp[2], sp[1], *sp);}
    //     else if (op == MCMP) { ax = memcmp((char *)sp[2], (char *)sp[1], *sp);}
    //     else {
    //         printf("unknown instruction:%d\n", op);
    //         return -1;
    //     }
    // }
    return 0;
}


struct Register{
    // virtual machine registers
    uint32_t* pc = nullptr;
    uint32_t* bp = nullptr;
    uint32_t* sp = nullptr;
    uint32_t ax = 0;
    uint32_t cycle = 0;
};

struct SegmentInfo{

    std::unique_ptr<uint32_t[]> text = nullptr;                    // text segment // for dump text segment
    std::unique_ptr<uint32_t[]> stack = nullptr;                 // stack
    std::unique_ptr<char[]> data = nullptr;                   // data segment
};


// ref:
// clang/Basic/TokenKinds.h
// clang/Basic/TokenKinds.def
namespace tok{

    enum TokenKind : uint16_t {
    #define TOK(X) X,
    #include "./TokenKinds.def"
    NUM_TOKENS
    };
    
    static const char * const TokNames[] = {
    #define TOK(X) #X,
    #include "./TokenKinds.def"
    nullptr
    };

    /// The name of a token will be an internal name (such as "l_square")
    /// and should not be used as part of diagnostic messages.
    const char *getTokenName(TokenKind Kind){

        if (Kind < tok::NUM_TOKENS)
            return TokNames[(uint16_t)Kind];

        return nullptr;
    }
}

namespace file{

    class SourceLocation;
}
namespace diag{

    enum DiagID{
        #define DIAG(name, level, info) \
            name,
        #include "DiagnosticKinds.def"
        NUM_DIAGID
    };

    class DiagnosticsEngine
    {
    private:
        /* data */
    public:
        void DiagReport(file::SourceLocation & loc, DiagID id);
    };
    
}

namespace file{

    class SourceLocation{
        public:
        uint64_t loc_offset = 0;
        const char * cur_buf_start = nullptr;
    };

    class FileManager{

        public:

        std::unique_ptr<char[]> content_buf = nullptr;
        uint64_t content_buf_size = 0;

        bool Init(const char * file_name){

            std::ifstream _in_file(file_name);

            if (!_in_file.is_open()){

                printf("Can't open input-c-file(%s)!!!\n", file_name);
                return false;        
            }

            _in_file.seekg(0, _in_file.end);
            ssize_t _content_size = _in_file.tellg();
            _in_file.seekg(0, _in_file.beg);    

            content_buf_size = _content_size;        

            if (nullptr == (content_buf = std::unique_ptr<char[]>(new (std::nothrow) char[content_buf_size + 1]))) {
                printf("could not malloc(%d) for source area\n", content_buf_size);
                return false;
            }

            // reset buf
            ::memset(content_buf.get(), 0, sizeof(char)*(content_buf_size + 1));

            //read file-content
            _in_file.read(content_buf.get(), content_buf_size);

            _in_file.close();

            return true;
        }
    };
}

namespace lexer
{
    class yToken{

        public:
        tok::TokenKind kind;

        /// token_len - This holds either the length of the token text, when
        /// a normal token, or the end of the SourceRange when an annotation
        /// token.
        uint64_t token_len;

        /// PtrData - This is a union of four different pointer types, which depends
        /// on what type of token this is:
        ///  Identifiers, keywords, etc:
        ///    This is an IdentifierInfo*, which contains the uniqued identifier
        ///    spelling.
        ///  Literals:  isLiteral() returns true.
        ///    This is a pointer to the start of the token in a text buffer, which
        ///    may be dirty (have trigraphs / escaped newlines).
        ///  Annotations (resolved type names, C++ scopes, etc): isAnnotation().
        ///    This is a pointer to sema-specific data for the annotation token.
        ///  Eof:
        //     This is a pointer to a Decl.
        ///  Other:
        ///    This is null.
        void *token_data;


        void InitToken(void){

            kind  = tok::unknown;
            token_len = 0;
            token_data = nullptr;
        }
    };

    class yLexer{

        public:
        const char * buf_start;
        const char * buf_end;
        const char * buf_cur_ptr;
        uint64_t row_num = 0;
        uint64_t col_num = 0;

        bool Init(file::FileManager & manager){

            buf_start = buf_cur_ptr = manager.content_buf.get();
            buf_end = manager.content_buf.get() + manager.content_buf_size + 1;

            // Check whether we have a BOM in the beginning of the buffer. If yes - act
            // accordingly. Right now we support only UTF-8 with and without BOM, so, just
            // skip the UTF-8 BOM if it's present.
            if (buf_cur_ptr[0] == 0xEF && buf_cur_ptr[1] == 0xBB && buf_cur_ptr[0] == 0xBF) {

                buf_cur_ptr += 3;
            }
        }
        void UpdateToken(lexer::yToken &token, const char *tok_end, tok::TokenKind kind){

            token.kind = kind;
            token.token_len = tok_end - buf_cur_ptr;
            token.token_data = (void *)buf_cur_ptr;

            buf_cur_ptr = tok_end;//update buf_cur_ptr
        }

        bool GetEndOfFileToken(lexer::yToken &token, const char * cur_ptr){

            UpdateToken(token, buf_end, tok::eof);
            return true;
        }

        bool IsNumericChar(const char * cur_ptr){

            if ( *cur_ptr >= '0' && *cur_ptr <= '9') return true;

            return false;
        }

        bool GetNumericConstant(lexer::yToken &token, const char * cur_ptr){

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

        bool IsIdentifierBodyChar(const char * cur_ptr){

            if ( (*cur_ptr >= 'a' && *cur_ptr <= 'z') || \
                (*cur_ptr >= 'A' && *cur_ptr <= 'Z') || \
                (*cur_ptr >= '0' && *cur_ptr <= '9') || \
                *cur_ptr == '_'
            ) return true;

            return false;
        }
        bool GetIdentifier(lexer::yToken &token, const char * cur_ptr){

            while(IsIdentifierBodyChar(cur_ptr))
                cur_ptr ++;
            
            UpdateToken(token, cur_ptr, tok::identifier);
            return true;
        }

        bool GetCharConstant(lexer::yToken &token, const char * cur_ptr){

            if (cur_ptr[0] == '\\'){//

                cur_ptr++;
                cur_ptr++;//real char
                if (cur_ptr[0] != '\'')
                    return false;
            }
            else{

                cur_ptr++;
                if (cur_ptr[0] != '\'')
                    return false;                
            }

            UpdateToken(token, cur_ptr, tok::char_constant);         
        }

        bool GetStringLiteral(lexer::yToken &token, const char * cur_ptr){

            while(*cur_ptr != '"') cur_ptr++;

            cur_ptr++;//move ptr to after second '"'
            UpdateToken(token, cur_ptr, tok::string_literal);  
        }

        bool TryNextToken(lexer::yToken &token){

            NextToken(token);

            // back to origin
            buf_cur_ptr -= token.token_len;
        }
        bool NextToken(lexer::yToken &token){

            LEX_NEXT_TOKEN:
            
            const char * _tmp_buf_cur_ptr = buf_cur_ptr;
            const char _tmp_char = *_tmp_buf_cur_ptr;
            _tmp_buf_cur_ptr++;

            token.InitToken();

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
            case '\r':{
                
                // new line
                if ('\n' == *_tmp_buf_cur_ptr){
                    
                    row_num ++;
                    col_num = 0;

                    _tmp_buf_cur_ptr++;
                    buf_cur_ptr = _tmp_buf_cur_ptr;
                    goto LEX_NEXT_TOKEN;
                }
                

                token.kind = tok::unknown;
                break;
            }
            case '\n':{// new line

                row_num ++;
                col_num = 0;

                buf_cur_ptr = _tmp_buf_cur_ptr;
                goto LEX_NEXT_TOKEN;
                break;
            }
            case ' ':
            case '\t':
            case '\f'://换页(FF)，将当前位置移到下页开头, ascii = 0xC
            case '\v'://ascii = 0xB
            {

                if ('\t' == _tmp_char )
                    col_num += 4;
                else
                    col_num ++;
                
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
            case '$':{

                token.kind = tok::unknown;
                break;
            }
            // C99 6.4.4: Character Constants.
            case '\'':{

                return GetCharConstant(token, _tmp_buf_cur_ptr);
                break;
            }
            // C99 6.4.5: String Literals.
            case '"':{

                return GetStringLiteral(token, _tmp_buf_cur_ptr);
                break;
            }
            // C99 6.4.6: Punctuators.
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
            case '{':
                token.kind = tok::l_brace;
                break;
            case '}':
                token.kind = tok::r_brace;
                break;
            // case '.': this is processed in GetNumericConstant
            //     break;
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
            case '+':

                if (*_tmp_buf_cur_ptr == '+' || *_tmp_buf_cur_ptr == '='){

                    if (*_tmp_buf_cur_ptr == '+') 
                        token.kind = tok::plusplus;
                    else if (*_tmp_buf_cur_ptr == '=') 
                        token.kind = tok::plusequal;
                    
                    _tmp_buf_cur_ptr++;
                }
                else 
                    token.kind = tok::plus;

                break;
            case '-':

                if (*_tmp_buf_cur_ptr == '-' || *_tmp_buf_cur_ptr == '>' || *_tmp_buf_cur_ptr == '='){

                    if (*_tmp_buf_cur_ptr == '-')      // --
                        token.kind = tok::minusminus;
                    else if (*_tmp_buf_cur_ptr == '>')    // ->
                        token.kind = tok::arrow;
                    else if (*_tmp_buf_cur_ptr == '=')    // -=
                        token.kind = tok::minusequal;
                    _tmp_buf_cur_ptr++;
                }
                else 
                    token.kind = tok::minus;

                break;
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
            case '/':
                // 6.4.9: Comments
                if (*_tmp_buf_cur_ptr == '/') {         // Line comment.

                    while(*_tmp_buf_cur_ptr != '\n' || *_tmp_buf_cur_ptr != '\r')
                        _tmp_buf_cur_ptr ++;
                    
                    if (*_tmp_buf_cur_ptr == '\r' && _tmp_buf_cur_ptr[1] == '\n'){

                        _tmp_buf_cur_ptr ++;
                        
                        UpdateToken(token, _tmp_buf_cur_ptr, tok::comment);

                        row_num ++;
                        col_num = 0;
                        return true;
                    }

                    token.kind = tok::unknown;
                    break;
                }
                else if (*_tmp_buf_cur_ptr == '*') {  // /**/ comment.

                    _tmp_buf_cur_ptr ++;//to next pos

                    RE_CONTINUE_COMMENT:
                    while(*_tmp_buf_cur_ptr != '*')
                        _tmp_buf_cur_ptr++;

                    _tmp_buf_cur_ptr++;
                    if (*_tmp_buf_cur_ptr != '/'){
                        _tmp_buf_cur_ptr++;
                        goto RE_CONTINUE_COMMENT;
                    }
                    
                    _tmp_buf_cur_ptr ++;//to next pos
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
            default:
                break;
            }

            UpdateToken(token, _tmp_buf_cur_ptr, token.kind);
            return true;
        }
    };
    
} // namespace lexer


namespace parser{

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
    object-list ','
    object-list ',' object

Syntax3:
object:    
    identifier ':' '{' declaration-list '}'

Syntax4:
declaration-list:
    declaration-item
    declaration-item ','
    object
    object ','
    declaration-list ',' declaration-item
    declaration-list ',' object

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
    class Parser
    {
    private:
        /* data */
    public:
        lexer::yLexer lexer;
        lexer::yToken cur_token;
        lexer::yToken try_token;
        bool ParserTranslationUnit(void){

            return ParserObjectList();
        }
        bool ParserDeclareItem(void){

            if (cur_token.kind != tok::identifier){

                printf("a declareitem must start of identifier. cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }
            std::string _identifier_str((char *)cur_token.token_data, cur_token.token_len);

            lexer.NextToken(cur_token);//next tok
            if (cur_token.kind != tok::equal){

                printf("declareitem second field must be equal. cur token kind %s\n", tok::getTokenName(cur_token.kind));
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
            lexer.NextToken(cur_token);//next tok
            if (cur_token.kind == tok::numeric_constant){

                std::string _str((char *)cur_token.token_data, cur_token.token_len);
                if (_str.find('.') == _str.npos){//int

                    printf("identifier %s, int value is %d\n", _identifier_str.c_str(),  std::atoi(_str.c_str()));
                }
                else{//float

                    printf("identifier %s, float value is %f\n", _identifier_str.c_str(), std::atof(_str.c_str()));
                }
                lexer.NextToken(cur_token);
            }
            else if (cur_token.kind == tok::string_literal){

                std::string _str((char *)cur_token.token_data, cur_token.token_len);
                printf("identifier %s, str value is %s\n", _identifier_str.c_str(), _str.c_str());
                lexer.NextToken(cur_token);
            }
            else if (cur_token.kind == tok::identifier){//bool-constant only true or false

                std::string _str((char *)cur_token.token_data, cur_token.token_len);
                if (_str == "true" || _str == "false"){

                    printf("identifier %s, bool value is %s\n", _identifier_str.c_str(), _str.c_str());
                }
                else{

                    printf("the bool value must be true or false.but get str %s\n", _str.c_str());
                }
                
                lexer.NextToken(cur_token);
            }
            else{

                printf("declareitem value field must be float/int/string/bool. cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }

            if (cur_token.kind == tok::comma)//skip ',' before DeclareItem
                lexer.NextToken(cur_token);

            return true;
        }
        bool ParserDeclareList(void){
            
            while(cur_token.kind == tok::identifier){

                if (cur_token.kind != tok::identifier){

                    printf("a declarelist must start of identifier. cur token kind %s\n", tok::getTokenName(cur_token.kind));
                    return false;
                }

                lexer.TryNextToken(try_token);//try next token
                if (try_token.kind == tok::colon || try_token.kind == tok::equal){

                    if (try_token.kind == tok::colon){//object
                        if (!ParserObject())
                            return false;
                    }
                    else{//declare item
                        if (!ParserDeclareItem())
                            return false;
                    }
                }
                else{

                    printf("Syntax error for tokenType %s, row %d, col %d\n", tok::getTokenName(try_token.kind), lexer.row_num, lexer.col_num);
                    return false;                
                }
            }

            return true;
        }

        bool ParserObject(void){

            if (cur_token.kind != tok::identifier){

                printf("object must start of identifier. cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }
            lexer.NextToken(cur_token);//next token
            if (cur_token.kind != tok::colon){

                printf("object second field is  ':', cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }

            lexer.NextToken(cur_token);//next token
            if (cur_token.kind != tok::l_brace){

                printf("object third field is  '{', cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }
            
            lexer.NextToken(cur_token);//next token, object body
            if (cur_token.kind != tok::r_brace)//not a empty body
                if (!ParserDeclareList())
                    return false;
            
            //we get new token in ParserObject or ParserDeclareList
            if (cur_token.kind != tok::r_brace){

                printf("object final field is  '}', cur token kind %s\n", tok::getTokenName(cur_token.kind));
                return false;
            }

            lexer.NextToken(cur_token);//next token
            if (cur_token.kind == tok::comma)
                lexer.NextToken(cur_token);//next token

            return true;
        }
        bool ParserObjectList(void){

            lexer.NextToken(cur_token);//get first token
            if (cur_token.kind != tok::identifier && cur_token.kind != tok::comment){

                printf("expect a object or comment\n");
                return false;
            }
            while(cur_token.kind == tok::identifier || cur_token.kind == tok::comment){

                if (cur_token.kind == tok::comment){

                    printf("This is a comment token\n");
                    lexer.NextToken(cur_token);//consumer token
                    continue;//skip
                }

                if (!ParserObject()){

                    printf("parse a object failed.\n");
                    return false;
                }
            }

            if (cur_token.kind != tok::eof){

                printf("Syntax error for tokenType %s, row %d, col %d\n", tok::getTokenName(cur_token.kind), lexer.row_num, lexer.col_num);
                return false;
            }

            printf("Parse yConfig done.\n");
            return true;
        }

    };

    
} //namespace parser


int8_t InitSegmentInfo(SegmentInfo & info, uint64_t segment_size){

    /*  
    +------------------+
    |    stack   |     |      high address
    |    ...     v     |
    |                  |
    |                  |
    |                  |
    |                  |
    |    ...     ^     |
    |    heap    |     |
    +------------------+
    | bss  segment     |
    +------------------+
    | data segment     |
    +------------------+
    | text segment     |      low address
    +------------------+
*/
    // allocate memory for virtual machine
    if (nullptr == (info.text = std::unique_ptr<uint32_t[]>(new (std::nothrow) uint32_t[segment_size]))) {
        printf("could not malloc(%d) for text area\n", segment_size);
        return -1;
    }
    if (nullptr == (info.data = std::unique_ptr<char[]>(new (std::nothrow) char[segment_size]))) {
        printf("could not malloc(%d) for data area\n", segment_size);
        return -1;
    }
    if (nullptr == (info.stack = std::unique_ptr<uint32_t[]>(new (std::nothrow) uint32_t[segment_size]))) {
        printf("could not malloc(%d) for stack area\n", segment_size);
        return -1;
    }

    ::memset(info.text.get(), 0, segment_size*sizeof(uint32_t));
    ::memset(info.data.get(), 0, segment_size*sizeof(char));
    ::memset(info.stack.get(), 0, segment_size*sizeof(uint32_t));

}



Register g_sys_register;
SegmentInfo g_program_segment_info;

uint32_t g_poolsize = 256*1024;
uint32_t g_line_num = 1;

int main(int argc, char **argv)
{
    if (argc != 2){

        printf("Please input a c-file!!!\n");
        return -1;
    }


    file::FileManager _mgr;
    if (!_mgr.Init(argv[1]))
        return -1;


    //init program segment
    if (0 > InitSegmentInfo(g_program_segment_info, g_poolsize)){

        return -1;
    }

    g_sys_register.bp = g_sys_register.sp = ( uint32_t *)((uintptr_t)(g_program_segment_info.stack.get()) + g_poolsize);
    g_sys_register.ax = 0;

    parser::Parser _parser;;
    _parser.lexer.Init(_mgr);

    _parser.ParserTranslationUnit();
    
    return 0;
}