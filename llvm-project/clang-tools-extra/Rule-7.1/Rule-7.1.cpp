#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;
using namespace clang::tooling;
using namespace llvm;

// Define a class OctalLiteralFinder which inherits from PreprocessorFrontendAction
class OctalLiteralFinder : public clang::PreprocessorFrontendAction {
public:
  // Override the ExecuteAction() method
  virtual void ExecuteAction() {
    // Get the preprocessor, diagnostics, and source manager from the compiler instance
    auto& pp = getCompilerInstance().getPreprocessor();
    auto& diags = getCompilerInstance().getDiagnostics();
    auto& sm = pp.getSourceManager();

    // Tokenize the input source file
    pp.EnterMainSourceFile();
    clang::Token tok;

    // Loop over all the tokens in the input source file
    while (true) {
      pp.Lex(tok);
      if (tok.is(clang::tok::eof))
        break;

      // Check if the token is a literal with a length of at least 2 and starts with '0'
      if (tok.isLiteral() && tok.getLiteralData() != nullptr &&
          tok.getLength() >= 2 && tok.getLiteralData()[0] == '0' &&
          (tok.getKind() == clang::tok::numeric_constant ||
           tok.getKind() == clang::tok::char_constant)) {
        // Found an octal literal, report a diagnostic for MISRA C Rule 7.1 violation
        auto loc = sm.getSpellingLoc(tok.getLocation());
        auto line = sm.getSpellingLineNumber(loc);
        diags.Report(loc, diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "MISRA C Rule 7.1 Violation! octal constant on line %0")) << line;
      }
    }
  }
};

// Define a command line option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// Define the main function
int main(int argc, const char** argv) {
  // Parse the command line options using CommonOptionsParser
  auto ExpectedParser = CommonOptionsParser::create(argc, argv,MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // Run the OctalLiteralFinder action on the input source file(s)
  return Tool.run(newFrontendActionFactory<OctalLiteralFinder>().get());
}
                                          //DOCUMENTATION
/*
This is a C++ program that uses the Clang tooling library to analyze C source code for violations of MISRA C Rule 7.1, which prohibits the use of octal literals in C and C++ source code.

The program defines a class called "OctalLiteralFinder" that inherits from "clang::PreprocessorFrontendAction". The "PreprocessorFrontendAction" is a class in Clang's FrontendActions.h header file, which defines an action that is executed by the Clang driver before any other frontend actions.

The "OctalLiteralFinder" class overrides the "ExecuteAction()" method of the "PreprocessorFrontendAction" class. This method is called by the Clang driver to execute the action.

The "ExecuteAction()" method gets the preprocessor, diagnostics, and source manager from the compiler instance. It then tokenizes the input source file using the preprocessor, and loops over all the tokens in the input source file.

For each token, the method checks if it is a literal with a length of at least 2 and starts with '0'. If so, it checks if the literal is a numeric constant or a character constant. If the literal is an octal constant, the method reports a diagnostic for MISRA C Rule 7.1 violation.

The diagnostic is reported using the "getCustomDiagID()" method of the diagnostics engine. This method creates a custom diagnostic message with a unique ID that can be used to report the diagnostic. The custom diagnostic message includes the line number of the octal constant in the input source file.

The main function of the program creates a command line option category for the tool using the "llvm::cl::OptionCategory" class. It then parses the command line options using the "CommonOptionsParser::create()" method, which returns a "Expected<CommonOptionsParser>" object.

If the command line options are supported, the program creates a "ClangTool" object and runs the "OctalLiteralFinder" action on the input source file(s) using the "run()" method of the "ClangTool" class.

Overall, this program is a useful tool for analyzing C++ source code for violations of MISRA C Rule 7.1.
*/
