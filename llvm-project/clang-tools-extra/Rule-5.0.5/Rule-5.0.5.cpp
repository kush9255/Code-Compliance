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

// Create a matcher to match float to int casts downcast not allowed
StatementMatcher FloatToIntCastMatcher =
    castExpr(hasCastKind(CK_FloatingToIntegral))
        .bind("floattointcast");

// Create a matcher to match int to float casts upcasting only explicit
StatementMatcher IntToFloatCastMatcher =
    castExpr(allOf(
        hasCastKind(CK_IntegralToFloating),
         unless(hasParent(explicitCastExpr()))
    )).bind("inttofloatcast");

// Create a callback class for the match found by the matcher
class CastPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched cast expression node
    const CastExpr *castExpr = nullptr;
    if (const auto *FloatToIntCast = Result.Nodes.getNodeAs<CastExpr>("floattointcast")) {
        castExpr = FloatToIntCast;
    } else if (const auto *IntToFloatCast = Result.Nodes.getNodeAs<CastExpr>("inttofloatcast")) {
        castExpr = IntToFloatCast;
    } 
    else {
        // No cast expression node found, return
        return;
    }

    // Get the location of the cast in the source code
    SourceLocation loc = castExpr->getBeginLoc();
    // Get the full location of the cast in the source code
    FullSourceLoc fullLoc(loc,(Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();

    if (castExpr->getCastKind() == CK_FloatingToIntegral) {
        // Get a custom error ID for the cast from float to int violation
        const unsigned ID = DE.getCustomDiagID(
            clang::DiagnosticsEngine::Error,
            "MISRA C++ Rule 5.0.5 Violation! There shall be no implicit floating-integral conversions.");
        // Report the cast from float to int violation
        DE.Report(loc, ID);
    } else if (castExpr->getCastKind() == CK_IntegralToFloating) {
        // Get a custom error ID for the cast from int to float violation
        const unsigned ID = DE.getCustomDiagID(
            clang::DiagnosticsEngine::Error,
            "MISRA C++ Rule 5.0.5 Violation! There shall be no floating-integral conversions.");
        // Report the cast from int to float violation
        DE.Report(loc, ID);
    }
  }
};
// Create an option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// Main function
int main(int argc, const char **argv) {
  // Create a CommonOptionsParser object to parse command line arguments
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();

  // Create a ClangTool instance to run the tool
  ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

  // Create a CastPrinter instance as the callback for the match
  CastPrinter Printer;
  // Create a MatchFinder instance to find matches based on the matchers
  MatchFinder Finder;
  // Add the matchers to the MatchFinder instance
  Finder.addMatcher(FloatToIntCastMatcher, &Printer);
  Finder.addMatcher(IntToFloatCastMatcher, &Printer);

  // Run the tool with the MatchFinder instance as the action
  return Tool.run(newFrontendActionFactory(&Finder).get());
}
                                //DOCUMENTATION
/*
This code is a tool that uses the Clang AST Matchers library to detect and report violations of the MISRA C++ Rule 5.0.5, which states that there shall be no floating-integral conversions. The tool detects two types of conversions: casting from float to int and casting from int to float. 

The tool works by defining two matchers, one for each type of conversion. The FloatToIntCastMatcher matches any cast expression that converts from floating-point to integral type, while the IntToFloatCastMatcher matches any cast expression that converts from integral to floating-point type. Both matchers use the AST matcher library to define the pattern of the nodes to be matched.

Once a match is found by the matchers, the tool creates an instance of the CastPrinter class, which implements the MatchFinder::MatchCallback interface. The CastPrinter class overrides the virtual run() function, which is called by the MatchFinder when a match is found. The run() function retrieves the matched cast expression node, gets its location in the source code, and reports an error message using the Clang DiagnosticsEngine.

To use the tool, the user provides a C++ source file as input to the tool. The tool can be run from the command line, and it takes the path to the source file as an argument. The tool uses the CommonOptionsParser class from the Clang Tooling library to parse the command line arguments and create a ClangTool instance. The ClangTool instance is then used to run the tool with the MatchFinder instance as the action.

This tool can be useful for software development teams that want to enforce MISRA C++ Rule 5.0.5 and ensure that their code does not contain any floating-integral conversions. By using this tool, teams can catch these types of violations early in the development process and avoid potential bugs or issues caused by these conversions.
*/