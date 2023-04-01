# yConfig Language / yConfig语言
## Introduction / 引言
&nbsp;&nbsp;&nbsp;&nbsp;本文件主要参考IOS/IEC-9899:2011第6章节（Language）.

## Language / 语言
### Notation / 注释
&nbsp;&nbsp;&nbsp;&nbsp;无。

### Concepts / 概念
#### Scopes of identifiers / 标识符的作用域
&nbsp;&nbsp;&nbsp;&nbsp;标识符表示一个变量名和对象。每个标识符在其作用域中可见。相同标识符表示的实例一定位于不同的作用域中。这里有两种作用域：文件和块。

#### Name spaces of identifiers / 标识符的命名空间
&nbsp;&nbsp;&nbsp;&nbsp;标识符在同一个翻译单元同层级可见。在同一个对象同层级中可见。标识符在同一个命名空间里面是唯一的。

#### Types / 类型
&nbsp;&nbsp;&nbsp;&nbsp;yConfig仅支持共6种类型，它们分别是：对象，数组（当前不支持），64位有符号整形，64位双浮点类型，布尔型，字符串类型。

&nbsp;&nbsp;&nbsp;&nbsp;对于对象来说，它是yConfig的重要组成部分，其定义结构如下：
```
obj:{
    ...
}
```

&nbsp;&nbsp;&nbsp;&nbsp;对于整形和浮点型来说，它们的区别在于定义的时候必须是类
的整形或者浮点定义形式。比如：
```txt
123    整形
123.   浮点型
123f   浮点型
123.1  浮点型
```

&nbsp;&nbsp;&nbsp;&nbsp;对于布尔型来说，其只支持true和false两种值。


&nbsp;&nbsp;&nbsp;&nbsp;对于字符串类型来说，以英文双引号包括的内容定义为字符串，默认用utf-8编码。


### Lexical elements / 词法要素
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;一个token是最小的词法单元。其共有5种类别：keywords, identifier, constant, string literal, punctuator

&nbsp;&nbsp;&nbsp;&nbsp;preprocessing-token当前未实现。

#### Keywords / 关键字
1. Syntax / 句法:
```text
keyword: one of
    namespace
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;上面的所有token是保留的，不要用作其他含义。

#### Identifiers / 标识符
1. Syntax / 句法:
```text
    identifier:
        identifier-nondigit
        identifier identifier-nondigit
        identifier digit
    identifier-nondigit:
        nondigit
    nondigit: one of
        _abcdefghijklm
        nopqrstuvwxyz
        ABCDEFGHIJKLM
        NOPQRSTUVWXYZ
    digit: one of
        0123456789
```    

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;一个identifier是非数字字符序列。

#### Constants / 常量
1. Syntax / 句法:
```text
    constant:
        integer-constant
        floating-constant
``` 

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;每一个constant都有一个语言默认的类型和它对应的类型的值。



##### Integer constants / 整数常量
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011 6.4.4.1。

##### Floating constants / 浮点常量
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011 6.4.4.2。

#### String literals / 字符串
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.5小节。

#### Punctuators / 标点符号
1. Syntax / 句法:
```text
    punctuator: one of
        : = [ ] { } ;
```
2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.6小节。


#### Import Config File Name / 导入配置文件名字
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.7小节。

#### Comments / 注释
1. Syntax / 句法:
```text
exp:
    //

exp:
    /**/
```
2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;上面单行注释表明了当前的注释只对当行有效。多行类似。

### Expressions / 表达式
&nbsp;&nbsp;&nbsp;&nbsp;一个表达式有几种含义，分别是：一个计算值的操作符和操作数的序列；标明一个对象；或者产生影响。


#### Primary expressions / 主表达式
1. Syntax / 句法:
```text
    primary-expression:
        identifier
        constant
        string-literal
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;一个identifier是一个主表达式，当它标识一个对象的时候。一个constant也是一个主表达式，它的类型由其形式和值决定。同理可知string-literal。


#### Postfix operators / 后缀操作符
1. Syntax / 句法:
```text
    postfix-expression:
        primary-expression
        postfix-expression [ expression ]
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;未实现。


#### Assignment operators / 等号操作符
1. Syntax / 句法:
```text
    assignment-expression:
        postfix-expression
        postfix-expression assignment-operator assignment-expression
    assignment-operator: one of
        =
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;赋值操作符将存储一个值到对象中。

#### Comma operator / 逗号操作符
1. Syntax / 句法:
```text
    expression:
        assignment-expression
        expression , assignment-expression
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;未实现。

### Declarations / 声明
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;一个声明指定一个identifier的属性和值。



#### Namespace specifiers / 命名空间说明符
1. Syntax / 句法:
```text
    namespace-specifier:
        assignment-expression
        expression , assignment-expression
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;未实现。

### Statements and blocks / 句子和块
1. Syntax / 句法:
```text
    statement:
        compound-statement
        expression-statement
```

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;一个句子和块标识着一个将被执行的动作。


#### Compound statement / 复合语句
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;复合语句是一个块。


#### Expression statements / 表达式语句
1. Syntax / 句法:
```text
    expression-statement:
        expression[opt] ;
```
2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;标示一个被执行和计算的语句。


### Preprocessing directives / 预处理指令
1. Syntax / 句法:
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

2. Semantics / 语法
&nbsp;&nbsp;&nbsp;&nbsp;不支持。