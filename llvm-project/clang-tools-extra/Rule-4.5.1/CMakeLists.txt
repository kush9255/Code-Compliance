set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-4.5.1
  Rule-4.5.1.cpp
  )
target_link_libraries(Rule-4.5.1
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )