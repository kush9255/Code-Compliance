set(LLVM_LINK_COMPONENTS support)

add_clang_executable(Rule-5.0.5
  Rule-5.0.5.cpp
  )
target_link_libraries(Rule-5.0.5
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )