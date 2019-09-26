#!/bin/bash

function clean(){

	echo "clean curl-7.55.1/ libxml2-2.7.1/ log4cpp/ curl-7.55.1/ libconfig-1.7.2/  ... ..."
	rm -rf rm curl-7.55.1/ libxml2-2.7.1/ log4cpp/ curl-7.55.1/  libconfig-1.7.2/

}

third_part_root_dir=$(cd `dirname $0`; pwd)

function init_lib(){

	tar -xvf curl-7.55.1.tar.gz
	tar -xvf libxml2-2.7.1.tar.gz
	tar -xvf libconfig-1.7.2.tar.gz
	tar -xvf log4cpp-1.1.3.tar.gz

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
	
	./configure --prefix=${third_part_root_dir}/build_out --without-librtmp --disable-rtsp --disable-ldap --disable-ldaps   CFLAGS=-fPIC CPPFLAGS=-fPIC
	if [ $? -ne 0 ]
	then

		exit 1
	fi

	make -j8

	make install

	echo -e "\033[1;32;40m building libcurl end ...  \033[0m"
}

function build_libxml(){

	echo -e "\033[1;32;40m building libxml start ...  \033[0m"

	cd libxml2-2.7.1

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

	echo "update scripts of config.guess and config.sub for tx2-board " 
	cp ../config.guess .
	cp ../config.sub .

 
	./configure --prefix=${third_part_root_dir}/build_out  CFLAGS=-fPIC CPPFLAGS=-fPIC
	if [ $? -ne 0 ]
	then
		
		exit 1
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
	
	cmake -DCMAKE_INSTALL_PREFIX=${third_part_root_dir}/build_out -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC  -DCMAKE_C_FLAGS=-std=c99  -DCMAKE_CXX_FLAGS=-std=c++11 -DBUILD_SHARED_LIBS=OFF ..
	if [ $? -ne 0 ]
	then 
	
		exit 1
	fi 

	#./configure --prefix=${third_part_root_dir}/build_out  CFLAGS=-fPIC CPPFLAGS=-fPIC


	make -j8

	make install

	echo -e "\033[1;32;40m building libconfig end ...  \033[0m"
}


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


	./configure --prefix=${third_part_root_dir}/build_out  CFLAGS=-fPIC CPPFLAGS=-fPIC
	if [ $? -ne -0 ]
	then 
	
		exit 1
	fi


	make -j8

	make install

	echo -e "\033[1;32;40m building liblog4cpp end ...  \033[0m"
}

function make_lib(){


	cd ${third_part_root_dir}
	build_libcurl
	if [ $? -ne 0 ]
	then
		echo "build libcurl failed."
		exit 1
	fi


	cd ${third_part_root_dir}
	build_libxml
	if [ $? -ne 0 ]
	then
        	echo "build libcurl failed."
		exit 1
	fi


	cd ${third_part_root_dir}
	build_libconfig
	if [ $? -ne 0 ]
	then
		echo "build libcurl failed."
		exit 1
	fi



	cd ${third_part_root_dir}
	build_liblog4cpp
	if [ $? -ne 0 ]
	then
		echo "build libcurl failed."
		exit 1
	fi

}

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


