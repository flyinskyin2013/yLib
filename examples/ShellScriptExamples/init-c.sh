#!/bin/bash

#start to build c-program basic structure.

#define init fuction
function make_comment()
{
	NowTime=`date`
	echo "/*">> $FILE_NAME$FILE_TYPE
	echo "	FileName:"$FILE_NAME$FILE_TYPE >> $FILE_NAME$FILE_TYPE
	echo "	Version:" >> $FILE_NAME$FILE_TYPE
	echo "	Description:" >> $FILE_NAME$FILE_TYPE
	echo "	Created On:"$NowTime >> $FILE_NAME$FILE_TYPE
	echo "	Modified date:" >> $FILE_NAME$FILE_TYPE
	echo "	Author:" >> $FILE_NAME$FILE_TYPE
	echo "*/">> $FILE_NAME$FILE_TYPE
}
function start_wirte()
{
	echo "#include <stdio.h>" >> $FILE_NAME$FILE_TYPE
	echo "int main(int argc, char * argv[]) {" >> $FILE_NAME$FILE_TYPE
	echo "" >> $FILE_NAME$FILE_TYPE
	echo "	return 0;" >> $FILE_NAME$FILE_TYPE
	echo "}"  >> $FILE_NAME$FILE_TYPE
}
function completed_wirte()
{
	echo "Wirte success"
}


#init default value
FILE_NAME=main
FILE_TYPE=.c

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

make_comment;
start_wirte;
completed_wirte;
