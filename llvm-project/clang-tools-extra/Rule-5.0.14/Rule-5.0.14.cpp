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

// Create a matcher to match the ternary conditional operator with the first operand as bool
StatementMatcher BoolTernaryMatcher = conditionalOperator(
    hasCondition(ignoringParenImpCasts(declRefExpr(
        to(varDecl(unless(hasType(booleanType()))))).bind("nonBoolVar"))),
    unless(hasFalseExpression(ignoringParenImpCasts(declRefExpr(
        to(varDecl(hasType(booleanType())))).bind("otherBoolVar"))))
).bind("ternaryStmt");

// Create a callback class for the match found by the matcher
class BoolTernaryPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched ternary statement node
    const ConditionalOperator *ternaryStmt = Result.Nodes.getNodeAs<ConditionalOperator>("ternaryStmt");
    // Get the location of the ternary statement in the source code
    SourceLocation loc = ternaryStmt->getBeginLoc();
    // Get the full location of the ternary statement in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.0.14 Violation! The first operand of a conditional-operator shall have type bool.");

    // Report the violation
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

  BoolTernaryPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(BoolTernaryMatcher, &Printer);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
                                      //DOCUMENTATION
/*
This is a C++ tool that uses the Clang library to detect violations of the MISRA C++ Rule 5.0.14, which states that the first operand of a conditional operator (ternary operator) shall have type bool.

The tool works by creating a Clang AST matcher to find conditional operators with a non-bool first operand. When a match is found, the tool reports a custom diagnostic error with a message describing the violation.

The matcher in this code is defined as BoolTernaryMatcher and it searches for a ternary conditional operator whose first operand is not of boolean type but some other type. The matcher is constructed using a chain of matcher functions provided by the AST Matchers library, such as conditionalOperator, hasCondition, ignoringParenImpCasts, declRefExpr, to, varDecl, unless, and bind.

Once the matcher finds a match, a callback function is called to process the match result. In this code, the BoolTernaryPrinter class is used as the callback function, and it reports a custom diagnostic message using the Clang Diagnostics Engine if a match is found.

The clang/AST/ASTContext.h header file is included to provide access to the AST context used in the callback function.

The code also uses the llvm/Support/CommandLine.h header file to define an option category for the tool and parse command-line arguments using the CommonOptionsParser class provided by the Clang Tooling library.

Finally, the ClangTool class is used to run the tool with the given AST matcher and frontend action factory.





*/