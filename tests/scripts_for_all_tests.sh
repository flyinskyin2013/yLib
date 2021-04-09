###
 # @Author: Sky
 # @Date: 2021-04-09 14:22:29
 # @LastEditors: Sky
 # @LastEditTime: 2021-04-09 14:54:23
 # @Description: 
### 

echo 'scripts for tests'
echo 'test per unit ... ...'

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
#core
${EXECUTE_PREFIX}./test_unit_yobject -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_yexception -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


${EXECUTE_PREFIX}./test_unit_ycommon -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


${EXECUTE_PREFIX}./test_unit_ybasicvalue -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


#ipc

#network
#tcp 
sleep 2
./test_unit_ytcpserver -d yes &
sleep 2
./test_unit_ytcpclient -d yes

#udp
sleep 2
./test_unit_yudpserver -d yes &
sleep 2
./test_unit_yudpclient -d yes

#utility
./test_unit_yconfig -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_yhttp -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_yjson -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_ylog -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_yshell -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_yxml -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

${EXECUTE_PREFIX}./test_unit_ytimer -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

#Algorithm
./basic_algorithm/test_algorithm_ylinearlist -d yes
./basic_algorithm/test_algorithm_ylinkedlist -d yes
./basic_algorithm/test_algorithm_yqueue -d yes
./basic_algorithm/test_algorithm_ystack -d yes

#echo 'test all ... ...'
# ./test_unit_all_in_one -d yes
# if [ $? -ne 0 ]
# then
#     echo 'test all error.'
#     exit -1
# fi