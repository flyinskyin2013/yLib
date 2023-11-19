# yConfig Language / yConfig语言
## 1 Introduction / 引言
&nbsp;&nbsp;&nbsp;&nbsp;本文件主要参考IOS/IEC-9899:2011第6章节（Language）和 ISO/IEC-14882:2011 第2章节 (Lexical conventions).

## 2 Language / 语言
### 2.1 Notation / 注释
&nbsp;&nbsp;&nbsp;&nbsp;无。

### 2.2 Concepts / 概念
#### 2.2.1 Scopes of identifiers / 标识符的作用域
&nbsp;&nbsp;&nbsp;&nbsp;标识符表示一个变量名或者对象。每个标识符在其作用域中可见。相同标识符表示的实例一定位于不同的作用域中。这里有两种作用域：文件和块。

#### 2.2.2 Name spaces of identifiers / 标识符的命名空间
&nbsp;&nbsp;&nbsp;&nbsp;标识符在同一个翻译单元同层级可见。在同一个对象同层级中可见。标识符在同一个命名空间里面是唯一的。

#### 2.2.3 Types / 类型
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


### 2.3 Lexical elements / 词法要素
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

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;一个token是最小的词法单元。其共有5种类别：keywords, identifier, constant, string literal, punctuator

&nbsp;&nbsp;&nbsp;&nbsp;preprocessing-token当前未实现。

#### 2.3.1 Keywords / 关键字
1. Syntax / 句法:
```text
keyword: one of
    namespace
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;上面的所有token是保留的，不要用作其他含义。

#### 2.3.2 Identifiers / 标识符
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

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;一个identifier是非数字字符开始的序列。

#### 2.3.3 Constants / 常量
1. Syntax / 句法:
```text
    constant:
        integer-constant
        floating-constant
``` 

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;每一个constant都有一个语言默认的类型和它对应的类型的值。



##### 2.3.3.1 Integer constants / 整数常量
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

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011 6.4.4.1。

##### 2.3.3.2 Floating constants / 浮点常量
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

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011 6.4.4.2。

#### 2.3.4 Boolean,String literals / 布尔字面量,字符串字面量
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

```text
    boolean-literal:
        false
        true
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.5小节与ISO/IEC-14882:2011 第2.14.6小节。

#### 2.3.5 Punctuators / 标点符号
1. Syntax / 句法:
```text
    punctuator: one of
        : = [ ] { } ;
```
2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.6小节。


#### 2.3.6 Import Config File Name / 导入配置文件名字
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

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;参考IOS/IEC-9899:2011的6.4.7小节。

#### 2.3.7 Comments / 注释
1. Syntax / 句法:
```text
exp:
    //

exp:
    /**/
```
2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;上面单行注释表明了当前的注释只对当行有效。多行类似。

### 2.4 Expressions / 表达式
&nbsp;&nbsp;&nbsp;&nbsp;一个表达式有几种含义，分别是：一个计算值的操作符和操作数的序列；标明一个对象；或者产生影响。


#### 2.4.1 Primary expressions / 主表达式
1. Syntax / 句法:
```text
    primary-expression:
        identifier
        constant
        string-literal
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;一个identifier是一个主表达式，当它标识一个对象的时候。一个constant也是一个主表达式，它的类型由其形式和值决定。同理可知string-literal。


#### 2.4.2 Postfix operators / 后缀操作符
1. Syntax / 句法:
```text
    postfix-expression:
        primary-expression
        postfix-expression [ expression ]
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;未实现。


#### 2.4.3 Assignment operators / 等号操作符
1. Syntax / 句法:
```text
    assignment-expression:
        postfix-expression
        postfix-expression assignment-operator assignment-expression
    assignment-operator: one of
        =
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;赋值操作符将存储一个值到对象中。

#### 2.4.4 Comma operator / 逗号操作符
1. Syntax / 句法:
```text
    expression:
        assignment-expression
        expression , assignment-expression
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;未实现。

### 2.5 Declarations / 声明
1. Syntax / 句法:
```text
    declaration:
        declaration-specifiers init-declarator-list[opt] ;
    declaration-specifiers:
        namespace-specifier
    init-declarator-list:
        init-declarator
        init-declarator-list , init-declarator
    init-declarator:
        declarator
        declarator = initializer
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;一个声明指定一个identifier的属性和值。



#### 2.5.1 Namespace specifiers / 命名空间说明符
1. Syntax / 句法:
```text
    namespace-specifier:
        assignment-expression
        expression , assignment-expression
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;未实现。

#### 2.5.2  Declarators / 说明符
1. Syntax / 句法:
```text
    declarator:
        pointer[opt] direct-declarator
    direct-declarator:
        identifier
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;无。


#### 2.5.3  Initialization / 初始化
1. Syntax / 句法:
```text
    initializer:
        assignment-expression
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;无。



### 2.6 Statements and blocks / 句子和块
1. Syntax / 句法:
```text
    statement:
        compound-statement
        expression-statement
```

2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;一个句子和块标识着一个将被执行的动作。


#### 2.6.1 Compound statement / 复合语句
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

2. Semantics / 语义
&nbsp;&nbsp;&nbsp;&nbsp;复合语句是一个块。


#### 2.6.2 Expression statements / 表达式语句
1. Syntax / 句法:
```text
    expression-statement:
        expression[opt] ;
```
2. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;标示一个被执行和计算的语句。


### 2.7 External definitions / 外部定义
1. Syntax / 句法:
```text
    translation-unit:
        external-declaration
        translation-unit external-declaration

    external-declaration:
        object-definition
```
2. Constraints / 约束

&nbsp;&nbsp;&nbsp;&nbsp;在一个翻译单元中，所有的声明标识符必须是唯一的。若存在import的预处理标识，则应该先预处理，然后再进行翻译单元。

3. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;在本文的定义中，一个经过预处理之后的程序文本作为一个翻译单元。

### 2.7.1 Object definitions / 对象定义
1. Syntax / 句法:
```text
    object-definition:
        declarator : compound-statement
```
2. Constraints / 约束

&nbsp;&nbsp;&nbsp;&nbsp;无。

3. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;标识一个对象的定义，一个config文件中有0或者多个对象定义，这些对象也可以import（未实现）来自于其他文件。

### 2.8 Preprocessing directives / 预处理指令
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
2. Constraints / 约束

&nbsp;&nbsp;&nbsp;&nbsp;无。

3. Semantics / 语义

&nbsp;&nbsp;&nbsp;&nbsp;不支持。