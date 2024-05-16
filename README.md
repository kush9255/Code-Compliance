<div align="center">

# 💻 `Code Compliance Project` 💻 

</div>

## About

This repository contains all the Clang Passes for Code Compliance Project using the MISRA C 2012 standards written as a part of "CS6383-Introduction to Compiler Engineering" course.

## Clang Passes implemented :


## Quick Start

Clone the LLVM Project repository

```bash
git clone --depth 1 --branch llvmorg-14.0.6  https://github.com/llvm/llvm-project/
```

Now, follow the steps below to run these clang passes on your local machine:

```bash
cd llvm-project

# Create a build directory inside this llvm-project folder & go inside the build folder
mkdir build && cd build

# Build Clang using CMake
cmake -G "Unix Makefiles" ../llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_BUILD_TESTS=ON

# Now compile our Clang tool from the build directory using make command
make clang clang-tools-extra clang-check

# Now, copy everything from 'clang-extra-tools' within this repository into 'llvm-project/clang-extra-tools' and compile our new tools by running make from the build directory
make
```

</table>
