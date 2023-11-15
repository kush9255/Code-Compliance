set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-4.5.2
  Rule-4.5.2.cpp
  )
target_link_libraries(Rule-4.5.2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )