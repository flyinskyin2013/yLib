cmake_minimum_required(VERSION 2.8.10)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME})

#-----------------------------------start------------------------------------------------

MESSAGE("================================================================")
MESSAGE("             Welcome to build project:${PROJECT_NAME}")
MESSAGE("================================================================")

#----------------------------------------------------------------------------------------



#------------------------------------option----------------------------------------------
################################################################
#yLib lib-type
option(BUILD_STATIC_YLIB "Build static ylib " ON)
option(BUILD_SHARED_YLIB "Build shared ylib " ON)

#yLib built-in module
option(BUILD_MODULE_YCURL "Build module-ycurl(it insteads by yhttp) " ON)
option(BUILD_MODULE_YHTTP "Build module-yhttp " ON)
option(BUILD_MODULE_YLOG "Build module-ylog " ON)
option(BUILD_MODULE_YSHELL "Build module-yshell " ON)
option(BUILD_MODULE_YXML "Build module-yxml " ON)
option(BUILD_MODULE_YCONFIG "Build module-yconfig " ON)
option(BUILD_MODULE_YJSON "Build module-yjson " ON)
option(BUILD_MODULE_YSHAREDMEMORY "Build module-ysharedmemory " ON)
option(BUILD_MODULE_YTCPSERVER "Build module-ytcpserver " ON)
option(BUILD_MODULE_YTCPCLIENT "Build module-ytcpclient " ON)
option(BUILD_MODULE_YUDPSERVER "Build module-yudpserver " ON)
option(BUILD_MODULE_YUDPCLIENT "Build module-yudpclient " ON)

#yLib Framework
option(BUILD_MODULE_YTEMP "Build example module-ytemp" ON)
option(BUILD_MODULE_YMAIN "Build example module-ymain" ON)

#yLib build-all flags
option(BUILD_ALL_MODULE "build all module flags" ON)
option(BUILD_EXAMPLE "build example flags" ON)
option(BUILD_UNIT_TEST "build unit-test flags" ON)
################################################################

#----------------------------------------------------------------------------------------


#----------------------------check-args--------------------------------------------------
# BUILD_YLIB_ARCH
#arch should be x86 x86_64 armeabi armeabihf aarch64
#set(BUILD_YLIB_ARCH x86_64 CACHE STRING "Arch of ylib" FORCE)

# check necessary CMAKE-ARGS
if (NOT DEFINED BUILD_YLIB_ARCH)
    message(FATAL_ERROR "User must define ARG: BUILD_YLIB_ARCH. The value of it can be one of 'x86|x86_64|armeabi|armeabihf|aarch64'")
else()
    if(NOT BUILD_YLIB_ARCH  MATCHES "(x86|x86_64|armeabi|armeabihf|aarch64)")
        message(FATAL_ERROR "User must define ARG: BUILD_YLIB_ARCH. The value of it can be one of 'x86|x86_64|armeabi|armeabihf|aarch64'")
    endif()
endif()



if(NOT CMAKE_BUILD_TYPE  MATCHES "(Release|RELEASE|release|Debug|DEBUG|debug)")
    message(FATAL_ERROR "User must define ARG: CMAKE_BUILD_TYPE. The value of it can be one of 'Release|RELEASE|release|Debug|DEBUG|debug'")
endif()
#----------------------------------------------------------------------------------------


#---------------set plateform, lib-type, common-compile-flags and so on------------------

#define BUILD_PLATFORM
set(BUILD_PLATFORM  "" CACHE STRING INTERNAL FORCE)
set(BUILD_VERSION "" CACHE STRING INTERNAL FORCE)
set(BUILD_VERSION_MAJOR "" CACHE STRING INTERNAL FORCE)
set(BUILD_VERSION_MINOR "" CACHE STRING INTERNAL FORCE)
set(BUILD_VERSION_PATCH "" CACHE STRING INTERNAL FORCE)
set(BUILD_TIMESTAMP "" CACHE STRING INTERNAL FORCE)
set(BUILD_GIT_COMMIT_ID "" CACHE STRING INTERNAL FORCE)
set(BUILTIN_MODULE_LIST "" CACHE STRING INTERNAL FORCE)
set(BUILTIN_TEST_LIST "" CACHE STRING INTERNAL FORCE)

string(TIMESTAMP BUILD_TIMESTAMP "%a %b %d %H:%M:%S %Y")

execute_process(COMMAND sed -n "1p" INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CurrentVersionStr.txt OUTPUT_VARIABLE BUILD_VERSION_MAJOR)
string(REPLACE "\n" "" BUILD_VERSION_MAJOR ${BUILD_VERSION_MAJOR})
execute_process(COMMAND sed -n "2p" INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CurrentVersionStr.txt OUTPUT_VARIABLE BUILD_VERSION_MINOR)
string(REPLACE "\n" "" BUILD_VERSION_MINOR ${BUILD_VERSION_MINOR})
execute_process(COMMAND sed -n "3p" INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CurrentVersionStr.txt OUTPUT_VARIABLE BUILD_VERSION_PATCH)
string(REPLACE "\n" "" BUILD_VERSION_PATCH ${BUILD_VERSION_PATCH})
#execute_process(COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/CurrentVersionStr.txt OUTPUT_VARIABLE BUILD_VERSION)
set(BUILD_VERSION "V${BUILD_VERSION_MAJOR}.${BUILD_VERSION_MINOR}.${BUILD_VERSION_PATCH}")

execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE BUILD_GIT_COMMIT_ID)
string(REPLACE "\n" "" BUILD_GIT_COMMIT_ID ${BUILD_GIT_COMMIT_ID})
if(WIN32)

    set(BUILD_PLATFORM "Windows")

    MESSAGE(STATUS "BUILD_TYPE is : ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS "BUILD_ARCH is : ${BUILD_YLIB_ARCH}")
    MESSAGE(STATUS "BUILD_PLATFORM is : ${BUILD_PLATFORM}")

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    set(BUILD_PLATFORM "Linux")

    MESSAGE(STATUS "BUILD_TYPE is : ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS "BUILD_ARCH is : ${BUILD_YLIB_ARCH}")
    MESSAGE(STATUS "BUILD_PLATFORM is : ${BUILD_PLATFORM}")

    if (CMAKE_INSTALL_PREFIX MATCHES "")
        set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
    endif()
    
    if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")

        #release mode 

    else()

        #debug mode
        add_compile_options(-g)
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

    endif()

    #set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # if gcc > 4.9 add sanitizers
    # gcc 5.x has bug for "ld: unrecognized option '--push-state--no-as-needed'", we should use gcc 7.5
    if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.3)

        message(STATUS "Enable AddressSanitizer MemorySanitizer")
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined")
    endif()

elseif(ANDROID)
    
endif()


#----------------------------------------------------------------------------------------

#----------------------------choose build module-----------------------------------------


# On windows platefrom, now, Only support yLog and ySharedMemory
if(WIN32)

    set(BUILD_MODULE_YBASICVALUE "ON")
    set(BUILD_MODULE_YCURL "OFF")
    set(BUILD_MODULE_YHTTP "OFF")
    set(BUILD_MODULE_YLOG "ON")
    set(BUILD_MODULE_YSHELL "OFF")
    set(BUILD_MODULE_YXML "OFF")
    set(BUILD_MODULE_YCONFIG  "ON")
    set(BUILD_MODULE_YJSON  "ON")
    set(BUILD_MODULE_YSHAREDMEMORY "ON")
    set(BUILD_MODULE_YTEMP "ON")
    
    set(BUILD_MODULE_YMAIN "OFF")

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_ALL_MODULE)

        set(BUILD_STATIC_YLIB "ON")
        set(BUILD_SHARED_YLIB "ON")
        set(BUILD_MODULE_YBASICVALUE "ON")
        set(BUILD_MODULE_YCURL "ON")
        set(BUILD_MODULE_YHTTP "ON")
        set(BUILD_MODULE_YLOG "ON")
        set(BUILD_MODULE_YSHELL "ON")
        set(BUILD_MODULE_YXML "ON")
        set(BUILD_MODULE_YCONFIG "ON")
        set(BUILD_MODULE_YJSON "ON")
        set(BUILD_MODULE_YSHAREDMEMORY "ON")
        set(BUILD_MODULE_YTCPSERVER "ON")
        set(BUILD_MODULE_YTCPSOCKET "ON")
        set(BUILD_MODULE_YUDPSERVER "ON")
        set(BUILD_MODULE_YUDPSOCKET "ON")



        set(BUILD_MODULE_YTEMP "ON")
        set(BUILD_MODULE_YMAIN "ON")
    endif()

elseif(ANDROID)
    
endif()

#----------------------------------------------------------------------------------------


#------------------------------set include dir-------------------------------------------
if(WIN32)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows/libconfig)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux/libconfig)
elseif(ANDROID)
    
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/Catch2/inc)
#----------------------------------------------------------------------------------------

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(examples)
endif()

if(BUILD_UNIT_TEST)
    add_subdirectory(tests)
endif()

MESSAGE("----------------------------------------------------------------")
MESSAGE("             ${PROJECT_NAME}'s Configure Information")
MESSAGE("----------------------------------------------------------------")
MESSAGE(STATUS "BUILD_TYPE             : ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "BUILD_ARCH             : ${BUILD_YLIB_ARCH}")
MESSAGE(STATUS "BUILD_PLATFORM         : ${BUILD_PLATFORM}")
MESSAGE(STATUS "BUILD_VERSION          : ${BUILD_VERSION}")
MESSAGE(STATUS "BUILD_TIMESTAMP        : ${BUILD_TIMESTAMP}")
MESSAGE(STATUS "BUILD_GIT_COMMIT_ID    : ${BUILD_GIT_COMMIT_ID}")
MESSAGE("----------------------------------------------------------------")
MESSAGE(STATUS "Builtin Moudle List    : ")
list(LENGTH BUILTIN_MODULE_LIST BUILTIN_MODULE_LIST_LEN)
#message(${BUILTIN_MODULE_LIST_LEN})
#message(${BUILTIN_MODULE_LIST})
#SET(FRIENDLY_MSG "")
#foreach(index RANGE ${BUILTIN_MODULE_LIST_LEN})
foreach(item ${BUILTIN_MODULE_LIST})
    #list(GET BUILTIN_MODULE_LIST ${index} item)
    #SET(FRIENDLY_MSG "${FRIENDLY_MSG} ${item}")
    MESSAGE("           <${item}>")
endforeach()
#message(${FRIENDLY_MSG})
MESSAGE("----------------------------------------------------------------")
MESSAGE(STATUS "Builtin Test List      : ")
foreach(item ${BUILTIN_TEST_LIST})
    MESSAGE("           <${item}>")
endforeach()
MESSAGE("----------------------------------------------------------------")