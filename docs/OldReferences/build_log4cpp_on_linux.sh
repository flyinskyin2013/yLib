###
 # @Author: Sky
 # @Date: 2021-09-04 16:33:24
 # @LastEditTime: 2021-09-04 16:33:24
 # @LastEditors: Sky
 # @Description: 
 # @FilePath: \yLib\docs\OldReferences\build_log4cpp_on_linux.sh
### 
function build_liblog4cpp(){

	echo -e "\033[1;32;40m building liblog4cpp start ...  \033[0m"

	cd log4cpp

	if [  ! -d 'build' ]
	then
		echo -e "\033[0;33;40m create dir --- > build ...  \033[0m"
		mkdir build
	fi

	if [  ! -d '_install' ]
	then
		echo -e "\033[0;33;40m create dir --- > _install ...  \033[0m"
		mkdir _install
	fi

	
	echo "update scripts of config.guess and config.sub for tx2-board " 
	cp ../config.guess ./config/
	cp ../config.sub ./config/



	if [ ${Default_Arch} = "x86" ] || [ ${Default_Arch} = "x86_64" ]
	then

		if [ ${Default_Arch} = "x86" ]
		then
			mv tests/Makefile.in tests/Makefile.in.bak
			touch tests/Makefile.in
		fi

		./configure --prefix=${third_part_root_dir}/build_out  CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}" --enable-shared=no --disable-remote-syslog --disable-smtp 
		
		if [ $? -ne -0 ]
		then 
		
			exit 1
		fi

		if [ ${Default_Arch} = "x86" ]
		then
			
			echo "all:" > tests/Makefile
		fi

	else

		# in cross compile mode

		mv tests/Makefile.in tests/Makefile.in.bak
		touch tests/Makefile.in


		./configure --prefix=${third_part_root_dir}/build_out  CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}" --enable-shared=no --disable-remote-syslog --disable-smtp --host="${CONFIGURE_HOST}"  CC="${CONFIGURE_CC}" CXX="${CONFIGURE_CXX}"
		
		if [ $? -ne -0 ]
		then 
		
			exit 1
		fi

		echo "all:" > tests/Makefile

	fi


	make -j8

	make install

	echo -e "\033[1;32;40m building liblog4cpp end ...  \033[0m"
}