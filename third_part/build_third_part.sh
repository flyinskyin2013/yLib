### 
# @Author: Sky
 # @Date: 2019-10-28 17:35:17
 # @LastEditors: Please set LastEditors
 # @LastEditTime: 2021-09-04 16:33:50
 # @Description: 
 ###
#!/bin/bash

Default_Arch="x86_64"
SELF_C_FLAGS="-fPIC" 
SELF_CXX_FLAGS="-fPIC"

CONFIGURE_HOST=""
CONFIGURE_BUILD=""
CONFIGURE_CC=""
CONFIGURE_CXX=""

DEF_CMAKE_SYSTEM_PROCESSOR="arm"
DEF_CMAKE_C_COMPILER=""
DEF_CMAKE_CXX_COMPILER=""

function clean(){

	echo "clean curl-7.55.1/ libxml2-2.7.1/ curl-7.55.1/ libconfig-1.7.2/ jsoncpp-1.8.4/ ... ..."
	rm -rf curl-7.55.1/ libxml2-2.7.1/  curl-7.55.1/  libconfig-1.7.2/ libxml2-2.9.9/ jsoncpp-1.8.4/

	echo "clean build_out/ ... ..."
	rm -rf build_out/
}

third_part_root_dir=$(cd `dirname $0`; pwd)

function init_lib(){

	tar -xzf curl-7.55.1.tar.gz
	#tar -xvf libxml2-2.7.1.tar.gz
	tar -xzf  libxml2-2.9.9.tar.gz
	tar -xzf libconfig-1.7.2.tar.gz
	tar -xzf jsoncpp_1.8.4.tar.gz

}

if [ ! -d "build_out" ]
then
	mkdir ${third_part_root_dir}/build_out
fi


function build_libcurl(){

	echo -e "\033[1;32;40m building libcurl start ...  \033[0m"

	cd curl-7.55.1

	if [ ! -d 'build' ]
	then
		echo -e "\033[0;33;40m create dir --- > build ...  \033[0m"
		mkdir build
	fi

	if [ !  -d '_install' ]
	then
		echo -e "\033[0;33;40m create dir --- > _install ...  \033[0m"
		mkdir _install
	fi
#cmake  -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/curl-7.55.1/install -DBUILD_CURL_EXE=OFF \
# -DCURL_STATICLIB=ON -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_C_FLAGS=-fPIC -DUSE_LIBRTMP=OFF  ..
# cmakelists.txt may have some issue

	if [ ${Default_Arch} = "x86" ] || [ ${Default_Arch} = "x86_64" ]
	then

		./configure --prefix=${third_part_root_dir}/build_out --without-ssl --without-zlib --without-librtmp --disable-rtsp --disable-ldap --disable-ldaps   CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}"
		if [ $? -ne 0 ]
		then

			exit 1
		fi

	else

		# in cross compile mode
		./configure --prefix=${third_part_root_dir}/build_out --without-ssl --without-zlib --without-librtmp --disable-rtsp --disable-ldap --disable-ldaps --host=${CONFIGURE_HOST}  CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}" CC="${CONFIGURE_CC}" CXX="${CONFIGURE_CXX}"
		if [ $? -ne 0 ]
		then

			exit 1
		fi

	fi

	make -j8

	make install

	echo -e "\033[1;32;40m building libcurl end ...  \033[0m"
}

