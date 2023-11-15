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

// Create a matcher to match int to bool casts
StatementMatcher intToBooleanMatcher =
    castExpr(hasCastKind(CK_IntegralToBoolean),
             hasParent(expr(anyOf(binaryOperator(hasAnyOperatorName("||", "&&","<","<=",">",">=","==","!=")), unaryOperator(hasOperatorName("!"))))))
        .bind("intToBoolean");

// Create a callback class for the match found by the matcher
class IntToBoolPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched cast expression node
    const CastExpr *castExpr = Result.Nodes.getNodeAs<CastExpr>("intToBoolean");
    // Get the location of the cast in the source code
    SourceLocation loc = castExpr->getBeginLoc();
    // Get the full location of the cast in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the cast from int to bool violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.3.1 Violation! Each operand of the ! operator, the logical && or the logical || operators shall have type bool.");

    // Report the cast from int to bool violation
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
  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  IntToBoolPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(intToBooleanMatcher, &Printer);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
                                              //DOCUMENTATION
/*
This is a C++ program that uses the Clang AST Matchers library to find violations of MISRA C++ Rule 5.3.1, which states that "Each operand of the ! operator, the logical && or the logical || operators shall have type bool."

The program starts by including necessary header files from Clang and LLVM. The namespaces for Clang, AST Matchers, Tooling, and LLVM are used to simplify the code.

A matcher is defined to match int to bool casts. Specifically, it matches a CastExpr node that has a cast kind of CK_IntegralToBoolean and a parent node that is an expression that is either a binary operator with the operators ||, &&, <, <=, >, >=, ==, or != or a unary operator with the operator !. The matcher binds the matched node to the identifier "intToBoolean".

A callback class, IntToBoolPrinter, is defined to process the match result. The class inherits from MatchFinder::MatchCallback and overrides the virtual function run. The run function gets the matched CastExpr node, gets the location of the cast in the source code, gets the full location of the cast in the source code, gets the diagnostics engine to report errors, and gets a custom error ID for the cast from int to bool violation. Finally, the run function reports the violation using the diagnostics engine.

An option category, MyToolCategory, is created for the tool.

In the main function, a CommonOptionsParser object is created to parse command line arguments. If there is an error in parsing the arguments, the program gracefully exits with an error message. Otherwise, a ClangTool object is created with the parsed compilations and source path list. An instance of IntToBoolPrinter is created, and a MatchFinder object is created and the intToBooleanMatcher and Printer are added to it. Finally, the Tool is run with the MatchFinder object.

Overall, this program uses Clang's AST Matchers to find violations of MISRA C++ Rule 5.3.1, and reports them using the diagnostics engine.
*/