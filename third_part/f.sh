#!/bin/bash

echo target-dir = $1

rm -f ${1}/curl-prefix/src/*.gz
rm -f ${1}/jsoncpp-prefix/src/*.gz
rm -f ${1}/libxml2-prefix/src/*.gz



cp curl-7.55.1.tar.gz ${1}/curl-prefix/src/curl-7.55.1.tar.gz
cp jsoncpp_1.8.4.tar.gz ${1}/jsoncpp-prefix/src/1.8.4.tar.gz
cp libxml2-2.9.9.tar.gz ${1}/libxml2-prefix/src/v2.9.9.tar.gz