function build_libxml_2_9_9(){

	echo -e "\033[1;32;40m building libxml start ...  \033[0m"

	cd libxml2-2.9.9

	if [  ! -d 'build' ]
	then
		echo -e "\033[0;33;40m create dir --- > build ...  \033[0m"
		mkdir build
	fi

	if [  ! -d'_install' ]
	then
		echo -e "\033[0;33;40m create dir --- > _install ...  \033[0m"
		mkdir _install
	fi

	#echo "update scripts of config.guess and config.sub for tx2-board " 
	#cp ../config.guess .
	#cp ../config.sub .

 	# ./configure --prefix=xxxx  CFLAGS=-fPIC CPPFLAGS=-fPIC --with-python=no
	# ./configure --prefix=${third_part_root_dir}/build_out  CFLAGS=-fPIC CPPFLAGS=-fPIC
	# autogen.sh note:I am going to run ./configure with no arguments - if you wish to pass any to it, please specify them on the ./autogen.sh command line.


	if [ ${Default_Arch} = "x86" ] || [ ${Default_Arch} = "x86_64" ]
	then

		./autogen.sh --prefix=${third_part_root_dir}/build_out  CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}" --with-python=no  --with-lzma=no --with-zlib=no

		if [ $? -ne 0 ]
		then
			
			exit 1
		fi

	else

		# in cross compile mode
		./autogen.sh --prefix=${third_part_root_dir}/build_out  CFLAGS="${SELF_C_FLAGS}" CPPFLAGS="${SELF_CXX_FLAGS}" --with-python=no  --with-lzma=no --with-zlib=no --host="${CONFIGURE_HOST}"  CC="${CONFIGURE_CC}" CXX="${CONFIGURE_CXX}"
		

		if [ $? -ne 0 ]
		then
			
			exit 1
		fi

	fi

	make -j8

	make install

	echo -e "\033[1;32;40m building libxml end ...  \033[0m"
}

