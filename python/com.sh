#!/bin/bash
###
 # @Author: Sky
 # @Date: 2021-09-13 16:45:33
 # @LastEditors: Sky
 # @LastEditTime: 2021-09-13 17:32:27
 # @Description: 
### 

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) ylib.cpp -o yLib$(python3-config --extension-suffix) -I$(python3-config --includes) -I../include
