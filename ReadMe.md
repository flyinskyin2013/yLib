yLib
==========
This lib collect and collate usage of common skills in my daily-work.



Version Notes:
==========
v0.02
----------
I reconstructed the framework of compiltion and test, and relized ylog-module yshell-module yxml-module  

v0.01
----------
It preliminary relized ycurl-module and establish a framework of compiltion and test.


Module Lists:
==========
ycurl-module<br> 
ylog-module<br> 
yshell-module<br> 
yxml-module<br> 

Dependences:
==========
libcurl-7.55.1<br> 
libxml2-2.7.1<br> 
liblog4cpp-1.1.3<br> 



Install:
==========
run the following commmands.

>>mkdir -p build 

>>cd build && cmake ..

>>make -j4 && make install

Default,In yLib/build/install dir , it may have those dir including cmake , examples , include , lib and so on. You can use it by following way:
1. In a cmake project,you can add following code to your CMakeLists.txt  to import yLib.Then,those variables (yLib_INCLUDE_DIRS,yLib_LIBS,yLib_FOUND and yLib_LIBS_DIR ) will be defined, you can use them in your CMakeLists.txt.
>>set(yLib_DIR ylib_install_dir/cmake)
>>find_package(yLib REQUIRED)

2. In other project,you can add compiler flags to include ylib_install_dir/include and add linker flags to link ylib_install_dir/lib/libylib.so.For example:
>> gcc  -I ylib_install_dir/include -L ylib_install_dir/lib -lylib
