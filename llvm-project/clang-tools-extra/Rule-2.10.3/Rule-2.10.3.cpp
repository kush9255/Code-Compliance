// include necessary header files
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

// create matcher for typedef declaration and variable declaration
DeclarationMatcher TypedefMatcher = typedefDecl().bind("typedef");
DeclarationMatcher VariableMatcher = varDecl().bind("variable");

// combine the two matchers into one
DeclarationMatcher AnyOfMatcher = anyOf(TypedefMatcher, VariableMatcher);

// create a statement matcher for a statement containing the combined matcher
StatementMatcher DeclMatcher = declStmt(
  has(AnyOfMatcher)).bind("declstmt");

// create vectors to store variable and typedef declarations
vector<string> varDeclarations;
vector<string> typedefDeclarations;

// create a class to handle the matches found by the matchers
class UniqueIdent : public MatchFinder::MatchCallback {
public :
  // override the run method to handle the match results
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // get the declaration statement containing the match
    const DeclStmt *var = Result.Nodes.getNodeAs<DeclStmt>("declstmt");
    // get the diagnostic engine for reporting errors/warnings
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // create a custom diagnostic message for the rule violation
    const unsigned ID = DE.getCustomDiagID(clang::DiagnosticsEngine::Error,
      "MISRA C++ Rule 2.10.3 Violation! A typedef name shall be a unique identifier.");

    if (Result.SourceManager->isInSystemHeader(var->getBeginLoc())) {
      // Declaration is in a system header, ignore it
      return;
    }

    // iterate over all declarations in the statement
    for (const Decl *D : var->decls()) {
      // check if the declaration is a typedef
      if (const TypedefDecl *ED = dyn_cast<TypedefDecl>(D)) {
        // get the name of the typedef
        string typeDefName = ED->getNameAsString();

        // check if the typedef name has already been declared
        for(auto itr: typedefDeclarations) {
          if(typeDefName == itr)
          DE.Report(ED->getLocation(), ID);
        }

        // check if the typedef name conflicts with a variable name
        for(auto itr: varDeclarations) {
          if(typeDefName == itr)
          DE.Report(ED->getLocation(), ID);
        }

        // add the typedef name to the list of declarations
        typedefDeclarations.push_back(typeDefName);
      }
    
      // check if the declaration is a variable
      else if(const VarDecl * VD = dyn_cast<VarDecl>(D)) {
        // get the name of the variable
        string varName = VD->getNameAsString();
        // get the location of the variable
        SourceLocation loc = VD->getLocation();

        // check if the variable name conflicts with a typedef name
        for(auto itr: typedefDeclarations) {
          if(varName == itr)
          DE.Report(loc, ID);
        }
        // add the variable name to the list of declarations
        varDeclarations.push_back(varName);
      }
    }
  }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  UniqueIdent ident;
  MatchFinder Finder;
  Finder.addMatcher(DeclMatcher, &ident);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}

                                        // DOCUMENTATION //
/*
This is a C++ code that uses the Clang tooling library to identify violations of a specific rule in C++ code. 
The rule that this code checks for is "MISRA C++ Rule 2.10.3 Violation! A typedef name shall be a unique identifier."

The code starts by including the necessary headers from the Clang tooling library and the LLVM support library. 
It then defines two declaration matchers: TypedefMatcher and VariableMatcher, which match on typedef and var declarations, respectively. 
It then defines an AnyOfMatcher that matches any declaration that is either a typedef or a var.

Next, the code defines a StatementMatcher named DeclMatcher that matches any declaration statement that contains a declaration that matches the AnyOfMatcher.

The code then defines a class called UniqueIdent that inherits from MatchFinder::MatchCallback. 
This class defines a run method that is called by the MatchFinder when it finds a match for the DeclMatcher. 
In this method, the code checks whether the matched declaration is a typedef or a var. 
If it is a typedef, the code checks whether the name of the typedef is unique by comparing it with all the previous typedef and var declarations. 
If it is not unique,the code reports an error using Clang's diagnostics engine.

The main function of the code sets up the Clang tool by creating a CommonOptionsParser object, which parses the command-line options, and a ClangTool object, which represents the compilation process. 
It then creates a MatchFinder object and adds the DeclMatcher and UniqueIdent objects to it. 
Finally, it runs the tool by calling the run method on the ClangTool object and passing it a factory for the frontend action that will run the MatchFinder.

Overall, this code uses the Clang tooling library to identify violations of a specific coding rule in C++ code and report them using Clang's diagnostics engine.
*/