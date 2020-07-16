#!/bin/bash

#echo -e "\033[字背景颜色;文字颜色m ${str} \033[0m"

#字体颜色：30m-37m 黑、红、绿、黄、蓝、紫、青、白
#echo -e "\033[30m ${str}\033[0m"      ## 黑色字体
#echo -e "\033[31m ${str}\033[0m"      ## 红色
#echo -e "\033[32m ${str}\033[0m"      ## 绿色
#echo -e "\033[33m ${str}\033[0m"      ## 黄色
#echo -e "\033[34m ${str}\033[0m"      ## 蓝色
#echo -e "\033[35m ${str}\033[0m"      ## 紫色
#echo -e "\033[36m ${str}\033[0m"      ## 青色
#echo -e "\033[37m ${str}\033[0m"      ## 白色

#背景颜色：40-47 黑、红、绿、黄、蓝、紫、青、白
#echo -e "\033[41;37m ${str} \033[0m"     ## 红色背景色，白色字体
#echo -e "\033[41;33m ${str} \033[0m"     ## 红底黄字
#echo -e "\033[1;41;33m ${str} \033[0m"   ## 红底黄字 高亮加粗显示
#echo -e "\033[5;41;33m ${str} \033[0m"   ## 红底黄字 字体闪烁显示
#echo -e "\033[47;30m ${str} \033[0m"     ## 白底黑字
#echo -e "\033[40;37m ${str} \033[0m"     ## 黑底白字

#　　\033[1;m 设置高亮加粗
#　　\033[4;m 下划线
#　　\033[5;m 闪烁


function self_print(){

	case $1 in
		"I")
			echo -e "\033[1;4;40;32m I :> ${2} \033[0m"     
		       	;;	
		"W")
			echo -e "\033[1;4;40;33m W :> ${2} \033[0m"      
			;;
		"E")
			
			echo -e "\033[1;4;40;31m E :> ${2} \033[0m"   
		     	;;	
		*)
			echo -e "\033[1;4;40;37m D :> ${2} \033[0m"
			;;
	esac

}

ROOT_DIR=`pwd`
THIRD_PART_DIR=${ROOT_DIR}/third_part

self_print I "Welcome yLib build dependence."
echo ""

function check_download_third_part(){

	self_print I "Now,we will check and download third-part resources.please wait ... ..."

	###############################################################################
	cd ${THIRD_PART_DIR}
	if [ -f curl-7.55.1.tar.gz ];
	then
		#check md5
		md5sum -c curl-7.55.1.md5
	else
		wget -O curl-7.55.1.tar.gz https://github.com/curl/curl/releases/download/curl-7_55_1/curl-7.55.1.tar.gz
	fi

	if [ -f libxml2-2.9.9.tar.gz ];
	then
		md5sum -c libxml2-2.9.9.md5
	else
		wget -O libxml2-2.9.9.tar.gz https://github.com/GNOME/libxml2/archive/v2.9.9.tar.gz
	fi

	if [ -f libconfig-1.7.2.tar.gz ];
	then
		md5sum -c libconfig-1.7.2.md5
	else
		wget -O libconfig-1.7.2.tar.gz https://github.com/hyperrealm/libconfig/archive/v1.7.2.tar.gz
	fi

	if [ -f jsoncpp_1.8.4.tar.gz ];
	then
		md5sum -c jsoncpp_1.8.4.md5
	else
		wget -O jsoncpp_1.8.4.tar.gz https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz
	fi

	if [ -f log4cpp-1.1.3.tar.gz ];
	then
		md5sum -c log4cpp-1.1.3.md5
	else
		wget -O log4cpp-1.1.3.tar.gz https://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.3.tar.gz/download
	fi

	self_print I "Download complete."

}
###############################################################################
###############################################################################
###############################################################################
###############################################################################
function copy_prepared_file(){

	cd ${THIRD_PART_DIR}

	self_print I "Now,we will copy some files, please wait ... ..."
	platform_type=""
	while [ 1 -eq 1 ]
	do
		self_print NORMAL "Please input build platform[linux/windows/android]"
		read -p "platform:" platform_type
		case ${platform_type} in
			"linux")
				break;
				;;
			"windows")
				break;
				;;
			"android")
				break;
				;;
				*)
				self_print E "Input build platform type error."
				;;
		esac
	done

	arch_type=""
	while [ 1 -eq 1 ]
	do
		self_print NORMAL "Please input build arch[x86/x86_64/armeabi/armeabi-v7a/arm64-v8a]"

		read -p "build arch:" arch_type
		case ${arch_type} in
			"x86")
				break;
				;;
			"x86_64")
				break;
				;;
			"armeabi")
				break;
				;;
			"armeabi-v7a")
				break;
				;;
			"arm64-v8a")
				break;
				;;
				*)
				self_print E "Input build arch type error."
				;;
		esac
	done
    self_print I "BuildArchIs:$arch_type"	
	
	mkdir -p ${ROOT_DIR}/include/third_part/$platform_type/
	mkdir -p ${ROOT_DIR}/include/third_part/$platform_type/libconfig

	mkdir -p ${ROOT_DIR}/lib/$platform_type/$arch_type

	cp build_out/include/* ${ROOT_DIR}/include/third_part/$platform_type/ -r
	cp build_out/lib/* ${ROOT_DIR}/lib/$platform_type/$arch_type -r

	cd ${ROOT_DIR}/include/third_part/$platform_type/
	cp libconfig.* libconfig/
	ln -s libxml2/libxml/ libxml

	cd ${ROOT_DIR}
	self_print I "Copy complete."
	
}

case $1 in
	"Download")
		check_download_third_part
		;;
	"Copy")
		copy_prepared_file
		;;
	*)
		echo "Help info:"
		echo "You can pass those argument(Download, Copy) to scripts"
		echo ""
		echo "Example:"
		echo "./build_deps.sh Download      ||   ./build_deps.sh     Copy"
esac


self_print I "Build dependence complete."
