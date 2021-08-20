#!/bin/bash

###
 # @Author: Sky
 # @Date: 2021-04-09 14:22:29
 # @LastEditors: Sky
 # @LastEditTime: 2021-08-20 14:42:54
 # @Description: 
### 

echo 'ylib-unit-tests start ... ...'

EXECUTE_PREFIX=""

if [ $# -ge 1 ]
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

    
    else
        echo "example:./scripts_for_tests.sh qemu-gnueabi"
        echo "example:./scripts_for_tests.sh qemu-gnueabihf"
        echo "example:./scripts_for_tests.sh qemu-aarch64"
        exit -1
    fi
fi

# core group
CORE_GROUP_MODULE_LIST=" \
                        yobject \
                        ycommon \
                        yexception \
                        yobject \
                        "

# ipc group
IPC_GROUP_MODULE_LIST=" "

# network group
NETWORK_GROUP_MODULE_LIST=" \
                            ytcpserver \
                            ytcpclient \
                            yudpserver \
                            yudpclient \
                            "

# utility group
UTILITY_GROUP_MODULE_LIST=" \
                            yconfig \
                            ycurl \
                            yhttp \
                            yjson \
                            ylog \
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

ALL_MODULE_LISTS=${CORE_GROUP_MODULE_LIST}${UTILITY_GROUP_MODULE_LIST}${BASIC_ALGORITHM_GROUP_MODULE_LIST}" all_in_one"

for module_name in ${ALL_MODULE_LISTS}
do
    echo "module name "${module_name}
    ${EXECUTE_PREFIX}./unit_test_${module_name} -d yes
    if [ $? -ne 0 ]
    then
        echo 'Unit test failed. module-name:'${module_name}
        exit -1
    fi
    
    ${EXECUTE_PREFIX}./unit_test_${module_name}_s -d yes
    if [ $? -ne 0 ]
    then
        echo 'Unit test failed. module-name:'${module_name}_s
        exit -1
    fi
done



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

