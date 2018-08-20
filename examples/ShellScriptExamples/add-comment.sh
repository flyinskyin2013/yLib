#!/bin/bash

#start to build c-program basic structure.

#init default value
FILE_CONTENT=
FILE_NAME=
#BACKUP_FILE_TYPE="$FILE_TYPE"".bak"
BACKUP_FILE_TYPE=".bak"


#define init fuction
function backup_file()
{
	#The first way is failed,because it can not save text format
	#FILE_CONTENT=$(cat $FILE_NAME$FILE_TYPE) 
	echo "buckuping ... ..."
	cat $FILE_NAME > $FILE_NAME$BACKUP_FILE_TYPE
}
function clean_file()
{
	echo "cleaning ... ..."
	echo "" > $FILE_NAME
}
function make_comment()
{
	NowTime=`date`
	Author="Sky"
	Version="V1.0"
	echo "/*"> $FILE_NAME
	echo "*	FileName:"$FILE_NAME >> $FILE_NAME
	echo "*	Version:"$Version >> $FILE_NAME
	echo "*	Description:" >> $FILE_NAME
	echo "*	Created On:"$NowTime >> $FILE_NAME
	echo "*	Modified date:" >> $FILE_NAME
	echo "*	Author:"$Author >> $FILE_NAME
	echo "*/">> $FILE_NAME
}
function start_wirte()
{
	cat $FILE_NAME$BACKUP_FILE_TYPE >> $FILE_NAME
	rm $FILE_NAME$BACKUP_FILE_TYPE
}
function completed_wirte()
{
	echo "Wirte success"
}



#check num of argument.
if [ $# -ge 1 ];
then
	echo "Now,starting ......"
else
	echo "error num,Useage: ./init.sh [filename] [...]"
	exit 1
fi

#check arg $1 
if [ -n "$1" ]
then
	FILE_NAME=$1
fi

backup_file;
clean_file;
make_comment;
start_wirte;
completed_wirte;
sync
