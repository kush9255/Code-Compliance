set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-5.3.2
  Rule-5.3.2.cpp
  )
target_link_libraries(Rule-5.3.2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )