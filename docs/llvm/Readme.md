# build
clang++-18 ../docs/llvm/class_ana.cpp -o class_ana -std=c++17 -I /usr/lib/llvm-18/include/ -L /usr/lib/llvm-18/lib/  -lclang-cpp -lLLVM-18 



命令行参数以"--"进行分隔
    ./class_ana ../docs/llvm/class_ana.cpp -- -I /usr/lib/llvm-18/include/
    ./class_ana ../src/core/yobject.cpp -- -I /usr/lib/llvm-18/include/ -I ../include/
    python3.7 ../docs/llvm/class_ana.py --ylib_root=`pwd`/../
    clang++-18 -Xclang -ast-dump -fsyntax-only ./t.cpp -I ../include/


# ref
    clang\tools\clang-check\ClangCheck.cpp