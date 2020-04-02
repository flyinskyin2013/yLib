#!/bin/bash

if [ $# -eq 1 ]
then
echo "Terminating process :*"${1}"*"
else

echo "Num of argument is wrong!!!We need a process name."
echo "Usage:./ TerminateProcessOfName.sh process_name"
exit 1

fi


pid_list=$(ps -aux | grep -v grep | grep -v TerminateProcessOfName | grep ${1} | awk '{print $2}' | xargs )

if [ -z "${pid_list}" ]
then
echo "We do not found process:"*${1}*
exit 0
fi

sub_process_pid=0

for i in ${pid_list}
do
	if [ ${sub_process_pid} -lt ${i} ]
	then
		sub_process_pid=${i}
	fi
done 

echo "Now we get sub-process pid:"${sub_process_pid}
echo "Killing pid: "${sub_process_pid}
kill -9 ${sub_process_pid}
echo "done!!!"
