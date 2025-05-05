#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclCXX.h>
#include <clang/Basic/SourceManager.h>
#include <iostream>
#include <llvm/Support/Casting.h>
#include <string>

using namespace clang;

bool findFieldInHierarchy(clang::CXXRecordDecl *currentDecl,
                          const std::string &name) {
    // 正确：遍历所有成员并筛选静态变量
    for (auto *Member : currentDecl->decls()) {
        if (auto *Field = dyn_cast<clang::VarDecl>(Member)) {

            if (Field->isStaticDataMember()) {

                if (Field->getNameAsString() == name) {
                    // std::cout << "Found in " << currentDecl->getNameAsString() << ": "
                    //             << Field->getNameAsString() << std::endl;
                    return true;
                }

            }
        }
    }
    for (clang::FieldDecl *field : currentDecl->fields()) {
        
        if (field->getNameAsString() == name) {
            // std::cout << "Found in " << currentDecl->getNameAsString() << ": "
            //             << field->getNameAsString() << std::endl;
            return true;
        }
    }
    return false;
    // 递归遍历基类
    // for (auto &base : currentDecl->bases()) {
    //     clang::CXXRecordDecl *baseDecl = base.getType()->getAsCXXRecordDecl();
    //     if (baseDecl) {
    //     findFieldInHierarchy(baseDecl, name);
    //     }
    // }
    return true;
}

void processNamespaceDecl(clang::NamespaceDecl *recordDecl, std::string & namespace_str) {
  // 获取声明上下文
  clang::DeclContext *context = recordDecl->getDeclContext();

  // 遍历直到找到命名空间或翻译单元
  while (context && !llvm::isa<clang::TranslationUnitDecl>(context)) {
        clang::NamespaceDecl *namespaceDecl = dyn_cast<clang::NamespaceDecl>(context);

        if (nullptr != namespaceDecl) {

            namespace_str = "::" + namespaceDecl->getNameAsString() + namespace_str;
            processNamespaceDecl(namespaceDecl, namespace_str);
            return;
        }
      
    
        context = context->getParent();
    }
    return ;
}

void processCXXRecordDecl(clang::CXXRecordDecl *recordDecl, std::string & namespace_str) {
  // 获取声明上下文
  clang::DeclContext *context = recordDecl->getDeclContext();

  // 遍历直到找到命名空间或翻译单元
  while (context && !llvm::isa<clang::TranslationUnitDecl>(context)) {

    clang::NamespaceDecl *namespaceDecl = dyn_cast<clang::NamespaceDecl>(context);

    if (nullptr != namespaceDecl) {
    //   std::cout << "Class " << recordDecl->getNameAsString()
    //             << " is in namespace " << namespaceDecl->getNameAsString()
    //             << "\n";

        namespace_str = "::" + namespaceDecl->getNameAsString();
        processNamespaceDecl(namespaceDecl, namespace_str);
        return ;
    }
    context = context->getParent();
  }

  return;
}

std::string basename(const std::string& path) {
    // 找到最后一个 '/' 的位置
    size_t pos = path.find_last_of("/\\");  // 兼容 Unix 和 Windows 路径分隔符
    if (pos != std::string::npos) {
        return path.substr(pos + 1);  // 返回最后一个 '/' 后的部分
    }
    return path;  // 如果没有 '/'，返回整个路径
}

class ClassASTVisitor : public clang::RecursiveASTVisitor<ClassASTVisitor> {
public:
  bool VisitCXXRecordDecl(clang::CXXRecordDecl *Decl) {
    clang::SourceLocation loc = Decl->getLocation();
    clang::SourceManager &SM = Decl->getASTContext().getSourceManager();
    std::string file_name = SM.getFilename(loc).str();

    if (!Decl->isCompleteDefinition())
        return true;

    if (std::string::npos != file_name.find("yLib/include") || std::string::npos != file_name.find("yLib/src")){
        
        std::string  type_key = "";

        processCXXRecordDecl(Decl, type_key);
        
        type_key = type_key + "::" + Decl->getName().str();
        

        std::cout << "{"<<std::endl;
        printf("\"file_name\": \"%s\",\n", file_name.c_str());
        printf("\"class_name\": \"%s\",\n", Decl->getName().str().c_str());
        printf("\"class_type_key_found\": %s,\n", findFieldInHierarchy(Decl, "type_key_")?"true":"false");
        printf("\"class_type_key\": \"%s\",\n", type_key.c_str());
        printf("\"class_base_class_num\": %d,\n", Decl->getNumBases());

        // std::cout << "File=" << file_name.c_str() << std::endl;
        // std::cout << "class name=" << Decl->getName().str() << std::endl;
        // std::cout << "class type-key is found=" << (findFieldInHierarchy(Decl, "type_key_")?1:0) <<std::endl;
        // std::cout << "class type-key=" << type_key <<std::endl;
        // std::cout << "class base class num=" << Decl->getNumBases() <<std::endl;

        
        std::string _base_class_name_list;
        for (auto _iter : Decl->bases()) {
            clang::QualType BaseType = _iter.getType(); // 获取基类的类型
            clang::CXXRecordDecl *BaseDecl = BaseType->getAsCXXRecordDecl(); // 获取基类的声明
            // std::cout << "  class base class name=" << BaseDecl->getName().str() <<std::endl;
            _base_class_name_list += "\"" + BaseDecl->getName().str() + "\", ";
        }
        printf("\"class_base_class_name\": [ %s ]\n", _base_class_name_list.substr(0, _base_class_name_list.length() - 2).c_str());
        std::cout << "},"<<std::endl;
        

    }

    

    return true;
  }
};


class ClassASTConsumer : public ASTConsumer {
private:
    ClassASTVisitor Visitor;
public:
    ClassASTConsumer() {}

    // 处理顶层声明（如函数、类等）
    bool HandleTopLevelDecl(DeclGroupRef DG) override {
        for (Decl *D : DG) {
            Visitor.TraverseDecl(D);  // 触发 Visitor 遍历
        }
        return true;
    }
};

#include <clang/Frontend/FrontendAction.h>

class ClassFrontendAction : public ASTFrontendAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                  StringRef file) override {
        return std::make_unique<ClassASTConsumer>();
    }
};

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

std::vector<std::string> get_all_file_path(const std::string& filepath)
{
    std::vector<std::string> file_list;

    std::ifstream file(filepath);
    if (!file.is_open())
        return {};
    
    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        if (line.empty())
            continue;

        file_list.push_back(line);
        /* code */
    }

    return file_list;
    
}

int main(int argc, const char **argv) {
    // for (int _i = 0; _i < argc; _i++)
    //     printf("arg[%d] = %s\n", _i, argv[_i]);
    // 解析命令行参数
    llvm::cl::OptionCategory MyToolCategory("MyTool");
    llvm::Expected<clang::tooling::CommonOptionsParser> Op = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);
    clang::tooling::ClangTool Tool(Op->getCompilations(), Op->getSourcePathList());
    for (auto & path:Op->getSourcePathList()){        
        fs::path _p = path;
        std::string _base_file_name = _p.filename().string();
        
        // std::cout << "input-file-base-name: " << basename(_base_file_name)  << std::endl;
    }
    // 运行自定义的 FrontendAction
    return Tool.run(clang::tooling::newFrontendActionFactory<ClassFrontendAction>().get());
}