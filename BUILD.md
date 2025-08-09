### build res
- build_android_aarch64/install
- build_linux_aarch64/install
- build_linux_armgnueabi/install
- build_linux_armgnueabihf/install
- build_linux_x64/install
- build_linux_x86/install


### android build
```bash
mkdir build_android_aarch64
export ANDROID_NDK_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake -DANDROID_ABI="arm64-v8a"  -DANDROID_PLATFORM=android-21 -DBUILD_YLIB_ARCH=android -DCMAKE_BUILD_TYPE=Release
```


### linux
```bash
mkdir build_linux_aarch64
export PRJ_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${PRJ_ROOT}/toolchains/linux_aarch64_gcc.cmake -DBUILD_YLIB_ARCH=aarch64 -DCMAKE_BUILD_TYPE=Release
```

```bash
mkdir build_linux_armgnueabi
export PRJ_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${PRJ_ROOT}/toolchains/linux_armgnueabi_gcc.cmake -DBUILD_YLIB_ARCH=armgnueabi -DCMAKE_BUILD_TYPE=Release
```

```bash
mkdir build_linux_armgnueabihf
export PRJ_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${PRJ_ROOT}/toolchains/linux_armgnueabihf_gcc.cmake -DBUILD_YLIB_ARCH=armgnueabihf -DCMAKE_BUILD_TYPE=Release
```

```bash
mkdir build_linux_x86_64
export PRJ_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${PRJ_ROOT}/toolchains/linux_x64_gcc.cmake -DBUILD_YLIB_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release
```

```bash
mkdir build_linux_x86
export PRJ_ROOT=/xxx
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install -DCMAKE_TOOLCHAIN_FILE=${PRJ_ROOT}/toolchains/linux_x64_gcc_m32.cmake -DBUILD_YLIB_ARCH=x86 -DCMAKE_BUILD_TYPE=Release
```


### windows
```bash

# v140/v141/v142/v143
$env:compile_tool_set = "v143"
#set compile_tool_set="v143"

# x64, Win32
$env:compile_tool_arch = "x64"
#set compile_tool_arch="x64"

mkdir build_${env:compile_tool_set}_${env:compile_tool_arch}

cmake -T $env:compile_tool_set,host=x64 -A $env:compile_tool_arch  -DBUILD_YLIB_ARCH=x86_64 -DCMAKE_INSTALL_PREFIX="$env:build_dir\\install" -DCOMPILE_TOOL_SET="$env:compile_tool_set" -DCOMPILE_TOOL_ARCH="$env:compile_tool_arch" ..

cmake --build . --config Release --target install

mkdir build_win_x86
```