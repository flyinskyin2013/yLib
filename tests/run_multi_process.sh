#!/bin/bash

###
 # @Author: Sky
 # @Date: 2023-09-02 14:22:29
 # @LastEditors: Sky
 # @LastEditTime: 2023-09-03 13:47:33
 # @Description: 
### 

# ARG0 ARG1... ... ... ... ARGN
# We run ARG1 .... ARGN-1 in backgroud, run ARGN in foreground

set -e

if [ $# -lt 2 ]
then
    echo "we need more than 1 and less than 10 process"
    exit -1
fi


PROCESS_NUM=${#}
PROCESS_ARGS=""
QEMU_EXECUTABLE_LOADER=
QEMU_EXECUTABLE_LOADER_ARGUMENTS=

echo SCRIPTS_NAME = $0

echo PROCESS_NUM = ${#}
echo PROCESS_ARGS = ${PROCESS_ARGS}

echo QEMU_EXECUTABLE_LOADER = ${QEMU_EXECUTABLE_LOADER}
echo QEMU_EXECUTABLE_LOADER_ARGUMENTS = ${QEMU_EXECUTABLE_LOADER_ARGUMENTS}


for idx in `seq $[$# - 1]`
do
    PROCESS=$(eval "echo \${${idx}}")
    echo run process ${PROCESS} in backgroud
    ${PROCESS} ${PROCESS_ARGS}&

done

PROCESS=$(eval "echo \${${#}}")
echo run process ${PROCESS} in foreground
${PROCESS} ${PROCESS_ARGS}