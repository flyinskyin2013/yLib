/*
Copyright (c) 2018 - 2025 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __CORE_YCOMPILER_AST_RECURSIVE_AST_VISITOR_HPP__
#define __CORE_YCOMPILER_AST_RECURSIVE_AST_VISITOR_HPP__



#include "core/ycompiler/ast/ydecl_base.hpp"
#include "core/ycompiler/ast/ydecl.hpp"
#include "core/ycompiler/ast/ystmt.hpp"
#include "core/ycompiler/ast/yexpr.hpp"
#include "core/casting.hpp"

namespace yLib
{
namespace ycompiler
{
    /// A class that does preorder or postorder
    /// depth-first traversal on the entire Clang AST and visits each node.
    /// These three method groups are tiered (Traverse* > WalkUpFrom* > Visit*).
    template <typename Derived> 
    class RecursiveASTVisitor {
    public:

        /// Return a reference to the derived class.
        Derived &getDerived() { return *static_cast<Derived *>(this); }

        /// Return whether this visitor should traverse post-order.
        bool shouldTraversePostOrder() const { return false; }

        /// Recursively visit a declaration, by dispatching to
        /// Traverse*Decl() based on the argument's dynamic type.
        ///
        /// \returns false if the visitation was terminated early, true
        /// otherwise (including when the argument is NULL).
        bool TraverseDecl(Decl *D);
        bool TraverseDeclContext(Decl *D);


        // ----------------- Type traversal -----------------
        // A helper macro to implement short-circuiting when recursing.  It
        // invokes CALL_EXPR, which must be a method call, on the derived
        // object (s.t. a user of RecursiveASTVisitor can override the method
        // in CALL_EXPR).
        #define TRY_TO(CALL_EXPR)                                                      \
            do {                                                                         \
                if (!getDerived().CALL_EXPR)                                               \
                return false;                                                            \
            } while (false)


        ///////////////////////////////////////////////////////////
        //For Stmt start
        ///////////////////////////////////////////////////////////
        // Declare Traverse*() for all concrete Stmt classes.
        //#define ABSTRACT_STMT(STMT)
        #define STMT(CLASS, PARENT) \
            bool Traverse##CLASS(CLASS *S);
        #include "stmt_nodes_kinds.def"

        #undef STMT


        // Define WalkUpFrom*() and empty Visit*() for all Stmt classes.
        bool WalkUpFromStmt(Stmt *S) { return getDerived().VisitStmt(S); }
        bool VisitStmt(Stmt *S) { return true; }
        #define STMT(CLASS, PARENT)                                                    \
            bool WalkUpFrom##CLASS(CLASS *S) {                                           \
                TRY_TO(WalkUpFrom##PARENT(S));                                             \
                TRY_TO(Visit##CLASS(S));                                                   \
                return true;                                                               \
            }                                                                            \
            bool Visit##CLASS(CLASS *S) { return true; }
        #include "stmt_nodes_kinds.def"
        
        #undef STMT
        ///////////////////////////////////////////////////////////
        //For Stmt end
        ///////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////
        //For Decl start
        ///////////////////////////////////////////////////////////


        // Declare Traverse*() for all concrete Decl classes.
        #define ABSTRACT_DECL(DECL)
        #define DECL(CLASS, BASE) bool Traverse##CLASS##Decl(CLASS##Decl *D);
        #include "decl_nodes_kinds.def"

        // The above header #undefs ABSTRACT_DECL and DECL upon exit.

        // Define WalkUpFrom*() and empty Visit*() for all Decl classes.
        bool WalkUpFromDecl(Decl *D) { return getDerived().VisitDecl(D); }
        bool VisitDecl(Decl *D) { return true; }
        #define DECL(CLASS, BASE)                                                      \
            bool WalkUpFrom##CLASS##Decl(CLASS##Decl *D) {                               \
                TRY_TO(WalkUpFrom##BASE(D));                                               \
                TRY_TO(Visit##CLASS##Decl(D));                                             \
                return true;                                                               \
            }                                                                            \
            bool Visit##CLASS##Decl(CLASS##Decl *D) { return true; }
        #include "decl_nodes_kinds.def"

        ///////////////////////////////////////////////////////////
        //For Decl end
        ///////////////////////////////////////////////////////////
    };




    template <typename Derived>
    bool RecursiveASTVisitor<Derived>::TraverseDecl(yDecl *D) {
        if (!D)
            return true;

        switch (D->get_decl_kind()) {

            #define ABSTRACT_DECL(DECL)
            #define DECL(CLASS, BASE)                                                      \
                case yDecl::CLASS##Decl:                                                            \
                    if (!getDerived().Traverse##CLASS##Decl(static_cast<CLASS##Decl *>(D)))    \
                    return false;                                                            \
                    break;
            /*
                example:
                case yDecl::TranslationUnitDecl:                                                            \
                        if (!getDerived().TraverseTranslationUnitDecl(static_cast<TranslationUnitDecl *>(D)))    \
                        return false;                                                            \
                        break;
            */
            #include "decl_nodes_kinds.def"
        }
        return true;
    }


    template <typename Derived>
    bool RecursiveASTVisitor<Derived>::TraverseDeclContext(Decl *D) {
        if (!D)
            return true;
        
        //check 'Decl' is derived 'DeclContext'
        DeclContext _tmp;
        // https://en.cppreference.com/w/cpp/language/dynamic_cast for sidecast

        // https://llvm.org/docs/ProgrammersManual.html#the-isa-cast-and-dyn-cast-templates
        // https://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html
        // llvm\include\llvm\Support\Casting.h for dyn_cast

        
        //DeclContext *_decl_ctx = dynamic_cast<DeclContext*>(D);
        //DeclContext *_decl_ctx = llvm::dyn_cast<DeclContext*>(D);
        DeclContext *_decl_ctx = ylib::dyn_cast<DeclContext>(D);

        if (nullptr != _decl_ctx){
            
            for (Decl * _decl : _decl_ctx->get_decl_group().get_decl_vec()){
                TRY_TO(TraverseDecl(_decl));
            }
        }
        return true;
    }


    // This macro makes available a variable T, the passed-in type.
    #define DEF_TRAVERSE_TYPE(TYPE, CODE)                                          \
    template <typename Derived>                                                  \
    bool RecursiveASTVisitor<Derived>::Traverse##TYPE(TYPE *T) {                 \
        if (!getDerived().shouldTraversePostOrder())                               \
        TRY_TO(WalkUpFrom##TYPE(T));                                             \
        { CODE; }                                                                  \
        if (getDerived().shouldTraversePostOrder())                                \
        TRY_TO(WalkUpFrom##TYPE(T));                                             \
        return true;                                                               \
    }

    // This macro makes available a variable D, the passed-in decl.
    #define DEF_TRAVERSE_DECL(DECL, CODE)                                          \
        template <typename Derived>                                                  \
        bool RecursiveASTVisitor<Derived>::Traverse##DECL(DECL *D) {                 \
            bool ShouldVisitChildren = true;                                           \
            bool ReturnValue = true;                                                   \
            if (!getDerived().shouldTraversePostOrder())                               \
                TRY_TO(WalkUpFrom##DECL(D));                                             \
            { CODE; }                                                                  \
            if (ReturnValue && ShouldVisitChildren)                                    \
                TRY_TO(TraverseDeclContext(D));                                           \
            if (ReturnValue && getDerived().shouldTraversePostOrder())                 \
                TRY_TO(WalkUpFrom##DECL(D));                                             \
            return ReturnValue;                                                        \
        } 

    DEF_TRAVERSE_DECL(TranslationUnitDecl, {})
    DEF_TRAVERSE_DECL(RecordDecl, {})
    DEF_TRAVERSE_DECL(ObjectDecl, {})
    DEF_TRAVERSE_DECL(VarDecl, {})
    DEF_TRAVERSE_DECL(FunctionDecl, {})


    // ----------------- Stmt traversal -----------------
    //
    // For stmts, we automate (in the DEF_TRAVERSE_STMT macro) iterating
    // over the children defined in children() (every stmt defines these,
    // though sometimes the range is empty).  Each individual Traverse*
    // method only needs to worry about children other than those.  To see
    // what children() does for a given class, see, e.g.,
    //   http://clang.llvm.org/doxygen/Stmt_8cpp_source.html

    // This macro makes available a variable S, the passed-in stmt.
    #define DEF_TRAVERSE_STMT(STMT, CODE)                                          \
    template <typename Derived>                                                  \
    bool RecursiveASTVisitor<Derived>::Traverse##STMT(STMT *S) {                 \
        return true; \
    }

/*   #define DEF_TRAVERSE_STMT(STMT, CODE)                                          \
    template <typename Derived>                                                  \
    bool RecursiveASTVisitor<Derived>::Traverse##STMT(STMT *S) {                 \
        bool ShouldVisitChildren = true;                                           \
        bool ReturnValue = true;                                                   \
        if (!getDerived().shouldTraversePostOrder())                               \
        TRY_TO(WalkUpFrom##STMT(S));                                             \
        { CODE; }                                                                  \
        if (ShouldVisitChildren) {                                                 \
        for (Stmt * SubStmt : getDerived().getStmtChildren(S)) {                 \
            TRY_TO_TRAVERSE_OR_ENQUEUE_STMT(SubStmt);                              \
        }                                                                        \
        }                                                                          \
         Call WalkUpFrom if TRY_TO_TRAVERSE_OR_ENQUEUE_STMT has traversed the    \
        * children already. If TRY_TO_TRAVERSE_OR_ENQUEUE_STMT only enqueued the  \
        * children, PostVisitStmt will call WalkUpFrom after we are done visiting \
        * children.                                                             \
        if (!Queue && ReturnValue && getDerived().shouldTraversePostOrder()) {     \
        TRY_TO(WalkUpFrom##STMT(S));                                             \
        }                                                                          \
        return ReturnValue;                                                        \
    }
*/ 
    DEF_TRAVERSE_STMT(CompoundStmt, {})
    DEF_TRAVERSE_STMT(DeclStmt, {})
    DEF_TRAVERSE_STMT(ValueStmt, {})
}
}
#endif //__CORE_YCOMPILER_AST_RECURSIVE_AST_VISITOR_HPP__