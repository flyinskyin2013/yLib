#!/bin/bash

###
 # @Author: Sky
 # @Date: 2021-04-09 14:22:29
 # @LastEditors: Sky
 # @LastEditTime: 2021-11-11 11:13:06
 # @Description: 
### 

echo 'ylib-unit-tests start ... ...'

EXECUTE_PREFIX=""
CODE_COVERAGE_MODE=0

function Usage()
{
    echo "Usage:"
    echo "example:./scripts_for_tests.sh [qemu-gnueabi]/[CodeCoverage]"
    echo "example:./scripts_for_tests.sh [qemu-gnueabihf]/[CodeCoverage]"
    echo "example:./scripts_for_tests.sh [qemu-aarch64]/[CodeCoverage]"
    exit -1
}

function KillProcessByName()
{   
    process_info=`ps -aux | grep $1 | grep -v grep`
    process_id=`echo ${process_info} | awk '{print $2}'`
    if [ ! ${process_id} = "" ]
    then
        kill -9 ${process_id}
    fi
}

if [ $# -gt 1 ]
then
    Usage
fi

if [ $# -eq 1 ]
then
    if [ $1 = "qemu-gnueabi" ]
    then
        EXECUTE_PREFIX="qemu-arm -L /usr/arm-linux-gnueabi/ "
        echo "In qemu mode .. .. .., EXECUTE_PREFIX is "${EXECUTE_PREFIX}

    elif [ $1 = "qemu-gnueabihf" ]
    then

        EXECUTE_PREFIX="qemu-arm -L /usr/arm-linux-gnueabihf/ "
        echo "In qemu mode .. .. .., EXECUTE_PREFIX is "${EXECUTE_PREFIX}

    elif [ $1 = "qemu-aarch64" ]
    then

        EXECUTE_PREFIX="qemu-aarch64 -L /usr/aarch64-linux-gnu/ "
        echo "In qemu mode .. .. .., EXECUTE_PREFIX is "${EXECUTE_PREFIX}

    elif [ $1 = "CodeCoverage" ]
    then

        CODE_COVERAGE_MODE=1
        echo "In Code Coverage Mode"

    else
        Usage
    fi
fi



# core group
CORE_GROUP_MODULE_LIST=" \
                        yobject \
                        ycommon \
                        yexception \
                        ybasicvalue \
                        yallocator \
                        ylib_extra_infos \
                        "

# ipc group
IPC_GROUP_MODULE_LIST=" "

# network group
NETWORK_GROUP_MODULE_LIST=" "

# utility group 

# UTILITY_GROUP_MODULE_LIST=" \
#                             yconfig \
#                             yhttp \
#                             yjson \
#                             yshell \
#                             ytimer \
#                             yxml \
#                             "
UTILITY_GROUP_MODULE_LIST=" \
                            yhttp \
                            yjson \
                            yshell \
                            ytimer \
                            yxml \
                            "

# basic_algorithm group
# BASIC_ALGORITHM_GROUP_MODULE_LIST=" \
#                                     ylinearlist \
#                                     ylinkedlist \
#                                     yqueue \
#                                     ystack \
#                                     "
BASIC_ALGORITHM_GROUP_MODULE_LIST=" "
                                    
ALL_MODULE_LISTS=${CORE_GROUP_MODULE_LIST}${IPC_GROUP_MODULE_LIST}${UTILITY_GROUP_MODULE_LIST}${BASIC_ALGORITHM_GROUP_MODULE_LIST}" all_in_one"


# special tests
# core group
echo "module name ylog"
rm yLogBackupTest*
rm LogFile*
rm LogFile_test_ylog*

${EXECUTE_PREFIX}./unit_test_ylog -s
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ylog'
    exit -1
fi
sleep 1
${EXECUTE_PREFIX}./unit_test_ylog_file_bakup -s
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ylog_file_bakup'
    exit -1
fi
sleep 1
${EXECUTE_PREFIX}./unit_test_ylog_verify -s
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ylog_verify'
    exit -1
fi
sleep 1
${EXECUTE_PREFIX}./unit_test_ylog_capi -s
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ylog_capi'
    exit -1
fi
sleep 1

# ipc group
echo "module name ysharedmemory"
ipcrm -M 0x123456
${EXECUTE_PREFIX}./unit_test_ysharedmemory_w -d yes
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ysharedmemory_w'
    exit -1
fi
${EXECUTE_PREFIX}./unit_test_ysharedmemory_r -d yes
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:unit_test_ysharedmemory_r'
    exit -1
fi


# network group
KillProcessByName unit_test_ytcpserver
KillProcessByName unit_test_yudpserver

echo "module name ytcpserver/ytcpsocket"
${EXECUTE_PREFIX}./unit_test_ytcpserver -d yes &
sleep 2
${EXECUTE_PREFIX}./unit_test_ytcpclient -d yes
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:ytcpserver/ytcpsocket'
    exit -1
fi

echo "module name yudpsocket"
${EXECUTE_PREFIX}./unit_test_yudpserver -d yes &
sleep 2
${EXECUTE_PREFIX}./unit_test_yudpclient -d yes
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:yudpsocket'
    exit -1
fi

for module_name in ${ALL_MODULE_LISTS}
do
    echo "module name "${module_name}
    ${EXECUTE_PREFIX}./unit_test_${module_name} -d yes
    if [ $? -ne 0 ]
    then
        echo 'Unit test failed. module-name:'${module_name}
        exit -1
    fi
    
    if [ ${CODE_COVERAGE_MODE} -eq 0 ]
    then
        ${EXECUTE_PREFIX}./unit_test_${module_name}_s -d yes
        if [ $? -ne 0 ]
        then
            echo 'Unit test failed. module-name:'${module_name}_s
            exit -1
        fi
    fi
done



