##############################

# file:   Makefile

# author:  sky

# modified-date:  2017-4-14

# created-date:  2016-12-26
###############################
export ROOT_DIR:=$(shell pwd)

export SRC_DIR:=${ROOT_DIR}/src
export INCLUDE_DIR:=${ROOT_DIR}/include
export TARGET:=test

SRCS:=$(wildcard *.cpp)
export OBJS:=$(patsubst %.cpp,%.o,${SRCS})
CUR_OBJS:=$(patsubst %.cpp,%.o,${SRCS})
CUR_DEPS:=$(patsubst %.o,%.dep,${CUR_OBJS})

export CC:=gcc
export CXX:=g++
export C_FLAGS:=
export CXX_FLAGS:=-I${INCLUDE_DIR} \
		-I${INCLUDE_DIR}/lktcp \
		-I${INCLUDE_DIR}/opencv \
		-I${INCLUDE_DIR}/tireddetect \
		-I${INCLUDE_DIR}/ffmpeg	     \
		-I${INCLUDE_DIR}/gst		\
		-I${INCLUDE_DIR}/glib-2.0	\
		-std=c++11 -Wall



#check ARCH

ifdef ARCH
ifeq (${ARCH},arm)
#set arch to arm
export LIB_DIR:=${ROOT_DIR}/armlib
export LD_FLAGS:=-L${LIB_DIR}/lktcp \
		-L${LIB_DIR}/opencv/lib \
		-L${LIB_DIR}/log4plus \
		-L${LIB_DIR}/dlib \
		-L${LIB_DIR}/corelib \
		-llog4cplus \
		-Wl,-rpath=${LIB_DIR}/corelib:${LIB_DIR}/dlib:\
			${LIB_DIR}/log4plus:${LIB_DIR}/opencv/lib:
			${LIB_DIR}/lktcp\
		-lsctp \
		-ldlib \
		-lpthread
LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/opencv/lib/*.so)))


else
#default set arch to x86
export LIB_DIR:=${ROOT_DIR}/lib
export LD_FLAGS:=-L${LIB_DIR}/tcplib \
		-L${LIB_DIR}/opencv \
		-L${LIB_DIR}/ffmpeg \
		-L${LIB_DIR} \
		-llog4cplus \
		-lsctp \
		-ldlib \
		-lpthread	

LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/opencv/*.so)))
LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/ffmpeg/*.so)))
LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/*.so)))



endif
else
#default set arch to x86
export LIB_DIR:=${ROOT_DIR}/lib
export LD_FLAGS:=-L${LIB_DIR}/tcplib \
		-L${LIB_DIR}/opencv \
		-L${LIB_DIR}/ffmpeg \
		-L${LIB_DIR} \
		-llog4cplus \
		-lsctp \
		-ldlib \
		-lpthread	

LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/opencv/*.so)))
LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/ffmpeg/*.so)))
LD_FLAGS+=$(patsubst lib%.so,-l%,$(notdir $(wildcard ${LIB_DIR}/*.so)))



endif


#end check ARCH

#check BUILD_RELEASE

ifdef BUILD_RELEASE
ifeq (${BUILD_RELEASE},TRUE)


export BUILD_DIR := $(ROOT_DIR)/Release  

else

export BUILD_DIR := $(ROOT_DIR)/Debug
export CXX_FLAGS+=-g
endif
else

export BUILD_DIR := $(ROOT_DIR)/Debug
export CXX_FLAGS+=-g
endif


#end check BUILD_RELEASE





#make build dir
ifeq ($(shell if [ -d ${BUILD_DIR} ];then echo "true";else echo "false";fi;),false)
$(shell mkdir -p ${BUILD_DIR})
else
#build dir exsit
endif







.PHONY:all clean help

.default:all 

all:${CUR_OBJS} subdir_objs 

subdir_objs:
	@${MAKE} -C src
	#@${CXX} ${OBJS} -o ${TARGET} ${LD_FLAGS}
	@${CXX} ${BUILD_DIR}/*.o -o ${TARGET} ${LD_FLAGS}
	@mv ${TARGET} ${BUILD_DIR}



%.o:%.cpp
	@${CXX} $< -c ${CXX_FLAGS}
	@cp $@ ${BUILD_DIR}

-include ${CUR_DEPS} 
%.dep:%.cpp
	set -e; rm -rf  $@.tmp; \
	${CXX} -MM ${CXX_FLAGS} $< > $@.tmp; \
	sed  's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp >$@; \
#	rm -rf $@ $@.tmp


clean:
#	rm -rf $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	rm -rf ./*.o
	rm -rf ./*.dep ./*.dep.tmp
	rm -rf ${BUILD_DIR}/*.o
	@${MAKE} -C src clean

help:
	@echo "\n	1. ARCH=x86 or arm ,to set compile some arch program.\n"
	@echo "	2. BUILD_RELEASE=TRUE or FALSE,to compile release program.\n"
