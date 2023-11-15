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
    binaryOperator(unless(anyOf(hasOperatorName("<"), hasOperatorName("<="),
                                hasOperatorName(">"), hasOperatorName(">="),
                                hasOperatorName("=="), hasOperatorName("!="),
                                hasOperatorName("="), hasOperatorName("[]"))))
        .bind("binaryoperator"),
    unaryOperator(unless(hasOperatorName("&"))).bind("unaryoperator"));

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
      // Get a custom error ID for MISRA C++ rule 4.5.2 violation
      const unsigned ID = DE.getCustomDiagID(
          clang::DiagnosticsEngine::Error,
          "MISRA C++ Rule 4.5.2 Violation! Expressions with type enum shall "
          "not be used as operands to built-in operators other than the "
          "subscript operator [ ], the assignment operator =, the equality "
          "operators == and !=, the unary & operator, and the relational "
          "operators <, <=, >, >=.");
      // Get the left-hand side and right-hand side of the operator and ignore
      // implicit casts
      auto *LHS = binOp ? binOp->getLHS()->IgnoreParenImpCasts() : nullptr;
      auto *RHS = binOp ? binOp->getRHS()->IgnoreParenImpCasts() : nullptr;
      // Check if both operands are of enumeration type
      if (LHS && RHS && LHS->getType()->isEnumeralType() ||
          RHS->getType()->isEnumeralType()) {
        const EnumType *enumLHS = LHS->getType()->getAs<EnumType>();
        const EnumType *enumRHS = RHS->getType()->getAs<EnumType>();
        // Check if both operands are of the same enumeration type
       // errs()<<"left enum "<<enumLHS->getDecl()<<" right enum "<<enumRHS->getDecl()<<"\n";
       // if (enumLHS && enumRHS && enumLHS->getDecl() != enumRHS->getDecl()) {
          // Report the MISRA C++ rule 4.5.2 violation
          DE.Report(loc, ID);
       // }
      }
     // DE.Report(loc, ID);
    }

    if (unOp) {
      loc = unOp->getOperatorLoc();
      // Get the full location of the operator in the source code
      FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
      // Get the diagnostics engine to report errors
      DiagnosticsEngine &DE = Result.Context->getDiagnostics();
      // Get a custom error ID for MISRA C++ rule 4.5.2 violation
      const unsigned ID = DE.getCustomDiagID(
          clang::DiagnosticsEngine::Error,
          "MISRA C++ Rule 4.5.2 Violation! Expressions with type enum shall "
          "not be used as operands to built-in operators other than the "
          "subscript operator [ ], the assignment operator =, the equality "
          "operators == and !=, the unary & operator, and the relational "
          "operators <, <=, >, >=.");
      // Get the operand of the operator and ignore implicit casts
      auto *operand = unOp->getSubExpr()->IgnoreParenImpCasts();
      // Check if the operand is of enumeration type
      if (operand && operand->getType()->isEnumeralType()) {
        // Report the MISRA C++ rule 4.5.2 violation
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
This is a C++ program that uses the Clang AST Matchers library to match and print occurrences of binary and unary operators that violate the MISRA C++ Rule 4.5.2, which states that expressions with type enum shall not be used as operands to built-in operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=.

The program includes necessary header files for the Clang AST Matchers library and other required dependencies. It defines a statement matcher called OperatorMatcher, which uses the anyOf function to match binary and unary operators that violate the MISRA C++ Rule 4.5.2. The binaryOperator and unaryOperator functions are used to match binary and unary operators, respectively. The unless function is used to exclude operators that are allowed by the rule.

The program then defines a callback class called OperatorPrinter, which inherits from the MatchFinder::MatchCallback class. The OperatorPrinter class overrides the virtual run function to process the match result. The run function gets the matched binary or unary operator node, gets the location of the operator in the source code, and checks if the operands of the operator are of enumeration type. If the operands are of enumeration type and the operator violates the MISRA C++ Rule 4.5.2, the program reports a custom error message using the DiagnosticsEngine object.

Finally, the program creates an option category for the tool and uses the CommonOptionsParser class to parse command line arguments. The ClangTool object is created using the parsed command line arguments and is run with the matchers and callbacks defined earlier. If there are any errors parsing the command line arguments, the program will fail gracefully and report the error.
*/