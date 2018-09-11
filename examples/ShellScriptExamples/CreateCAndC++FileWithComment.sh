#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: > ./CreateCAndC++FileWithComment.sh test.cpp"
    exit 1
fi

echo "Create file "$1" ... ..."
touch $1

echo "Add comment for file "$1" ... ..."
bash ./add-comment.sh $1