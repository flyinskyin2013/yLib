#!/bin/bash
set -e

REPOSITORY_ROOT_DIR=""
CURRENT_FILE_NAME=`basename $0`
BUILD_NUM=0
BUILD_VERSION="V1.0.0"
# all/cxx/python/android
BUILD_TARGET="all"
BUILD_HOST="ubuntu1804"
BUILD_ARCH="aarch64"

TEMP=`getopt -o b:r:v:t:h --long rep_root_dir:,build_num:,build_version:,build_target:,build_host:,build_arch:,help -n ${CURRENT_FILE_NAME} -- "$@"`
if [ $? != 0 ] ; then echo "build_base.sh run error, please run : ${CURRENT_FILE_NAME} -h see more info." >&2 ; exit 1 ; fi

eval set -- "$TEMP"
while true ; do
        case "$1" in
                -r|--rep_root_dir) REPOSITORY_ROOT_DIR=${2} ; shift 2;;
                -b|--build_num) BUILD_NUM=${2}; shift 2;;
                -h|--help) echo "call help ... ..." ; shift 2 ;;
                -v|--build_version) BUILD_VERSION=${2} ; shift 2 ;;
                -t|--build_target) BUILD_TARGET=${2} ; shift 2 ;;
                --build_host) BUILD_HOST=${2} ; shift 2 ;;
                --build_arch) BUILD_ARCH=${2} ; shift 2 ;;
                --) shift ; break ;;
                *) echo "Invalid param(${1}) for ${CURRENT_FILE_NAME}!" ; exit 1 ;;
        esac
done

if [ "${REPOSITORY_ROOT_DIR}" == "" ]
then
        echo "need -r|--rep_root_dir param."
        echo "call help ... ..."
        exit 1
fi


# PrintLog TAG Msg
function PrintLog(){

    cur_date=`date`
    echo "[${cur_date} ${1}]: "$2
}


PrintLog ${CURRENT_FILE_NAME} "CURRENT_FILE_NAME=${CURRENT_FILE_NAME}"
PrintLog ${CURRENT_FILE_NAME} "REPOSITORY_ROOT_DIR=${REPOSITORY_ROOT_DIR}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_NUM=${BUILD_NUM}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_VERSION=${BUILD_VERSION}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_TARGET=${BUILD_TARGET}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_HOST=${BUILD_HOST}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_ARCH=${BUILD_ARCH}"

#default switch to workspace dir
cd ${REPOSITORY_ROOT_DIR}

