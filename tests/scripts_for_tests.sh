#!/bin/bash

###
 # @Author: Sky
 # @Date: 2021-04-09 14:22:29
 # @LastEditors: Please set LastEditors
 # @LastEditTime: 2021-11-07 19:58:38
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
# IPC_GROUP_MODULE_LIST=" ysharedmemory_w \
#                         ysharedmemory_r "
IPC_GROUP_MODULE_LIST=" "

# network group
NETWORK_GROUP_MODULE_LIST=" "

# utility group 

# UTILITY_GROUP_MODULE_LIST=" \
#                             yconfig \
#                             yhttp \
#                             yjson \
#                             ylog \
#                             ylog_file_bakup \
#                             ylog_verify \
#                             yshell \
#                             ytimer \
#                             yxml \
#                             "
UTILITY_GROUP_MODULE_LIST=" \
                            yhttp \
                            yjson \
                            ylog \
                            ylog_file_bakup \
                            ylog_verify \
                            yshell \
                            ytimer \
                            yxml \
                            "

# basic_algorithm group
BASIC_ALGORITHM_GROUP_MODULE_LIST=" \
                                    ylinearlist \
                                    ylinkedlist \
                                    yqueue \
                                    ystack \
                                    "
                                    
ALL_MODULE_LISTS=${CORE_GROUP_MODULE_LIST}${IPC_GROUP_MODULE_LIST}${UTILITY_GROUP_MODULE_LIST}${BASIC_ALGORITHM_GROUP_MODULE_LIST}" all_in_one"



# network group
echo "module name ytcpserver/ytcpsocket"
${EXECUTE_PREFIX}./unit_test_ytcpserver -s &
sleep 2
${EXECUTE_PREFIX}./unit_test_ytcpclient -s
if [ $? -ne 0 ]
then
    echo 'Unit test failed. module-name:ytcpserver/ytcpsocket'
    exit -1
fi

echo "module name yudpsocket"
${EXECUTE_PREFIX}./unit_test_yudpserver -s &
sleep 2
${EXECUTE_PREFIX}./unit_test_yudpclient -s
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


# special tests

# network group
# echo "module name ytcpserver/ytcpsocket"
# ${EXECUTE_PREFIX}./unit_test_ytcpserver -s &
# sleep 2
# ${EXECUTE_PREFIX}./unit_test_ytcpclient -s
# if [ $? -ne 0 ]
# then
#     echo 'Unit test failed. module-name:ytcpserver/ytcpsocket'
#     exit -1
# fi

# echo "module name yudpsocket"
# ${EXECUTE_PREFIX}./unit_test_yudpserver -s &
# sleep 2
# ${EXECUTE_PREFIX}./unit_test_yudpclient -s
# if [ $? -ne 0 ]
# then
#     echo 'Unit test failed. module-name:yudpsocket'
#     exit -1
# fi

# #ipc

# #network
# #tcp 
# sleep 2
# ./test_unit_ytcpserver -d yes &
# sleep 2
# ./test_unit_ytcpclient -d yes

# #udp
# sleep 2
# ./test_unit_yudpserver -d yes &
# sleep 2
# ./test_unit_yudpclient -d yes

