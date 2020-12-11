###
 # @Author: Sky
 # @Date: 2020-03-31 10:41:36
 # @LastEditors: Sky
 # @LastEditTime: 2020-12-11 09:47:26
 # @Description: 
 ###
echo 'scripts for tests'
echo 'test per unit ... ...'
#core
./test_unit_yobject -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

./test_unit_yexception -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


./test_unit_ycommon -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


./test_unit_ybasicvalue -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi


#ipc

#network

#utility
./test_unit_yconfig -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

./test_unit_yhttp -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

./test_unit_yjson -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

./test_unit_ylog -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

./test_unit_yshell -d yes
if [ $? -ne 0 ]
then
    echo 'test per unit error.'
    exit -1
fi

# ./test_unit_yxml -d yes
# if [ $? -ne 0 ]
# then
#     echo 'test per unit error.'
#     exit -1
# fi


# echo 'test all ... ...'
# ./test_unit_all_in_one -d yes
# if [ $? -ne 0 ]
# then
#     echo 'test all error.'
#     exit -1
# fi