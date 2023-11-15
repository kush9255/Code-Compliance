set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-7.1
  Rule-7.1.cpp
  )
target_link_libraries(Rule-7.1
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )