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

// Define a class OctalLiteralFinder that derives from the PreprocessorFrontendAction class
class OctalLiteralFinder : public clang::PreprocessorFrontendAction {
public:
  virtual void ExecuteAction() {
    auto& pp = getCompilerInstance().getPreprocessor();
    auto& diags = getCompilerInstance().getDiagnostics();
    auto& sm = pp.getSourceManager();

    // Tokenize the input source file
    pp.EnterMainSourceFile();
    clang::Token tok;
    while (true) {
      pp.Lex(tok);
      if (tok.is(clang::tok::eof))
        break;
        
      // If the token is a literal and its kind is numeric_constant or char_constant, check if it's an octal literal
      if (tok.isLiteral() && tok.getLiteralData() != nullptr &&
          (tok.getKind() == clang::tok::numeric_constant ||
           tok.getKind() == clang::tok::char_constant)) {
        // Found an octal literal
    auto loc = sm.getSpellingLoc(tok.getLocation());
    auto line = sm.getSpellingLineNumber(loc);
    bool char_flg=check_char(tok.getLiteralData()[tok.getLength()-1]);
    // Check if the last character of the literal is a lowercase letter
    if(char_flg)
    diags.Report(loc, diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "MISRA C++ Rule 2.13.4 Literal suffixes shall be upper case.")) << line;
      }
    }
  }

  // A helper function that checks if a character is a lowercase letter
  bool check_char(char ch){
     if (std::isalpha(ch) && (islower(ch))) { 
      return true;
  }
    return false;
  }
};

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char** argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv,MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // Run the OctalLiteralFinder action on the source code
  return Tool.run(newFrontendActionFactory<OctalLiteralFinder>().get());
}
                                  //DOCUMENTATION
/*
The code you provided is a C++ program that uses the Clang tooling library to find octal literals in a source file and check if their suffixes are in upper case, in accordance with the MISRA C++ Rule 2.13.4.

Here is a detailed explanation of each part of the code:

1. Header files
The program includes several header files from the Clang tooling library and LLVM, which provide the necessary classes and functions to work with C++ source code.

2. Namespace declarations
The program uses several namespaces from the Clang and LLVM libraries to avoid naming conflicts.

3. OctalLiteralFinder class
The OctalLiteralFinder class derives from the PreprocessorFrontendAction class. This means that it will be executed by the Clang tooling library as part of the preprocessor phase of compiling a C++ source file.

4. ExecuteAction() function
This function is called by the Clang tooling library when the OctalLiteralFinder class is executed. It retrieves the preprocessor, diagnostics, and source manager objects from the CompilerInstance class, which is a member of the PreprocessorFrontendAction class

The function then tokenizes the input source file using the preprocessor's Lex() function, which returns the next token in the source file. If the token is an octal or character literal, the function checks if the last character of the literal is a lowercase letter. If it is, the function reports an error using the diagnostics object and the location of the offending token.

5. check_char() function
This is a helper function that checks if a character is a lowercase letter.It is called by the ExecuteAction() function to determine if the last character of an octal or character literal is a lowercase letter.

6. OptionCategory and main() function
The program defines an option category using the OptionCategory class from LLVM. This category is used to group the program's command line options.

The main() function uses the CommonOptionsParser class from the Clang tooling library to parse the program's command line options. It then creates a ClangTool object using the command line options and runs the OctalLiteralFinder action on the source code using the newFrontendActionFactory() function.

In summary, the program uses the Clang tooling library and LLVM to find octal literals in a C++ source file and check if their suffixes are in upper case, in accordance with the MISRA C++ Rule 2.13.4. It does this by defining a new class that derives from the PreprocessorFrontendAction class, implementing a function that tokenizes the input source file, and defining a helper function that checks if a character is a lowercase letter. The program also provides a command line interface using the CommonOptionsParser class and the OptionCategory class from LLVM.
*/