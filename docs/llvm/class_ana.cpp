#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace fs = std::filesystem;

/**
 * @brief Searches for a field (member variable) within a class hierarchy.
 *
 * This function checks for both static and non-static member variables
 * with the given name within the specified C++ class declaration.
 * It does not currently recurse into base classes, as the primary analysis
 * is done on a per-class basis.
 *
 * @param currentDecl The Clang declaration of the class to inspect.
 * @param name The name of the field to search for.
 * @return True if the field is found, false otherwise.
 */
bool findFieldInHierarchy(const clang::CXXRecordDecl *currentDecl,
                          const std::string &name) {
  // Check for null pointer
  if (!currentDecl) {
    return false;
  }

  // Check for static member variables
  for (const auto *Member : currentDecl->decls()) {
    if (!Member) continue;
    if (const auto *Field = llvm::dyn_cast<clang::VarDecl>(Member)) {
      if (Field->isStaticDataMember() && Field->getNameAsString() == name) {
        return true;
      }
    }
  }
  
  // Check for non-static member variables (fields)
  for (const auto *field : currentDecl->fields()) {
    if (!field) continue;
    if (field->getNameAsString() == name) {
      return true;
    }
  }
  
  return false;
}

/**
 * @brief Escapes JSON string values to prevent malformed JSON output.
 *
 * @param str The string to escape.
 * @return The escaped string.
 */
std::string escapeJsonString(const std::string& str) {
  std::string escaped;
  for (char c : str) {
    switch (c) {
      case '"':  escaped += "\\\""; break;
      case '\\': escaped += "\\\\"; break;
      case '\b': escaped += "\\b";  break;
      case '\f': escaped += "\\f";  break;
      case '\n': escaped += "\\n";  break;
      case '\r': escaped += "\\r";  break;
      case '\t': escaped += "\\t";  break;
      default:
        if (c >= 0 && c < 0x20) {
          // Format character as \u00xx
          char buf[7];
          snprintf(buf, sizeof(buf), "\\u00%02x", c);
          escaped += buf;
        } else {
          escaped += c;
        }
    }
  }
  return escaped;
}

/**
 * @brief An AST visitor that collects information about C++ classes.
 *
 * This visitor traverses the Abstract Syntax Tree (AST) of a C++ source file
 * and extracts metadata for each class definition found within the yLib
 * project directories ("yLib/include" or "yLib/src").
 * The collected data is printed to stdout in a JSON-like format.
 */
class ClassASTVisitor : public clang::RecursiveASTVisitor<ClassASTVisitor> {
public:
  explicit ClassASTVisitor(clang::ASTContext &Context) : Context(Context) {}

  bool VisitCXXRecordDecl(clang::CXXRecordDecl *Decl) {
    // Only process class definitions, not forward declarations.
    if (!Decl || !Decl->isCompleteDefinition()) {
      return true;
    }

    clang::SourceManager &SM = Context.getSourceManager();
    clang::SourceLocation loc = Decl->getLocation();
    
    // Check for valid source location
    if (loc.isInvalid()) {
      return true;
    }
    
    std::string file_name = SM.getFilename(loc).str();

    // Filter to process only files within the yLib project.
    if (file_name.find("yLib/include") != std::string::npos ||
        file_name.find("yLib/src") != std::string::npos) {

      // Use getQualifiedNameAsString to get the full namespace-qualified name.
      std::string qualified_name = Decl->getQualifiedNameAsString();
      std::string class_name = Decl->getNameAsString();

      // Escape strings for JSON output
      std::string escaped_file_name = escapeJsonString(file_name);
      std::string escaped_class_name = escapeJsonString(class_name);
      std::string escaped_qualified_name = escapeJsonString(qualified_name);

      // Output the class information in a structured format.
      std::cout << "{" << std::endl;
      std::cout << "  \"file_name\": \"" << escaped_file_name << "\"," << std::endl;
      std::cout << "  \"class_name\": \"" << escaped_class_name << "\"," << std::endl;
      std::cout << "  \"class_type_key_found\": "
                << (findFieldInHierarchy(Decl, "type_key_") ? "true" : "false")
                << "," << std::endl;
      std::cout << "  \"class_type_key\": \"::" << escaped_qualified_name << "\"," << std::endl;
      std::cout << "  \"class_base_class_num\": " << Decl->getNumBases() << "," << std::endl;

      std::cout << "  \"class_base_class_name\": [";
      bool first = true;
      for (const auto &base : Decl->bases()) {
        if (!first) {
          std::cout << ", ";
        }
        clang::CXXRecordDecl *BaseDecl = nullptr;
        // Add null pointer check
        if (!base.getType().isNull()) {
          BaseDecl = base.getType()->getAsCXXRecordDecl();
        }
        if (BaseDecl) {
          std::string base_name = BaseDecl->getNameAsString();
          std::cout << "\"" << escapeJsonString(base_name) << "\"";
        } else {
          std::cout << "\"\"";
        }
        first = false;
      }
      std::cout << "]" << std::endl;
      std::cout << "}," << std::endl;
    }

    return true;
  }

private:
  clang::ASTContext &Context;
};

/**
 * @brief An AST consumer that uses the ClassASTVisitor.
 *
 * This consumer is responsible for creating the visitor and passing
 * the top-level declarations of the source file to it for traversal.
 */
class ClassASTConsumer : public clang::ASTConsumer {
public:
  explicit ClassASTConsumer(clang::ASTContext &Context) : Visitor(Context) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  ClassASTVisitor Visitor;
};

/**
 * @brief A frontend action for creating the AST consumer.
 *
 * This class provides the entry point for the Clang tool to create
 * our custom AST consumer.
 */
class ClassFrontendAction : public clang::ASTFrontendAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef file) override {
    return std::make_unique<ClassASTConsumer>(CI.getASTContext());
  }
};

int main(int argc, const char **argv) {
  // Create an OptionCategory as a named variable to avoid temporary object issue
  static llvm::cl::OptionCategory MyToolCategory("class-ana-tool");

  // Use llvm::Expected for modern error handling with CommonOptionsParser.
  auto ExpectedParser = clang::tooling::CommonOptionsParser::create(
      argc, argv, MyToolCategory);
  
  if (!ExpectedParser) {
    llvm::errs() << "Error creating OptionsParser: " << ExpectedParser.takeError();
    return 1;
  }

  clang::tooling::CommonOptionsParser &OptionsParser = *ExpectedParser;
  clang::tooling::ClangTool Tool(OptionsParser.getCompilations(),
                                 OptionsParser.getSourcePathList());

  // Run the tool with our custom frontend action.
  int result = Tool.run(
      clang::tooling::newFrontendActionFactory<ClassFrontendAction>().get());
  
  return result;
}