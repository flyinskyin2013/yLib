:: @Description: 
:: @Author: Sky
:: @Date: 2019-11-28 14:37:25
:: @LastEditors: Sky
:: @LastEditTime: 2019-11-28 15:09:12
:: @FilePath: \yLib\third_part\build_third_part.bat
:: @Github: https://github.com/flyinskyin2013/yLib
::

::@%comspec% /k "D:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

echo off

@echo Start build third part ... ...
@set THIRD_PART_ROOT_PATH=%cd%
@echo THIRD_PART_ROOT_PATH = %THIRD_PART_ROOT_PATH%
@set BUILD_TYPE="Release"
@echo BUILD_TYPE = %BUILD_TYPE%


set Default_Arch="x86_64"
set Default_Platform="windows"
set Default_CompileSet="vs2015"
set compile_tool_set="v140"


REM echo %~1%
REM echo %~2%
set arg1=%~1%
set arg2=%~2%


if "%arg1%"=="vs2015" (

    set Default_CompileSet="vs2015"
    set compile_tool_set="v140"
)^
else if "%arg1%"=="vs2017" (

    set Default_CompileSet="vs2017"
    set compile_tool_set="v141"
)^
else if "%arg1%"=="vs2019" (

    set Default_CompileSet="vs2019"
    set compile_tool_set="v142"
)^
else if "%arg1%"=="vs2022" (

    set Default_CompileSet="vs2022"
    set compile_tool_set="v143"
)^
else (
    echo "Notice: we need two param. Those are CompileSet(vs2015(default)\vs2017\vs2019) and Arch(x86\x86_64(default))"
    echo "Example: build_deps.bat vs2015 x86_64"
    call:self_print I "Build third-part dependence complete."
    @goto:eof
)

if "%arg2%"=="x86" (

    set Default_Arch="x86"
    set compile_tool_arch="Win32"
)^
else if "%arg2%"=="x86_64" (

    set Default_Arch="x86_64"
    set compile_tool_arch="x64"
)^
else (
    echo "Notice: we need two param. Those are CompileSet(vs2015(default)\vs2017\vs2019) and Arch(x86\x86_64(default))"
    echo "Example: build_deps.bat vs2015 x86_64"
    call:self_print I "Build third-part dependence complete."
    @goto:eof
)

call:clean
call:init
call:make




goto:eof






:: function defines

:init

    @mkdir build_out

    @echo extract libxml2 ... ...
    @tar -xzf libxml2-2.9.9.tar.gz

    @echo extract libcurl ... ...
    @tar -xzf curl-7.55.1.tar.gz

    @echo extract jsoncpp ... ...
    @tar -xzf jsoncpp_1.8.4.tar.gz
@goto:eof

:make

call:build_libcurl

@REM libxml2 must build after any other-lib, because it needs build_out/include and build_out/lib
call:build_libxml299
call:build_jsoncpp

call:self_print I "Build dependence complete."

@goto:eof

:clean
    
    @echo remove libxml2-2.9.9 ... ...
    @rm -rf libxml2-2.9.9

    @echo remove curl-7.55.1 ... ...
    @rm -rf curl-7.55.1

    @echo remove jsoncpp-1.8.4 ... ...
    @rm -rf jsoncpp-1.8.4

    rm -rf build_out
@goto:eof


:build_libcurl

    @echo building curl-7.55.1 ... ...
    @cd %THIRD_PART_ROOT_PATH%/curl-7.55.1
    @mkdir build_msvc
    @cd build_msvc
    cmake -T %compile_tool_set%,host=x64 -A %compile_tool_arch% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%THIRD_PART_ROOT_PATH%\build_out -DHTTP_ONLY=ON -DBUILD_TESTING=OFF -DCURL_STATICLIB=ON -DBUILDING_LIBCURL=1 ..
    cmake --build . --config Release --target install

@goto:eof

:build_libxml299

    @echo building libxml2-2.9.9 ... ...
    @cd %THIRD_PART_ROOT_PATH%/libxml2-2.9.9/win32

    @cscript configure.js compiler=msvc  incdir=%THIRD_PART_ROOT_PATH%\build_out\include libdir=%THIRD_PART_ROOT_PATH%\build_out\lib static=yes iconv=no
    @nmake /f Makefile.msvc 
    @nmake /f Makefile.msvc install 
@goto:eof

:build_jsoncpp

    @echo building jsoncpp-1.8.4 ... ...
    @cd %THIRD_PART_ROOT_PATH%/jsoncpp-1.8.4
    @mkdir build_msvc
    @cd build_msvc
    cmake -T %compile_tool_set%,host=x64 -A %compile_tool_arch% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%THIRD_PART_ROOT_PATH%\build_out ..
    cmake --build . --config Release --target install

@goto:eof


:: echo -e "\033[字背景颜色;文字颜色m ${str} \033[0m"

:: 字体颜色：30m-37m 黑、红、绿、黄、蓝、紫、青、白
:: echo -e "\033[30m ${str}\033[0m"      ## 黑色字体
:: echo -e "\033[31m ${str}\033[0m"      ## 红色
:: echo -e "\033[32m ${str}\033[0m"      ## 绿色
:: echo -e "\033[33m ${str}\033[0m"      ## 黄色
:: echo -e "\033[34m ${str}\033[0m"      ## 蓝色
:: echo -e "\033[35m ${str}\033[0m"      ## 紫色
:: echo -e "\033[36m ${str}\033[0m"      ## 青色
:: echo -e "\033[37m ${str}\033[0m"      ## 白色
:: 
:: 背景颜色：40-47 黑、红、绿、黄、蓝、紫、青、白
:: echo -e "\033[41;37m ${str} \033[0m"     ## 红色背景色，白色字体
:: echo -e "\033[41;33m ${str} \033[0m"     ## 红底黄字
:: echo -e "\033[1;41;33m ${str} \033[0m"   ## 红底黄字 高亮加粗显示
:: echo -e "\033[5;41;33m ${str} \033[0m"   ## 红底黄字 字体闪烁显示
:: echo -e "\033[47;30m ${str} \033[0m"     ## 白底黑字
:: echo -e "\033[40;37m ${str} \033[0m"     ## 黑底白字
:: 
:: 　　\033[1;m 设置高亮加粗
:: 　　\033[4;m 下划线
:: 　　\033[5;m 闪烁


:self_print

if "%1" == "I" ( 
    echo I ^:^> %~2
) ^
else if "%1" == "w" ( 
    echo W ^:^> %~2
) ^
else if "%1" == "E" ( 
    echo E ^:^> %~2
) ^
else ( 
    echo I ^:^> %~2
)
@goto:eof