function build_libconfig(){

	echo -e "\033[1;32;40m building libconfig start ...  \033[0m"

	cd libconfig-1.7.2 

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
	
	patch -p1 < ../libconfig_cmake.patch

	
	cd build

	if [ ${Default_Arch} = "x86" ] || [ ${Default_Arch} = "x86_64" ]
	then

		SELF_C_FLAGS=${SELF_C_FLAGS}" -std=c99"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -std=c++11"
		cmake -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/build_out -DCMAKE_C_FLAGS="${SELF_C_FLAGS}" -DCMAKE_CXX_FLAGS="${SELF_CXX_FLAGS}" -DBUILD_SHARED_LIBS=OFF ..
		if [ $? -ne 0 ]
		then 
		
			exit 1
		fi 

	else

		# in cross compile mode
		SELF_C_FLAGS=${SELF_C_FLAGS}" -std=c99"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -std=c++11"
		cmake -DCMAKE_SYSTEM_PROCESSOR=${DEF_CMAKE_SYSTEM_PROCESSOR} -DCMAKE_C_COMPILER=${DEF_CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${DEF_CMAKE_CXX_COMPILER} -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/build_out -DCMAKE_C_FLAGS="${SELF_C_FLAGS}" -DCMAKE_CXX_FLAGS="${SELF_CXX_FLAGS}" -DBUILD_SHARED_LIBS=OFF ..
		if [ $? -ne 0 ]
		then 
		
			exit 1
		fi 

	fi
	#./configure --prefix=${third_part_root_dir}/build_out  CFLAGS=-fPIC CPPFLAGS=-fPIC


	make -j8

	make install

	echo -e "\033[1;32;40m building libconfig end ...  \033[0m"
}


function build_libjsoncpp(){

	echo -e "\033[1;32;40m building libjsoncpp start ...  \033[0m"

	cd jsoncpp-1.8.4

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

	cd build


	if [ ${Default_Arch} = "x86" ] || [ ${Default_Arch} = "x86_64" ]
	then

		SELF_C_FLAGS=${SELF_C_FLAGS}" -std=c99"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -std=c++11"
		cmake -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/build_out -DCMAKE_C_FLAGS="${SELF_C_FLAGS}" -DCMAKE_CXX_FLAGS="${SELF_CXX_FLAGS}" -DBUILD_SHARED_LIBS=OFF ..
		if [ $? -ne -0 ]
		then 
		
			exit 1
		fi

	else

		# in cross compile mode
		SELF_C_FLAGS=${SELF_C_FLAGS}" -std=c99"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -std=c++11"
		cmake -DCMAKE_SYSTEM_PROCESSOR=${DEF_CMAKE_SYSTEM_PROCESSOR} -DCMAKE_C_COMPILER=${DEF_CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${DEF_CMAKE_CXX_COMPILER} -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/build_out -DCMAKE_C_FLAGS="${SELF_C_FLAGS}" -DCMAKE_CXX_FLAGS="${SELF_CXX_FLAGS}" -DBUILD_SHARED_LIBS=OFF -DJSONCPP_WITH_TESTS=OFF ..
		if [ $? -ne -0 ]
		then 
		
			exit 1
		fi

	fi

	make -j8

	make install

	echo -e "\033[1;32;40m building libjsoncpp end ...  \033[0m"
}


function make_lib(){


	cd ${third_part_root_dir}
	build_libcurl
	if [ $? -ne 0 ]
	then
		echo "build libcurl failed."
		exit 1
	fi

## for libxml, you should install libtool autoconf pkgconfig
	cd ${third_part_root_dir}
	build_libxml_2_9_9
	if [ $? -ne 0 ]
	then
        	echo "build libxml failed."
		exit 1
	fi


	cd ${third_part_root_dir}
	build_libconfig
	if [ $? -ne 0 ]
	then
		echo "build libconfig failed."
		exit 1
	fi
	

	cd ${third_part_root_dir}
	build_libjsoncpp
	if [ $? -ne 0 ]
	then
		echo "build jsoncpp failed."
		exit 1
	fi
}


case $2 in
	"x86")
		Default_Arch="x86"
		SELF_C_FLAGS=${SELF_C_FLAGS}" -m32"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -m32"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}
		;;
	"x86_64")
		Default_Arch="x86_64"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}
		;;
	"armeabi")
		Default_Arch="armeabi"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}

		CONFIGURE_HOST="arm-linux-gnueabi"
		CONFIGURE_CC="arm-linux-gnueabi-gcc"
		CONFIGURE_CXX="arm-linux-gnueabi-g++"

		DEF_CMAKE_C_COMPILER="arm-linux-gnueabi-gcc"
		DEF_CMAKE_CXX_COMPILER="arm-linux-gnueabi-g++"
		SELF_C_FLAGS=${SELF_C_FLAGS}" -march=armv7-a -mfloat-abi=softfp -mfpu=neon-vfpv4"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -march=armv7-a -mfloat-abi=softfp -mfpu=neon-vfpv4"
		;;
	"armeabihf")
		Default_Arch="armeabihf"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}
		CONFIGURE_HOST="arm-linux-gnueabihf"
		CONFIGURE_CC="arm-linux-gnueabihf-gcc"
		CONFIGURE_CXX="arm-linux-gnueabihf-g++"

		DEF_CMAKE_C_COMPILER="arm-linux-gnueabihf-gcc"
		DEF_CMAKE_CXX_COMPILER="arm-linux-gnueabihf-g++"
		SELF_C_FLAGS=${SELF_C_FLAGS}" -march=armv7-a -mfloat-abi=hard -mfpu=neon"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -march=armv7-a -mfloat-abi=hard -mfpu=neon"
		;;
	"aarch64")
		Default_Arch="aarch64"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}
		CONFIGURE_HOST="aarch64-linux-gnu"
		CONFIGURE_CC="aarch64-linux-gnu-gcc"
		CONFIGURE_CXX="aarch64-linux-gnu-g++"
		
		DEF_CMAKE_C_COMPILER="aarch64-linux-gnu-gcc"
		DEF_CMAKE_CXX_COMPILER="aarch64-linux-gnu-g++"
		SELF_C_FLAGS=${SELF_C_FLAGS}" -march=armv8-a"
		SELF_CXX_FLAGS=${SELF_CXX_FLAGS}" -march=armv8-a"
		;;
	*)
		Default_Arch="x86_64"
		echo "Notice(build_third_part.sh): set Default_Arch is "${Default_Arch}
		
esac


#clean func
if [ $1 = "clean" ]
then
	clean
	exit 0
fi

#init lib

if [ $1 = "init" ]
then
	init_lib
	exit 0
fi

#make lib
if [ $1 = "make" ]
then
	make_lib	
	exit 0
fi


