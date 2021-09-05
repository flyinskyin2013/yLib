:: @Description: 
:: @Author: Sky
:: @Date: 2019-11-28 14:37:25
:: @LastEditors: Sky
:: @LastEditTime: 2019-11-28 15:09:12
:: @FilePath: \yLib\build_deps.bat
:: @Github: https://github.com/flyinskyin2013/yLib

@echo off


set ROOT_DIR=%cd%
set THIRD_PART_DIR=%ROOT_DIR%/third_part

call:self_print I "Welcome yLib build dependence."


set Default_Arch="x86_64"
set Default_Platform="windows"
set Default_CompileSet="vs2015"



if "%1%"=="vs2015" (

    set Default_CompileSet="vs2015"
)^
else if "%1%"=="vs2017" (

    set Default_CompileSet="vs2017"
)^
else if "%1%"=="vs2019" (

    set Default_CompileSet="vs2019"
)^
else (
    echo "Notice: we need two param. Those are CompileSet(vs2015(default)\vs2017\vs2019) and Arch(x86\x86_64(default))"
    echo "Example: build_deps.bat vs2015 x86_64"
    call:self_print I "Build dependence complete."
    @goto:eof
)

if "%2%"=="x86" (

    set Default_Arch="x86"
)^
else if "%2%"=="x86_64" (

    set Default_Arch="x86_64"
)^
else (
    echo "Notice: we need two param. Those are CompileSet(vs2015(default)\vs2017\vs2019) and Arch(x86\x86_64(default))"
    echo "Example: build_deps.bat vs2015 x86_64"
    call:self_print I "Build dependence complete."
    @goto:eof
)

call:self_print I "CompileSet is %Default_CompileSet%"
call:self_print I "Arch is %Default_Arch%"

cd %ROOT_DIR%
call:check_download_third_part

cd %ROOT_DIR%
call:build_third_part

cd %ROOT_DIR%
call:copy_prepared_file



cd %ROOT_DIR%

call:self_print I "Build dependence complete."

@goto:eof




:: function defines
:check_download_third_part

	call:self_print I "Now,we will check and download third-part resources.please wait ... ..."

	::###############################################################################
	cd %THIRD_PART_DIR%
	if exist curl-7.55.1.tar.gz (
        REM check md5
		REM md5sum -c curl-7.55.1.md5
        call:self_print I "check md5 for curl-7.55.1.tar.gz"
    )^
    else (

        wget -O curl-7.55.1.tar.gz https://github.com/curl/curl/releases/download/curl-7_55_1/curl-7.55.1.tar.gz
    )

	if exist libxml2-2.9.9.tar.gz (
        REM check md5
		REM md5sum -c libxml2-2.9.9.md5
        call:self_print I "check md5 libxml2-2.9.9.tar.gz"
    )^
    else (

        wget -O libxml2-2.9.9.tar.gz https://github.com/GNOME/libxml2/archive/v2.9.9.tar.gz
    )

    if exist libconfig-1.7.2.tar.gz (
        REM check md5
		REM md5sum -c libconfig-1.7.2.md5
        call:self_print I "check md5 for libconfig-1.7.2.tar.gz"
    )^
    else (

        wget -O libconfig-1.7.2.tar.gz https://github.com/hyperrealm/libconfig/archive/v1.7.2.tar.gz
    )

    if exist jsoncpp_1.8.4.tar.gz (
        REM check md5
		REM md5sum -c jsoncpp_1.8.4.md5
        call:self_print I "check md5 for jsoncpp_1.8.4.tar.gz"
    )^
    else (

        wget -O jsoncpp_1.8.4.tar.gz https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz
    )

	call:self_print I "Download complete."
@goto:eof


:build_third_part

	cd %THIRD_PART_DIR%
	call:self_print I "Now,we will build third-part.please wait ... ..."
	call:self_print W "The compileSet is %Default_CompileSet%"
    call:self_print W "The Default_Arch is %Default_Arch%"


    call build_third_part.bat %Default_CompileSet% %Default_Arch%


	call:self_print I "Build complete."
@goto:eof



:copy_prepared_file

	cd %THIRD_PART_DIR%

	call:self_print I "Now,we will copy some files, please wait ... ..."

	call:self_print I "arch_type is: %Default_Arch%"
	
	mkdir %ROOT_DIR%\include\third_part\%Default_Platform%
	mkdir %ROOT_DIR%\include\third_part\%Default_Platform%\libconfig
	mkdir %ROOT_DIR%\lib\%Default_Platform%\%Default_Arch%

	cp build_out/include/* %ROOT_DIR%/include/third_part/%Default_Platform%/ -r
	cp build_out/lib/* %ROOT_DIR%/lib/%Default_Platform%/%Default_Arch% -r

    REM fix libconfig inc

	cd %ROOT_DIR%/include/third_part/%Default_Platform%/
	cp libconfig.h libconfig.h++ libconfig.hh libconfig/
    mv libxml2/libxml/ libxml
    rm -r libxml2

	REM cd ${ROOT_DIR}
	call:self_print I "Copy complete."

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

