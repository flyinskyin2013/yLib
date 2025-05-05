#include "core/yobject.hpp"
#include "core/yexception.hpp"
#include "core/casting.hpp"

#include "core/ycompiler/ast/ydecl.hpp"
#include "core/ycompiler/ast/yast_context.hpp"


#include <memory>

int main(int argc, const char * argv[])
{
    // object type-key test
    yLib::Object obj;
    printf("yObject's type-key = %s\n", obj.type_key().c_str());
    
    yLib::ycompiler::yDecl dec;
    yLib::Object* dec_obj = &dec;
    printf("yDecl's type-key = %s\n", dec.type_key().c_str());
    printf("yDecl's yObject-Ponter type-key = %s\n", dec_obj->type_key().c_str());

    yLib::yException exp;
    yLib::Object* exp_obj = &exp;
    printf("yException's type-key = %s\n", exp.type_key().c_str());
    printf("yException's yObject-Ponter type-key = %s\n", exp_obj->type_key().c_str());


    printf("yDecl is base-class of yObject = %s\n", yLib::Object::classof(dec.type_index(), obj.type_index())?"true":"false");
    printf("yObject is base-class of yDecl = %s\n", yLib::Object::classof(obj.type_index(), dec.type_index())?"true":"false");

    printf("yException is base-class of yObject = %s\n", yLib::Object::classof(exp.type_index(), obj.type_index())?"true":"false");
    printf("yObject is base-class of yException = %s\n", yLib::Object::classof(obj.type_index(), exp.type_index())?"true":"false");
    

    //obj cast test
    yLib::ycompiler::yASTContext ctx;
    std::unique_ptr<yLib::ycompiler::TranslationUnitDecl> tu(yLib::ycompiler::TranslationUnitDecl::Create(ctx));

    yLib::ycompiler::yDecl* decl = tu.get();

    yLib::ycompiler::yDeclContext* decl_ctx = yLib::dyn_cast<yLib::ycompiler::yDeclContext>(decl);

    printf("TranslationUnitDecl type-key = %s\n", tu->type_key().c_str());
    printf("TranslationUnitDecl castto yDecl type-key = %s\n", decl->type_key().c_str());
    printf("yDeclContext is base-class of TranslationUnitDecl = %s\n", yLib::Object::classof(yLib::yObject::TypeKey2Index(yLib::ycompiler::yDeclContext::static_type_key()), yLib::yObject::TypeKey2Index(yLib::ycompiler::TranslationUnitDecl::static_type_key()))?"true":"false");
    printf("TranslationUnitDecl castto yDeclContext type-key = %s\n", decl_ctx->type_key().c_str());
    return 0;
}