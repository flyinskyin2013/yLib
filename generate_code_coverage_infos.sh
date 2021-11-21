#!/bin/bash

###
 # @Author: Sky
 # @Date: 2021-08-27 11:25:20
 # @LastEditors: Sky
 # @LastEditTime: 2021-11-23 15:01:17
 # @Description: 
### 

if [ $# -eq 1 ]
then
    if [ $1 = "rebuild" ]
    then

        rm -rf build_codecoverage/

        mkdir build_codecoverage
    else
        echo "./xxx.sh rebuild"
        exit -1
    fi
fi


cd build_codecoverage

WORK_DIR=`pwd`

cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/linux_x64_gcc.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=`pwd`/install -DBUILD_YLIB_ARCH=x86_64 -DENABLE_YLIB_COVERAGE=ON -DBUILD_STATIC_YLIB=OFF ..

make -j2

make install

cd install/tests
# clean old log-txt
rm ./yLogBackupTest*
rm ./LogFile*


./scripts_for_tests.sh CodeCoverage

cd ${WORK_DIR}

lcov -d ./src -c -o lcov.info 
lcov -r lcov.info '/usr/*' -o lcov.info 
lcov -r lcov.info '*include/third_part*' -o lcov.info 
genhtml -o cover_report --legend --title "lcov"  --prefix=./ lcov.info 
