set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-5.0.14
  Rule-5.0.14.cpp
  )
target_link_libraries(Rule-5.0.14
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )