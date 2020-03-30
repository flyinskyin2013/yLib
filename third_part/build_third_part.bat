:: @Description: 
:: @Author: Sky
:: @Date: 2019-11-28 14:37:25
:: @LastEditors: Sky
:: @LastEditTime: 2019-11-28 15:09:12
:: @FilePath: \yLib\third_part\build_third_part.bat
:: @Github: https://github.com/flyinskyin2013/yLib
::

::@%comspec% /k "D:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

@echo Start build third part ... ...
@set THIRD_PART_ROOT_PATH=%cd%
@echo THIRD_PART_ROOT_PATH = %THIRD_PART_ROOT_PATH%
@set BUILD_TYPE="debug"
@echo BUILD_TYPE = %BUILD_TYPE%

@if "%1%"=="init" (

    call:init
    @goto:eof
)^
else if "%1%"=="clean" (

    call:clean
    @goto:eof
)^
else if "%1%"=="make" (

    call:make
    @goto:eof
)^
else (
    echo we need a param, such as init, clean, make 
    @goto:eof
)






:: function defines

:init

    @mkdir build_out
    @echo extract log4cpp ... ...
    @tar -xzf log4cpp-1.1.3.tar.gz
    @echo extract jsoncpp ... ...
    @tar -xzf jsoncpp_1.8.4.tar.gz
    @echo extract libconfig ... ...
    @tar -xzf libconfig-1.7.2.tar.gz
@goto:eof

:make

call:build_log4cpp
call:build_jsoncpp
call:build_libconfig

@goto:eof

:clean

    @echo remove log4cpp ... ...
    @rm -rf log4cpp

    @echo remove jsoncpp-1.8.4 ... ...
    @rm -rf jsoncpp-1.8.4

    @echo remove libconfig-1.7.2 ... ...
    @rm -rf libconfig-1.7.2
@goto:eof


:build_jsoncpp

    @echo building jsoncpp-1.8.4 ... ...
    @cd %THIRD_PART_ROOT_PATH%/jsoncpp-1.8.4
    @mkdir build_vs2015
    @cd build_vs2015
    cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%THIRD_PART_ROOT_PATH%/build_out ..
    nmake 
    nmake install

@goto:eof


:build_log4cpp

    @echo building log4cpp ... ...
    @cd %THIRD_PART_ROOT_PATH%/log4cpp
    @mkdir build_vs2015
    @cd build_vs2015
    cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%THIRD_PART_ROOT_PATH%/build_out ..
    nmake 
    nmake install

@goto:eof

:build_libconfig

    @echo building libconfig-1.7.2 ... ...
    @cd %THIRD_PART_ROOT_PATH%/libconfig-1.7.2
    @echo Patching cmake files ... ...
    @patch -p1 < ../libconfig_cmake.patch
    @mkdir build_vs2015
    @cd build_vs2015
    cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%THIRD_PART_ROOT_PATH%/build_out ..
    nmake 
    nmake install

@goto:eof

