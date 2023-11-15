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


## Contributors

<table>
  <tr>
  <td align="center"><a href="https://github.com/girishgr8/"><img src="https://avatars.githubusercontent.com/girishgr8" width="100px;" alt=""/><br /><sub><b>Girish Thatte</b></sub></a></td>
  <td align="center"><a href="https://github.com/raghav9-97/"><img src="https://avatars.githubusercontent.com/raghav9-97" width="100px;" alt=""/><br /><sub><b>Raghvendra Gupta</b></sub></a></td>
  <td align="center"><a href="https://github.com/Venky-8/"><img src="https://avatars.githubusercontent.com/Venky-8" width="100px;" alt=""/><br /><sub><b>Venkatesh Mangnale</b></sub></a></td>
  <td align="center"><a href="https://github.com/deepak1h/"><img src="https://avatars.githubusercontent.com/deepak1h" width="100px;" alt=""/><br /><sub><b>Deepak Pandey</b></sub></a></td>
  <td align="center"><a href="https://github.com/kush9255/"><img src="https://avatars.githubusercontent.com/kush9255" width="100px;" alt=""/><br /><sub><b>Kushal Sharma</b></sub></a></td>
  </tr>
</table>
