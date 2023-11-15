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

// Create a matcher to match casts from integral types to boolean

StatementMatcher IntegralToBoolCastMatcher = implicitCastExpr(
    hasCastKind(CK_IntegralToBoolean),
    hasParent(stmt(anyOf(ifStmt(), whileStmt(), forStmt()))),
    unless(anyOf(
            hasAncestor(ifStmt(hasDescendant(varDecl().bind("varDecl")))),
            hasAncestor(whileStmt(hasDescendant(varDecl().bind("varDecl"))))
        ))
    ).bind("integralToBoolCast");


// Create a callback class for the match found by the matcher
class IntegralToBoolCastPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched cast expression node
    const ImplicitCastExpr *castExpr = Result.Nodes.getNodeAs<ImplicitCastExpr>("integralToBoolCast");
    // Get the matched unary operator node
    // Get the location of the cast in the source code
    SourceLocation loc = castExpr->getBeginLoc();
    // Get the full location of the cast in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the integral to boolean violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.0.13 Violation! The condition of an if-statement and the condition of an iteration-statement shall have type bool.");

    // Report the integral to boolean violation
    DE.Report(loc, ID);
  }
};

StatementMatcher OperatorMatcher = anyOf(
    binaryOperator(anyOf(
                                hasOperatorName("||"), hasOperatorName("&&")
                                ))
        .bind("binaryoperator"),
    unaryOperator(((hasOperatorName("&"),hasOperatorName("!")))).bind("unaryoperator"));

// Create a callback class for the match found by the matcher
class OperatorPrinter : public MatchFinder::MatchCallback {
public:
  // Override the virtual run function to process the match result
  virtual void run(const MatchFinder::MatchResult &Result) override {
    // Get the matched binary operator node
    const BinaryOperator *binOp =
        Result.Nodes.getNodeAs<BinaryOperator>("binaryoperator");
    // Get the matched unary operator node
    const UnaryOperator *unOp =
        Result.Nodes.getNodeAs<UnaryOperator>("unaryoperator");
    // Get the location of the operator in the source code
    SourceLocation loc;
    if (binOp) {
      loc = binOp->getOperatorLoc();
      // Get the full location of the operator in the source code
      FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
      // Get the diagnostics engine to report errors
      DiagnosticsEngine &DE = Result.Context->getDiagnostics();
      // Get a custom error ID for the violation
      const unsigned ID = DE.getCustomDiagID(
          clang::DiagnosticsEngine::Error,
"MISRA C++ Rule 5.0.13 Violation! The condition of an if-statement and the condition of an iteration-statement shall have type bool.");      // Get the left-hand side and right-hand side of the operator and ignore
      // implicit casts
      auto *LHS = binOp ? binOp->getLHS()->IgnoreParenImpCasts() : nullptr;
      auto *RHS = binOp ? binOp->getRHS()->IgnoreParenImpCasts() : nullptr;
      // Check if either operands is of boolean type
      if (LHS && RHS && !(LHS->getType()->isBooleanType() && RHS->getType()->isBooleanType())) {
        // Report the violation
        DE.Report(loc, ID);
      }
    }

    if (unOp) {
      loc = unOp->getOperatorLoc();
      // Get the full location of the operator in the source code
      FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
      // Get the diagnostics engine to report errors
      DiagnosticsEngine &DE = Result.Context->getDiagnostics();
      // Get a custom error ID for the violation
      const unsigned ID = DE.getCustomDiagID(
          clang::DiagnosticsEngine::Error,
"MISRA C++ Rule 5.0.13 Violation! The condition of an if-statement and the condition of an iteration-statement shall have type bool.");      // Get the left-hand side and right-hand side of the operator and ignore
          auto *operand = unOp->getSubExpr()->IgnoreParenImpCasts();
      // Check if the operand is of boolean type
      if (operand && !operand->getType()->isBooleanType()) {
        // Report the MISRA C++ rule 4.5.1 violation
        DE.Report(loc, ID);
      }
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
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  IntegralToBoolCastPrinter Printer1;
  OperatorPrinter Printer2;
  MatchFinder Finder;
  Finder.addMatcher(IntegralToBoolCastMatcher, &Printer1);
  Finder.addMatcher(OperatorMatcher,&Printer2);

  return Tool.run(newFrontendActionFactory(&Finder).get());

}
                              //DOCUMENTATION
/*
This code is a C++ tool that uses the Clang library to enforce MISRA C++ rules, specifically Rule 5.0.13. The code includes necessary header files for Clang, as well as the necessary namespaces. The tool defines two matchers using the AST matchers API to find violations of Rule 5.0.13.

The first matcher is called IntegralToBoolCastMatcher and matches implicit casts from integral types to boolean types. The matcher searches for these casts in the condition of an if-statement or an iteration statement (for, while, do-while loops) but excludes cases where the condition is a declaration or initialization statement. If a match is found, a callback function called IntegralToBoolCastPrinter is invoked to report the violation. The function extracts the location of the cast and creates a custom error message using the DiagnosticsEngine.

The second matcher is called OperatorMatcher and matches binary or unary operators that are used in the condition of an if-statement or an iteration statement. The matcher checks if either operand is not of boolean type and reports the violation if it is found. If a match is found, a callback function called OperatorPrinter is invoked to report the violation. The function extracts the location of the operator and creates a custom error message using the DiagnosticsEngine.

The code also defines an option category for the tool.
*/
