set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-2.10.3
  Rule-2.10.3.cpp
  )
target_link_libraries(Rule-2.10.3
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )