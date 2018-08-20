##############################

# file:   Makefile

# author:  sky

# modified-date:  2017-4-14

# created-date:  2016-12-26
###############################

SRCS:=$(wildcard *.cpp)
OBJS+=$(patsubst %.cpp,%.o,${SRCS})
CUR_OBJS:=$(patsubst %.cpp,%.o,${SRCS})
CUR_DEPS:=$(patsubst %.o,%.dep,${CUR_OBJS})


.PHONY:all clean


all:${CUR_OBJS}

%.o:%.cpp
	@${CXX} $< -c ${CXX_FLAGS}
	@cp $@ ${BUILD_DIR}

-include ${CUR_DEPS} 
%.dep:%.cpp
	set -e; rm -rf  $@.tmp; \
	${CXX} -MM ${CXX_FLAGS} $< > $@.tmp; \
	sed  's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp >$@; \


clean:
	rm -rf ./*.o
	rm -rf ./*.dep ./*.dep.tmp
