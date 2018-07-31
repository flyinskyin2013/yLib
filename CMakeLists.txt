cmake_minimum_required(VERSION 2.8.10)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME})

#----------------------------------------------------------------------------------------

MESSAGE("================================================================")
MESSAGE("             Welcome to build project:${PROJECT_NAME}")
MESSAGE("================================================================")
#MESSAGE(STATUS "Welcome to build yLib")


#----------------------------------------------------------------------------------------

#set(CMAKE_BUILD_TYPE release CACHE STRING "Type of build" FORCE)
MESSAGE(STATUS "BUILD_TYPE is : ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")



else()

#debug code
add_compile_options(-g)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

endif()

#set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#----------------------------------------------------------------------------------------

################################################################
option(BUILD_STATIC_YLIB "Build static ylib " ON)
option(BUILD_SHARED_YLIB "Build shared ylib " ON)
option(BUILD_MODULE_YCURL "Build module-ycurl " ON)
option(BUILD_MODULE_YLOG "Build module-ylog " ON)
option(BUILD_MODULE_YSHELL "Build module-yshell " ON)
################################################################


#----------------------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------------------

add_subdirectory(src)

add_subdirectory(examples)