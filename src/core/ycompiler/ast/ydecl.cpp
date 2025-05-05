#include "core/ycompiler/ast/ydecl.hpp"
YLIB_NAMESPACE_START
namespace ycompiler{

TranslationUnitDecl::TranslationUnitDecl(yASTContext &ctx)
:ctx(ctx)
{}

TranslationUnitDecl* TranslationUnitDecl::Create(yASTContext &ctx)
{
    return new (std::nothrow) TranslationUnitDecl(ctx);
}

}
YLIB_NAMESPACE_END