# yConfig Language / yConfig语言
## Notation / 注释
    This file reference Chapter 6 of IOS/IEC-9899:2011.

## Concepts / 概念
### Scopes of identifiers / 标识符的作用域
    标识符表示一个变量名；

### Name spaces of identifiers / 标识符的命名空间
    标识符在同一个翻译单元同层级可见。在同一个对象同层级中可见。标识符在同一个命名空间里面是唯一的。


## Lexical elements / 词法要素
Syntax:
```text
    token:
        keyword
        identifier
        constant
        string-literal
        punctuator
        
    preprocessing-token:
        import-config-file-name
```

### Keywords / 关键字
    keyword: namespace

### Identifiers / 标识符
Syntax:
```text
    identifier:
        identifier-nondigit
        identifier identifier-nondigit
        identifier digit
    identifier-nondigit:
        nondigit
        universal-character-name
        other implementation-defined characters
    nondigit: one of
        _abcdefghijklm
        nopqrstuvwxyz
        ABCDEFGHIJKLM
        NOPQRSTUVWXYZ
    digit: one of
        0123456789
```    

### Constants / 常量
Syntax
```text
    constant:
        integer-constant
        floating-constant
``` 

#### Integer constants / 整数常量
Syntax
```text
    integer-constant:
        decimal-constant integer-suffixopt
        octal-constant integer-suffixopt
        hexadecimal-constant integer-suffixopt
    decimal-constant:
        nonzero-digit
        decimal-constant digit
    octal-constant:
        0
        octal-constant octal-digit
    hexadecimal-constant:
        hexadecimal-prefix hexadecimal-digit
        hexadecimal-constant hexadecimal-digit
    hexadecimal-prefix: one of
        0x 0X
    nonzero-digit: one of
        123456789
    octal-digit: one of
        01234567
    hexadecimal-digit: one of
        0123456789
        abcdef
        ABCDEF
    integer-suffix:
        unsigned-suffix long-suffixopt
        unsigned-suffix long-long-suffix
        long-suffix unsigned-suffixopt
        long-long-suffix unsigned-suffixopt
    unsigned-suffix: one of
        u U
    long-suffix: one of
        l L
    long-long-suffix: one of
        ll LL
```

#### Floating constants / 浮点常量
Syntax
```text
    floating-constant:
        decimal-floating-constant
        hexadecimal-floating-constant
    decimal-floating-constant:
        fractional-constant exponent-partopt floating-suffixopt
        digit-sequence exponent-part floating-suffixopt
    hexadecimal-floating-constant:
        hexadecimal-prefix hexadecimal-fractional-constant
        binary-exponent-part floating-suffixopt
        hexadecimal-prefix hexadecimal-digit-sequence
        binary-exponent-part floating-suffixopt
    fractional-constant:
        digit-sequenceopt . digit-sequence
        digit-sequence .
    exponent-part:
        e signopt digit-sequence
        E signopt digit-sequence
    sign: one of
        + -
    digit-sequence:
        digit
        digit-sequence digit
    hexadecimal-fractional-constant:
        hexadecimal-digit-sequenceopt .
        hexadecimal-digit-sequence
        hexadecimal-digit-sequence .
    binary-exponent-part:
        p signopt digit-sequence
        P signopt digit-sequence
    hexadecimal-digit-sequence:
        hexadecimal-digit
        hexadecimal-digit-sequence hexadecimal-digit
    floating-suffix: one of
        f l F L
```

### String literals / 字符串
Syntax
```text
    string-literal:
        encoding-prefixopt " s-char-sequenceopt "
    encoding-prefix:
        u8
        u
        U
        L
    s-char-sequence:
        s-char
        s-char-sequence s-char
    s-char:
        any member of the source character set except
        the double-quote ", backslash \, or new-line character
        escape-sequence
```

### Punctuators / 标点符号
Syntax
```text
    punctuator: one of
        = [ ] { } ;
```

### Import Config File Name / 导入配置文件名字
Syntax
```text
    import-config-file-name:
        " q-char-sequence "

    q-char-sequence:
        q-char
        q-char-sequence q-char
    q-char:
        any member of the source character set except
        the new-line character and "
```

### Comments / 注释
```text
exp:
    //

exp:
    /**/
```

## Expressions / 表达式
### Primary expressions / 主表达式
Syntax
```text
    primary-expression:
        identifier
        constant
        string-literal
```

### Postfix operators / 后缀操作符
Syntax
```text
    postfix-expression:
        primary-expression
        postfix-expression [ expression ]
```

### Assignment operators / 等号操作符
Syntax
```text
    assignment-expression:
        postfix-expression
        postfix-expression assignment-operator assignment-expression
    assignment-operator: one of
        =
```

### Comma operator / 逗号操作符
Syntax
```text
    expression:
        assignment-expression
        expression , assignment-expression
```

## Declarations / 声明
Syntax
```text
    declaration:
        declaration-specifiers init-declarator-listopt ;
    declaration-specifiers:
        namespace-specifier
    init-declarator-list:
        init-declarator
        init-declarator-list , init-declarator
    init-declarator:
        declarator
        declarator = initializer
```

### Namespace specifiers / 命名空间说明符
Syntax
```text
    namespace-specifier:
        assignment-expression
        expression , assignment-expression
```

## Statements and blocks / 句子和块
Syntax
```text
    statement:
        compound-statement
        expression-statement
```
### Compound statement / 复合语句
Syntax
```text
    compound-statement:
        { block-item-list[opt] }
    block-item-list:
        block-item
        block-item-list block-item
    block-item:
        declaration
        statement
```

### Expression statements / 表达式语句
Syntax
```text
    expression-statement:
        expression[opt] ;
```

## Preprocessing directives / 预处理指令
Syntax
```text
    preprocessing-file:
        group[opt]
    group:
        group-part
        group group-part
    group-part:
        control-line
    control-line:
        import pp-tokens new-line
    pp-tokens:
        preprocessing-token
        pp-tokens preprocessing-token
    new-line:
        the new-line character    
```