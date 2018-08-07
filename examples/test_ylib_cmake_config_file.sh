#!/bin/bash


mkdir -p test_ylib_cmake_config_file
cp CMakeLists.txt.example test_ylib_cmake_config_file/CMakeLists.txt
cd test_ylib_cmake_config_file
cmake .
