set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-2.13.4
  Rule-2.13.4.cpp
  )
target_link_libraries(Rule-2.13.4
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )