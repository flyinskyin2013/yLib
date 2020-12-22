###
 # @Author: Sky
 # @Date: 2020-03-31 10:41:36
 # @LastEditors: Sky
 # @LastEditTime: 2020-12-22 17:38:40
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

#utility
# ./test_unit_yconfig -d yes
# if [ $? -ne 0 ]
# then
#     echo 'test per unit error.'
#     exit -1
# fi

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


# echo 'test all ... ...'
# ./test_unit_all_in_one -d yes
# if [ $? -ne 0 ]
# then
#     echo 'test all error.'
#     exit -1
# fi