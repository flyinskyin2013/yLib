### template <typename Derived> class RecursiveASTVisitor  核心讲解

该类执行以下三个不同的任务：
  1. 遍历 AST（即访问每个节点）；
  2. 在给定节点处，从节点的动态类型开始，沿类层次结构向上遍历，直到达到顶层类（例如 Stmt、Decl 或 Type）；
  3. 给定一个 (节点, 类) 组合，其中“类”是“节点”的动态类型的某个基类，调用一个用户可重写的函数来实际访问该节点。

这三个任务分别由以下三组方法完成：
  1. TraverseDecl(Decl *x) 完成任务 #1。它是以 x 为根节点遍历 AST 的入口点。
     该方法简单地分派（即转发）到 TraverseFoo(Foo *x)，其中 Foo 是 *x 的动态类型，
     然后调用 WalkUpFromFoo(x)，并递归访问 x 的子节点。
     TraverseStmt(Stmt *x) 和 TraverseType(QualType x) 的工作方式类似。
  2. WalkUpFromFoo(Foo *x) 完成任务 #2。它不会尝试访问任何 x 的子节点。
     相反，它会首先调用 WalkUpFromBar(x)，其中 Bar 是 Foo 的直接父类（除非 Foo 没有父类），
     然后调用 VisitFoo(x)（见下一个列表项）。
  3. VisitFoo(Foo *x) 完成任务 #3。

这三组方法是分层的（Traverse* > WalkUpFrom* > Visit*）。一个方法（例如 Traverse*）
可以调用同一层的方法（例如其他 Traverse*）或更低一层的方法（例如 WalkUpFrom*）。
它不能调用更高层的方法。

请注意，由于 WalkUpFromFoo() 在调用 VisitFoo() 之前会调用 WalkUpFromBar()（其中 Bar 是 Foo 的超类），
结果是对于一个节点的 Visit*() 方法按照自顶向下的顺序调用（例如对于类型为 NamespaceDecl 的节点，
调用顺序将是 VisitDecl()、VisitNamedDecl()，然后是 VisitNamespaceDecl()）。

这种机制保证了同一 AST 节点的所有 Visit*() 调用是分组在一起的。换句话说，不同节点的 Visit*() 方法不会交错调用。

使用该访问器的客户端应继承该访问器（提供自身作为模板参数，使用 curiously recurring template pattern，奇异递归模板模式），
并重写任何 Traverse*、WalkUpFrom* 和 Visit* 方法，以便对声明、类型、语句、表达式或其他 AST 节点进行自定义行为。
大多数用户只需要重写 Visit* 方法。高级用户可以重写 Traverse* 和 WalkUpFrom* 方法以实现自定义遍历策略。
从这些重写的函数之一返回 false 将中止整个遍历。

默认情况下，该访问器试图只访问显式源代码的每个部分一次。
对模板的默认策略是深入到“模式”类或函数体，而不是任何显式或隐式实例化。
显式特化仍然会被访问，而部分特化的模式会被单独访问。
通过在派生类中重写 shouldVisitTemplateInstantiations() 并返回 true，可以更改此行为，
此时将同时访问所有已知的隐式和显式实例化以及它们生成的模式。

默认情况下，该访问器以前序方式遍历 AST。如果需要后序遍历，则需要重写 \c shouldTraversePostOrder 方法并返回 \c true。