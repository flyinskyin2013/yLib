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