// Include necessary header files
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <vector>
#include "clang/AST/ASTContext.h"

// Use these namespaces to simplify code
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

// Create a matcher to match var decl nodes for unsigned integers with negation
const auto unsignedVarDeclMatcher = varDecl(
  hasType(isUnsignedInteger()),
  hasInitializer(ignoringParenImpCasts(unaryOperator(hasOperatorName("-"))))
).bind("unsignedVarDecl");

// Create a callback class for the match found by the matcher
class UnsignedVarDeclPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched var decl node
    const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("unsignedVarDecl");
    // Get the location of the var decl in the source code
    SourceLocation loc = varDecl->getBeginLoc();
    // Get the full location of the var decl in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the unsigned variable with negation violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.3.3 Violation! The unary minus operator shall not be applied to an operand whose underlying type is unsigned.");

    // Report the unsigned variable with negation violation
    DE.Report(loc, ID);
  }
};

// Create an option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// Main function
int main(int argc, const char **argv) {
  // Create a CommonOptionsParser object to parse command line arguments
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  UnsignedVarDeclPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(unsignedVarDeclMatcher, &Printer);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
                                          //DOCUMENTATION
/*
This code is a C++ program that uses the Clang tooling library to analyze C++ source code for violations of the MISRA C++ Rule 5.3.2. The rule specifies that the unary minus operator shall not be applied to an operand whose underlying type is unsigned.

The program defines a matcher that matches var decl nodes for unsigned integers with negation. It uses the Clang AST Matchers library to define this matcher. The matcher is named "unsignedVarDeclMatcher" and it binds to the node as "unsignedVarDecl".

The program also defines a callback class named "UnsignedVarDeclPrinter". The callback class extends the "MatchFinder::MatchCallback" class and overrides the virtual "run" function to process the match result. The "run" function gets the matched var decl node, gets the location of the var decl in the source code, gets the full location of the var decl in the source code, and gets the diagnostics engine to report errors. It then reports the unsigned variable with negation violation by using a custom error ID.

The main function of the program creates a CommonOptionsParser object to parse command line arguments, creates an option category for the tool, and creates a ClangTool object. It also creates a "UnsignedVarDeclPrinter" object, a "MatchFinder" object, adds the "unsignedVarDeclMatcher" matcher to the "MatchFinder" object, and runs the tool with the "UnsignedVarDeclPrinter" object.

Overall, this program is a useful tool for analyzing C++ source code for violations of the MISRA C++ Rule 5.3.2.
*/