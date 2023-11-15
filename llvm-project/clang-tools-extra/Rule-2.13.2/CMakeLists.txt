set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-2.13.2
  Rule-2.13.2.cpp
  )
target_link_libraries(Rule-2.13.2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )