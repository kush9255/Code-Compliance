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

// Define a class that inherits from the clang::PreprocessorFrontendAction class
class OctalLiteralFinder : public clang::PreprocessorFrontendAction {
public:
  virtual void ExecuteAction() {
    // Get references to the preprocessor, diagnostics, and source manager
    auto& pp = getCompilerInstance().getPreprocessor();
    auto& diags = getCompilerInstance().getDiagnostics();
    auto& sm = pp.getSourceManager();

    // Tokenize the input source file
    pp.EnterMainSourceFile();
    clang::Token tok;
    bool uns_flg=false;
    bool oct_flg=false;
    while (true) {
      pp.Lex(tok);
      // Break if we reach the end of the file
      if (tok.is(clang::tok::eof))
        break;

      // Check if the current token is the "unsigned" keyword
      if (tok.is(clang::tok::kw_unsigned)) {
        // Print a message to stderr
        errs() << "Found unsigned keyword!\n";
        // Set the unsigned flag to true
        uns_flg=true;
      }
      
      // Check if the current token is an octal literal
      if (tok.isLiteral() && tok.getLiteralData() != nullptr &&
          tok.getLength() >= 2 && tok.getLiteralData()[0] == '0' &&
          (tok.getKind() == clang::tok::numeric_constant ||
           tok.getKind() == clang::tok::char_constant)) {
        // Found an octal literal, get the location and line number
        auto loc = sm.getSpellingLoc(tok.getLocation());
        auto line = sm.getSpellingLineNumber(loc);
        // Check if the octal literal violates a coding rule
        if((tok.getLiteralData()[1] == 'x' || tok.getLiteralData()[1] == 'X'))
          diags.Report(loc, diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "MISRA C++ Rule 3.9.3 Violation! The underlying bit representations of floating-point values shall not be used.")) << line;
        // Reset the unsigned flag
        uns_flg=false;
      }
    }
  }
};

// Define an option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char** argv) {
  // Create a CommonOptionsParser object
  auto ExpectedParser = CommonOptionsParser::create(argc, argv,MyToolCategory);
  if (!ExpectedParser) {
    // If creation of the CommonOptionsParser object fails, print an error message and exit
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  // Get a reference to the CommonOptionsParser object
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  // Create a ClangTool object
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  // Run the tool with an instance of the OctalLiteralFinder class as the frontend action
  return Tool.run(newFrontendActionFactory<OctalLiteralFinder>().get());
}
                              //DOCUMENTATION
/*
This is a C++ code that uses the Clang tooling library to identify violations of a specific rule in C++ code. The rule that this code checks for is "MISRA C++ Rule 3.9.3 Violation! The underlying bit representations of floating-point values shall not be used."

The code starts by including the necessary headers from the Clang tooling library and the LLVM support library. It then defines a class called OctalLiteralFinder that inherits from the clang::PreprocessorFrontendAction class.

In the main function, the code creates a CommonOptionsParser object, which parses the command-line options, and a ClangTool object, which represents the compilation process. It then runs the tool by calling the run method on the ClangTool object and passing it a factory for the frontend action that will run the OctalLiteralFinder.

The OctalLiteralFinder class defines a virtual function called ExecuteAction that is called by the Clang tool when it processes the input source file. In this method, the code gets references to the preprocessor, diagnostics, and source manager. It then tokenizes the input source file and searches for octal literals and the "unsigned" keyword.

If an octal literal is found, the code gets the location and line number of the literal and checks whether it violates a coding rule. If it does, the code reports an error using Clang's diagnostics engine.

If the "unsigned" keyword is found, the code sets a flag to true. If an octal literal is found and the flag is true, the code reports an error because the combination of "unsigned" and octal literals violates the coding rule.

Overall, this code uses the Clang tooling library to identify violations of a specific coding rule in C++ code and report them using Clang's diagnostics engine.
*/