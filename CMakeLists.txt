cmake_minimum_required(VERSION 2.8.10)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME})

#----------------------------------------------------------------------------------------

MESSAGE("================================================================")
MESSAGE("             Welcome to build project:${PROJECT_NAME}")
MESSAGE("================================================================")
#MESSAGE(STATUS "Welcome to build yLib")


#----------------------------------------------------------------------------------------

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

#set(CMAKE_BUILD_TYPE release CACHE STRING "Type of build" FORCE)
set(CMAKE_BUILD_TYPE debug CACHE STRING "Type of build" FORCE)
MESSAGE(STATUS "BUILD_TYPE is : ${CMAKE_BUILD_TYPE}")
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




#----------------------------------------------------------------------------------------
#arch should be x86 X86 x86_64 X86_64 armeabi armeabi-v7a arm64-v8a
set(BUILD_YLIB_ARCH arm64-v8a CACHE STRING "Arch of ylib" FORCE)
################################################################
option(BUILD_STATIC_YLIB "Build static ylib " ON)
option(BUILD_SHARED_YLIB "Build shared ylib " ON)
option(BUILD_MODULE_YCURL "Build module-ycurl " ON)
option(BUILD_MODULE_YLOG "Build module-ylog " ON)
option(BUILD_MODULE_YSHELL "Build module-yshell " ON)
option(BUILD_MODULE_YXML "Build module-yxml " ON)
option(BUILD_MODULE_YCONFIG "Build module-yconfig " ON)
################################################################


#----------------------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/core)
#----------------------------------------------------------------------------------------

add_subdirectory(src)

add_subdirectory(examples)
