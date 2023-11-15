// Include necessary header files
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <vector>

// Use these namespaces to simplify code
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

StatementMatcher OperatorMatcher = anyOf(
    binaryOperator(unless(anyOf(
                                hasOperatorName("||"), hasOperatorName("&&"),
                                hasOperatorName("=="), hasOperatorName("!="),
                                hasOperatorName("="))))
        .bind("binaryoperator"),
    unaryOperator(unless(anyOf(hasOperatorName("&"),hasOperatorName("!")))).bind("unaryoperator"));

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
          "MISRA C++ Rule 4.5.1 Violation! Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator =, the logical operators &&, ||, !, the equality operators == and !=, the unary & operator,and the conditional operator.");
      // Get the left-hand side and right-hand side of the operator and ignore
      // implicit casts
      auto *LHS = binOp ? binOp->getLHS()->IgnoreParenImpCasts() : nullptr;
      auto *RHS = binOp ? binOp->getRHS()->IgnoreParenImpCasts() : nullptr;
      // Check if either operands is of boolean type
      if (LHS && RHS && (LHS->getType()->isBooleanType() || RHS->getType()->isBooleanType())) {
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
          "MISRA C++ Rule 4.5.1 Violation! Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator =, the logical operators &&, ||, !, the equality operators == and !=, the unary & operator,and the conditional operator.");
          auto *operand = unOp->getSubExpr()->IgnoreParenImpCasts();
      // Check if the operand is of boolean type
      if (operand && operand->getType()->isBooleanType()) {
        // Report the MISRA C++ rule 4.5.1 violation
        DE.Report(loc, ID);
      }
    }
  }
};

// Create an option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

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

  OperatorPrinter printer;
  MatchFinder finder;
  finder.addMatcher(OperatorMatcher, &printer);

  return Tool.run(newFrontendActionFactory(&finder).get());
}

                                  //DOCUMENTATION
/*
This is a C++ program that uses the Clang AST Matchers library to find violations of the MISRA C++ Rule 4.5.1, which states that expressions with type bool shall not be used as operands to built-in operators other than the assignment operator =, the logical operators &&, ||, !, the equality operators == and !=, the unary & operator, and the conditional operator.

The program first includes necessary header files, which are used to import the Clang AST Matchers library, the Clang Frontend Actions library, the Clang Tooling library, and the LLVM command line library. It also uses several namespaces to simplify the code.

The program then defines a `StatementMatcher` called `OperatorMatcher` that matches binary and unary operators, except for those that are allowed by the MISRA C++ Rule 4.5.1. The matcher uses the `bind` function to capture the matched nodes.

The program defines a class called `OperatorPrinter` that inherits from the `MatchFinder::MatchCallback` class. The `OperatorPrinter` class overrides the `run` function, which is called when a match is found. The function retrieves the matched binary and unary operators, gets the location of the operator in the source code, and checks if either operand is of boolean type. If either operand is of boolean type, the function reports a custom error message using the `DiagnosticsEngine` class.

Finally, the program defines an option category for the tool and creates a `CommonOptionsParser` object to parse command line arguments. It creates a `ClangTool` object and a `MatchFinder` object, adds the `OperatorMatcher` to the `MatchFinder`, and runs the tool using a frontend action factory and the `run` function.
*/
