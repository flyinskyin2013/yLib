# Decl/Stmt/Expr在AST中的核心逻辑总结

## 1. 基本定义与分类
| 类型    | 定义       | 子类示例       | 
|---------|--------|-------------|
| **Decl** | 表示程序中的**命名实体定义**（变量、函数、类等）                     | `VarDecl`（变量声明）、`FuncDecl`（函数声明）、`ClassDecl`（类声明）       |       
| **Stmt** | 表示**控制流或操作**（如条件语句、循环、语句块）                     | `IfStmt`（条件语句）、`ForStmt`（循环）、`CompoundStmt`（语句块）           |       
| **Expr** | 表示**计算或值**（如字面量、运算、函数调用）                         | `BinaryOperator`（二元运算）、`Literal`（字面量）、`CallExpr`（函数调用）   |        

---

## 2. 逻辑关系
### (1) **Decl作为顶层结构**
- **顶层节点**：AST的根通常是`TranslationUnitDecl`（如C/C++的文件级声明）。
- **嵌套关系**：
  - 函数声明（`FuncDecl`）的**函数体**是`Stmt`（如`CompoundStmt`）。
  - 变量声明的**初始化表达式**是`Expr`（如`BinaryOperator`）。

### (2) **Stmt控制程序流程**
- **嵌套规则**：
  - 函数体（`FuncDecl.Body`）是一个`Stmt`（如语句块`CompoundStmt`）。
  - 条件语句（`IfStmt`）的条件是`Expr`，分支是`Stmt`。
- **表达式语句**：如赋值语句包裹在`ExprStmt`（Stmt）中。

### (3) **Expr作为计算单元**
- **嵌套规则**：
  - 嵌套在`Stmt`或`Decl`中（如函数返回值、变量初始化）。
  - 复杂表达式由多个`Expr`组成（如`(a + b) * c`）。
- **不直接包含Stmt/Decl**：仅表示值或计算。

---
