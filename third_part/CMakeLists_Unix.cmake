if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
message(FATAL_ERROR  "must set -DCMAKE_TOOLCHAIN_FILE")
endif ()

if (NOT DEFINED BUILD_YLIB_ARCH)
message(FATAL_ERROR  "must set -DBUILD_YLIB_ARCH")
endif ()

set(CMAKE_BUILD_TYPE Release)

# tools
set(MY_CC "${CMAKE_C_COMPILER}")
set(MY_CXX "${CMAKE_CXX_COMPILER}")
# assembler
set(MY_AS "")
set(MY_LD "")
set(MY_AR "")
set(MY_RANLIB "")
set(MY_STRIP "")

# tools flag
set(MY_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
set(MY_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")


set(MY_CONFIGURE_ARGS CC=${MY_CC} CXX=${MY_CXX} CFLAGS=${MY_C_FLAGS} CPPFLAGS=${MY_CXX_FLAGS})
set(MY_CONFIGURE_HOST "")


set(THIRD_PARTY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(MY_CMAKE_ARGS -DCMAKE_C_FLAGS=${MY_C_FLAGS} -DCMAKE_CXX_FLAGS=${MY_CXX_FLAGS} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}  -DBUILD_YLIB_ARCH=${BUILD_YLIB_ARCH})


if(${BUILD_YLIB_ARCH} MATCHES "(aarch64)")
set(MY_CONFIGURE_HOST "aarch64-linux-gnu")
endif()

if(${BUILD_YLIB_ARCH} MATCHES "(armgnueabi)")
set(MY_CONFIGURE_HOST "armgnueabi-linux-gnu")
endif()

if(${BUILD_YLIB_ARCH} MATCHES "(armgnueabihf)")
set(MY_CONFIGURE_HOST "armgnueabihf-linux-gnu")
endif()

if(${BUILD_YLIB_ARCH} MATCHES "(android|Android|ANDROID)")

    if (NOT DEFINED ANDROID_ABI)
    message(FATAL_ERROR  "must set -DANDROID_ABI")
    endif ()

    if (NOT DEFINED ANDROID_PLATFORM)
    message(FATAL_ERROR  "must set -DANDROID_PLATFORM")
    endif ()

    set(TARGET_ABI "aarch64-linux-android")
    set(API_LEVEL "21")

    set(MY_CC "${ANDROID_TOOLCHAIN_ROOT}/bin/${TARGET_ABI}${API_LEVEL}-clang --target=aarch64-linux-android21")
    set(MY_CXX "${ANDROID_TOOLCHAIN_ROOT}/bin/${TARGET_ABI}${API_LEVEL}-clang++ --target=aarch64-linux-android21")
    set(MY_AR "${ANDROID_TOOLCHAIN_ROOT}/bin/llvm-ar")
    set(MY_LD "${ANDROID_TOOLCHAIN_ROOT}/bin/ld")
    set(MY_AS "${TOOLCHAIN_DIR}/bin/${TARGET_ABI}-as")
    set(MY_STRIP "${TOOLCHAIN_DIR}/bin/${TARGET_ABI}-strip")
    set(MY_RANLIB "${TOOLCHAIN_DIR}/bin/${TARGET_ABI}-ranlib")


    message(STATUS "ANDROID_TOOLCHAIN_ROOT = ${ANDROID_TOOLCHAIN_ROOT}")
    message(STATUS "ANDROID_ABI = ${ANDROID_ABI}")
    message(STATUS "ANDROID_PLATFORM = ${ANDROID_PLATFORM}")


    set(MY_C_FLAGS "${MY_C_FLAGS} -Wl,--undefined-version")
    set(MY_CONFIGURE_HOST "aarch64-linux-android")

    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/remove_werror_for_android.cmake)


    set(MY_CMAKE_ARGS ${MY_CMAKE_ARGS} -DANDROID_ABI=${ANDROID_ABI} -DANDROID_PLATFORM=${ANDROID_PLATFORM} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})
    set(MY_CONFIGURE_ARGS "${MY_CONFIGURE_ARGS} --host=aarch64-linux-android-")
endif()

message(STATUS "MY_CC = ${MY_CC}")
message(STATUS "MY_CXX = ${MY_CXX}")
message(STATUS "MY_AS = ${MY_AS}")
message(STATUS "MY_LD = ${MY_LD}")
message(STATUS "MY_AR = ${MY_AR}")
message(STATUS "MY_RANLIB = ${MY_RANLIB}")
message(STATUS "MY_STRIP = ${MY_STRIP}")
message(STATUS "MY_C_FLAGS = ${MY_C_FLAGS}")
message(STATUS "MY_CXX_FLAGS = ${MY_CXX_FLAGS}")

message(STATUS "MY_CMAKE_ARGS = ${MY_CMAKE_ARGS}")
message(STATUS "MY_CONFIGURE_ARGS = ${MY_CONFIGURE_ARGS}")

# JSONCPP
ExternalProject_Add(
    jsoncpp
    URL https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz
    URL_HASH MD5=fa47a3ab6b381869b6a5f20811198662
    PATCH_COMMAND
        bash -c [=[
                echo 1&&sed -i "s/-Werror=conversion//g" <SOURCE_DIR>/CMakeLists.txt&&sed -i "s/JSONCPP_WITH_TESTS \"Compile and (for jsoncpp_check) run JsonCpp test executables\" ON/JSONCPP_WITH_TESTS \"Compile and (for jsoncpp_check) run JsonCpp test executables\" OFF/g" <SOURCE_DIR>/CMakeLists.txt]=]
        
        # sed -i "s/-Werror=conversion//g" <SOURCE_DIR>/CMakeLists.txt
        # sed -i "s/JSONCPP_WITH_TESTS \"Compile and (for jsoncpp_check) run JsonCpp test executables\" ON/JSONCPP_WITH_TESTS \"Compile and (for jsoncpp_check) run JsonCpp test executables\" OFF/g" <SOURCE_DIR>/CMakeLists.txt
        CMAKE_ARGS ${MY_CMAKE_ARGS}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

 # CURL
ExternalProject_Add(
    curl
    URL https://github.com/curl/curl/releases/download/curl-7_55_1/curl-7.55.1.tar.gz
    URL_HASH MD5=3b832160a8c9c40075fd71191960307c
    CONFIGURE_COMMAND  <SOURCE_DIR>/configure --prefix=${THIRD_PARTY_INSTALL_DIR} --without-libidn2 --without-ssl --without-zlib --without-librtmp --disable-rtsp --disable-ldap --disable-ldaps --host=${MY_CONFIGURE_HOST}  CC=${MY_CC} CFLAGS=${MY_C_FLAGS} LD=${LD_COMPILER} AR=${AR_COMPILER}
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# LIBXML2
ExternalProject_Add(
    libxml2
    URL https://github.com/GNOME/libxml2/archive/v2.9.9.tar.gz
    URL_HASH MD5=435110d1f26268f1952350b344a3a934
    CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh --prefix=${THIRD_PARTY_INSTALL_DIR} --with-python=no  --with-lzma=no --with-zlib=no  --host=${MY_CONFIGURE_HOST}  CC=${MY_CC} CFLAGS=${MY_C_FLAGS} LD=${LD_COMPILER} AR=${AR_COMPILER}
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
