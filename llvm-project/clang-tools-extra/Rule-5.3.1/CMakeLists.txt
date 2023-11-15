set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-5.3.1
  Rule-5.3.1.cpp
  )
target_link_libraries(Rule-5.3.1
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )