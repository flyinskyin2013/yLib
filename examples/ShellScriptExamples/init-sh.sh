#!/bin/bash

#start to build shell-script basic structure.

#define init fuction

function start_wirte()
{
	echo "#!/bin/bash">> $FILE_NAME$FILE_TYPE
}
function completed_wirte()
{
	echo "Wirte success"
}


#init default value
FILE_NAME=
FILE_TYPE=.sh

#check num of argument.
if [ $# -ge 1 ];
then
	echo "Now,starting ......"
else
	echo "error num,Useage: ./init.sh [filename] [...]"
	exit 1
fi

#check arg $1 
if [ -n "$1" ];
then
	FILE_NAME=$1
else
	echo "error arg,Useage: ./init.sh [filename] [...]"
	exit 1
fi

start_wirte;
completed_wirte;
