set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-3.9.3
  Rule-3.9.3.cpp
  )
target_link_libraries(Rule-3.9.3
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )