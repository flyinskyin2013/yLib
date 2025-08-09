# yLib C++ 代码分析工具 (yLib C++ Code Analysis Tool)

该目录包含用于静态分析 `yLib` C++ 源代码的工具。其主要目的是解析 C++ 类定义，提取继承关系和元信息，并自动生成一个包含整个库的类层次结构的 C++ 源文件 (`ylib_class_info.cpp`)。

This directory contains tools for static analysis of `yLib` C++ source code. Its main purpose is to parse C++ class definitions, extract inheritance relationships and metadata, and automatically generate a C++ source file (`ylib_class_info.cpp`) containing the entire library's class hierarchy.

## 功能概述 (Function Overview)

- **`class_ana` (C++)**: 一个基于 Clang LibTooling 的可执行文件。它负责解析单个 C++ 源文件，遍历其抽象语法树 (AST)，并以 JSON 格式输出其中定义的类的相关信息（如类名、基类、命名空间等）。

- **`class_ana.py` (Python)**: 一个 Python 脚本，作为整个分析过程的协调器。它会自动查找 `yLib` 项目中的所有 C++ 源文件，调用 `class_ana` 对它们进行分析，收集所有类的继承信息，最后生成 `ylib_class_info.cpp` 文件。这个生成的文件为 yLib 的运行时类型信息 (RTTI) 系统提供了必要的数据。

## 环境依赖 (Environment Dependencies)

- **Clang 和 LLVM**: 需要安装 Clang 和 LLVM 库（开发头文件和库文件）。脚本和代码示例使用的是 `18` 版本。请确保已安装相应版本的 `clang`、`libclang-cpp` 和 `libLLVM`。
  - 支持的版本: LLVM/Clang 10.0 - 18.x

- **Python**: 需要 Python 3.6 或更高版本。

## 构建 `class_ana` (Building `class_ana`)

您可以使用以下命令编译 `class_ana.cpp`。请根据您系统中安装的 LLVM/Clang 版本调整路径和库名称。

You can compile `class_ana.cpp` with the following command. Please adjust the paths and library names according to the LLVM/Clang version installed on your system.

```bash
# 确保 LLVM/Clang 的版本号 (例如 -18) 与您的环境匹配
# Make sure the LLVM/Clang version number (e.g., -18) matches your environment
# sudo apt install libclang-dev clang
clang++-18 class_ana.cpp -o class_ana \
    -std=c++17 \
    -I /usr/lib/llvm-18/include/ \
    -L /usr/lib/llvm-18/lib/ \
    -lclang-cpp -lLLVM-18
```

```bash
# for window
wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-18.1.8.zip

unzip llvm-project-llvmorg-18.1.8.zip
cd llvm-project-llvmorg-18.1.8
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..\llvm\ -DLLVM_ENABLE_PROJECTS="clang;lld;clang-tools-extra" -DCMAKE_CXX_CLAGS="/utf-8"  -DLLVM_TARGETS_TO_BUILD="X86"

# download prebuilt-llvm from github
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/clang+llvm-18.1.8-x86_64-pc-windows-msvc.tar.xz
tar -xf clang+llvm-18.1.8-x86_64-pc-windows-msvc.tar.xz

# merge llvm.lib clang.lib
python docs/llvm/simple_llvm_lib.py install_path_clang+llvm-18.1.8-x86_64-pc-windows-msvc 
# now in 'install_path_clang+llvm-18.1.8-x86_64-pc-windows-msvc ', we have LLVM.lib clang.lib

# build class_ana
cl.exe ..\docs\llvm\class_ana.cpp /I "G:\SourceCodes\clang+llvm-18.1.8-x86_64-pc-windows-msvc\include" /std:c++17 /MD /link /libpath:"G:\SourceCodes\clang+llvm-18.1.8-x86_64-pc-windows-msvc\lib\" clang.lib LLVM.lib msvcrt.lib ws2_32.lib version.lib

# generate ylib_class_info.cpp

```

## 使用方法 (Usage)

### 1. 直接使用 `class_ana` (用于调试) (Direct usage of `class_ana` (for debugging))

您可以直接运行 `class_ana` 来分析单个文件。注意，传递给源文件的编译器标志（如头文件包含路径）必须放在 `--` 分隔符之后。

You can run `class_ana` directly to analyze a single file. Note that compiler flags passed to the source file (such as header include paths) must be placed after the `--` separator.

**示例 (Examples):**
```bash
# 分析自身代码
# Analyze the code itself
./class_ana ./class_ana.cpp -- -I /usr/lib/llvm-18/include/

# 分析 yLib 中的一个文件
# Analyze a file in yLib
./class_ana ../src/core/yobject.cpp -- -I /usr/lib/llvm-18/include/ -I ../include/
```

### 2. 使用 `class_ana.py` (推荐) (Using `class_ana.py` (Recommended))

这是生成 `ylib_class_info.cpp` 的标准方法。脚本会自动处理所有细节。

This is the standard method for generating `ylib_class_info.cpp`. The script automatically handles all details.

**步骤 (Steps):**

1. 确保 `class_ana` 可执行文件与 `class_ana.py` 在同一目录下。
   Ensure the `class_ana` executable is in the same directory as `class_ana.py`.

2. 从 `yLib` 项目的根目录的 `build` 或类似目录下运行此脚本。
   Run this script from the `build` directory or similar directory of the `yLib` project root.

3. 提供 `yLib` 的根目录路径作为参数。
   Provide the root directory path of `yLib` as a parameter.

**示例 (Example):**

如果您在 `docs/llvm` 目录下：
If you are in the `docs/llvm` directory:

```bash
# 运行前请先构建 class_ana
# Please build class_ana before running
# `pwd`/../.. 指向 yLib 根目录
# `pwd`/../.. points to the yLib root directory
python3 ./class_ana.py --ylib_root=`pwd`/../..
```

该脚本将在当前目录下生成 `ylib_class_info.cpp` 文件。
The script will generate the `ylib_class_info.cpp` file in the current directory.

## 故障排除 (Troubleshooting)

1. **找不到 LLVM/Clang 库**:
   - 确保已安装 LLVM/Clang 开发包
   - 检查路径是否正确指向 LLVM/Clang 安装目录

2. **class_ana 编译失败**:
   - 检查 LLVM/Clang 版本是否兼容
   - 确保所有依赖库都已正确安装

3. **Python 脚本运行错误**:
   - 检查 Python 版本是否满足要求
   - 确认 class_ana 可执行文件存在且有执行权限

## 其他参考命令 (Other Reference Commands)

以下命令在开发和调试过程中可能会有用：
The following commands may be useful during development and debugging:

```bash
# 使用 clang -ast-dump 查看 C++ 文件的抽象语法树
# Use clang -ast-dump to view the abstract syntax tree of a C++ file
clang++-18 -Xclang -ast-dump -fsyntax-only ./t.cpp -I ../include/
```

## 内部参考 (Internal References)

`class_ana` 的实现参考了 `clang/tools/clang-check/ClangCheck.cpp` 的设计。
The implementation of `class_ana` refers to the design of `clang/tools/clang-check/ClangCheck.cpp`.