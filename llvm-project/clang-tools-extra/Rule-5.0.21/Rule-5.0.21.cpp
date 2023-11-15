#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <vector>
#include "clang/AST/ASTContext.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

// StatementMatcher BitwiseOpMatcher = anyOf(
//     binaryOperator(
//         anyOf(hasOperatorName("|"), hasOperatorName("&"), hasOperatorName("^"), 
//               hasOperatorName("<<"), hasOperatorName(">>"), hasOperatorName("|="),
//               hasOperatorName("&="), hasOperatorName("^="), hasOperatorName(">>="),
//               hasOperatorName("<<=")),
//         hasEitherOperand(hasType(isSignedInteger())),
//         hasEitherOperand(unless(hasType(isUnsignedInteger())))
//     ).bind("binaryBitwiseOp"),
//     unaryOperator(
//         hasOperatorName("~"),
//         hasUnaryOperand(hasType(isSignedInteger())),
//         unless(hasType(isUnsignedInteger()))
//     ).bind("unaryBitwiseOp")
// );

StatementMatcher BitwiseOpMatcher = anyOf(
    binaryOperator(
        anyOf(hasOperatorName("|"), hasOperatorName("&"), hasOperatorName("^"), 
              hasOperatorName("<<"), hasOperatorName(">>"), hasOperatorName("|="),
              hasOperatorName("&="), hasOperatorName("^="), hasOperatorName(">>="),
              hasOperatorName("<<=")),
        hasEitherOperand(hasType(isSignedInteger())),
        hasEitherOperand(unless(hasType(isUnsignedInteger()))),
        unless(hasType(isUnsignedInteger()))
    ).bind("binaryBitwiseOp"),
    unaryOperator(
        hasOperatorName("~"),
        hasUnaryOperand(hasType(isSignedInteger())),
        unless(hasType(isUnsignedInteger()))
    ).bind("unaryBitwiseOp")
);


class BitwiseOpChecker : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) override {
    if (const BinaryOperator *bitwiseOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryBitwiseOp")) {
        SourceLocation loc = bitwiseOp->getBeginLoc();
        FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();
        const unsigned ID = DE.getCustomDiagID(
            clang::DiagnosticsEngine::Error,
            "MISRA C++ Rule 5.0.21 Violation! Bitwise operator applied to operands of non-unsigned underlying type");

        DE.Report(loc, ID);
    }
    if (const UnaryOperator *bitwiseOp = Result.Nodes.getNodeAs<UnaryOperator>("unaryBitwiseOp")) {
        SourceLocation loc = bitwiseOp->getBeginLoc();
        FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();
        const unsigned ID = DE.getCustomDiagID(
            clang::DiagnosticsEngine::Error,
            "MISRA C++ Rule 5.0.21 Violation! Bitwise operator applied to operands of non-unsigned underlying type");

        DE.Report(loc, ID);
    }
  }
};

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  BitwiseOpChecker Checker;
  MatchFinder Finder;
  Finder.addMatcher(BitwiseOpMatcher, &Checker);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
                                        //DOCUMENTATION
/*
This code is a C++ program that uses the Clang AST Matcher library to detect violations of MISRA C++ Rule 5.0.21, which prohibits applying bitwise operators to operands of non-unsigned underlying type.

The code starts by including several header files, including those for the Clang AST Matcher library and the Clang Tooling library.

The `using` statements at the beginning of the code declare several namespaces that will be used throughout the code, including `clang`, `clang::ast_matchers`, `clang::tooling`, and `llvm`.

Next, a `StatementMatcher` object called `BitwiseOpMatcher` is defined. This matcher is used to match expressions that use bitwise operators, including `|`, `&`, `^`, `<<`, `>>`, `|=`, `&=`, `^=`, `>>=`, and `<<=`. The matcher includes two sub-matchers: one for binary operators and one for unary operators.

The `BinaryOperator` matcher matches binary operators that have one of the specified operator names, at least one operand that has a signed integer type, and at least one operand that is not an unsigned integer type.

The `UnaryOperator` matcher matches unary operators that have the `~` operator name, an operand that has a signed integer type, and an operand that is not an unsigned integer type.

After the matcher is defined, a class called `BitwiseOpChecker` is defined. This class inherits from `MatchFinder::MatchCallback`, which is a base class for objects that can be registered with a `MatchFinder` object to be called when a match is found. The `BitwiseOpChecker` class overrides the `run` method of the `MatchCallback` class to perform actions when a match is found.

In particular, if a match is found for the `binaryBitwiseOp` or `unaryBitwiseOp` node in the AST, the `run` method retrieves the source location of the match and creates a diagnostic message using the Clang `DiagnosticsEngine` object. The diagnostic message includes a custom error code and a message that explains the violation of MISRA C++ Rule 5.0.21.

Finally, the `main` function sets up the Clang tool by creating a `CommonOptionsParser` object, creating a `ClangTool` object, creating a `BitwiseOpChecker` object, creating a `MatchFinder` object, adding the `BitwiseOpMatcher` to the `MatchFinder`, and running the `ClangTool` with the `MatchFinder` as a frontend action.

Overall, this code is an example of using the Clang AST Matcher library to detect violations of a coding standard rule, in this case MISRA C++ Rule 5.0.21. It demonstrates how to define a matcher for specific types of AST nodes, how to create a custom `MatchCallback` object to perform actions when matches are found, and how to use the Clang `DiagnosticsEngine` object to create custom diagnostic messages.
*/