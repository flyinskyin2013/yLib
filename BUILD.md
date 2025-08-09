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
mkdir build_win_x64


mkdir build_win_x86
```