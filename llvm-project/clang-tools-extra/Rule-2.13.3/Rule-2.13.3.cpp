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

// This class is a frontend action that will be run on the preprocessor stage.
class OctalLiteralFinder : public clang::PreprocessorFrontendAction {
public:
  virtual void ExecuteAction() {
    // Get the preprocessor, diagnostics engine, and source manager.
    auto& pp = getCompilerInstance().getPreprocessor();
    auto& diags = getCompilerInstance().getDiagnostics();
    auto& sm = pp.getSourceManager();

    // Tokenize the input source file
    pp.EnterMainSourceFile();
    clang::Token tok;
    bool uns_flg=false;
    bool oct_flg=false;
    // Process each token in the input source file.
    while (true) {
      pp.Lex(tok);
      if (tok.is(clang::tok::eof))
        break;
        
      // If we find an "unsigned" keyword, set the flag to true.
      if (tok.is(clang::tok::kw_unsigned)) {
        errs() << "Found unsigned keyword!\n";
        uns_flg=true;
      }
      
      // If we find a literal token that starts with '0' and has at least 2 characters,
      // and is either a numeric or character constant token, we may have found an octal literal.
      if (tok.isLiteral() && tok.getLiteralData() != nullptr &&
          tok.getLength() >= 2 && tok.getLiteralData()[0] == '0' &&
          (tok.getKind() == clang::tok::numeric_constant ||
           tok.getKind() == clang::tok::char_constant)) {
        // Found an octal literal
        auto loc = sm.getSpellingLoc(tok.getLocation());
        auto line = sm.getSpellingLineNumber(loc);
        // Check if the octal literal violates MISRA C++ Rule 2.13.3, which requires
        // that all octal or hexadecimal integer literals of unsigned type have a 'U' suffix.
        if(uns_flg && (tok.getLiteralData()[0] == 'x' || tok.getLiteralData()[0] == 'X' ||  tok.getLiteralData()[tok.getLength()-1] != 'U'))
          diags.Report(loc, diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "MISRA C++ Rule 2.13.3 Violation! A U  suffix shall be applied to all octal or hexadecimal integer literals of unsigned type ")) << line;
        uns_flg=false;
      }
    }
  }
};

// Define an option category for the tool.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char** argv) {
  // Parse the command line arguments and options.
  auto ExpectedParser = CommonOptionsParser::create(argc, argv,MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<OctalLiteralFinder>().get());
  // Run the frontend action defined by OctalLiteralFinder on the input source file.

}

                              //DOCUMENTATION
/*
The given code is a C++ program that uses the Clang C++ compiler infrastructure to find octal literals in a source file and check if they violate MISRA C++ Rule 2.13.3. The program takes a C++ source file as input and reports any octal literals that violate the MISRA C++ Rule 2.13.3. 

The program uses the Clang Tooling API to create a custom frontend action, `OctalLiteralFinder`, which inherits from the `PreprocessorFrontendAction` class. The `OctalLiteralFinder` class overrides the `ExecuteAction()` method, which is called when the frontend action is executed. 

In the `ExecuteAction()` method, the program retrieves the preprocessor, diagnostics engine, and source manager from the compiler instance. The input source file is tokenized using the preprocessor, and each token is processed to find octal literals. The program checks if a literal token starts with '0' and has at least 2 characters and is either a numeric or character constant token, indicating it may be an octal literal. If it is an octal literal, the program checks if it violates MISRA C++ Rule 2.13.3, which requires that all octal or hexadecimal integer literals of unsigned type have a 'U' suffix. If a violation is found, the program reports an error using the diagnostics engine. 

The program defines an option category for the tool and uses the `CommonOptionsParser` class to parse command line arguments and options. It creates a `ClangTool` object and runs the frontend action defined by `OctalLiteralFinder` on the input source file.

Overall, the program provides a way to enforce a specific coding rule by finding violations in the source code and reporting them to the user.
*/