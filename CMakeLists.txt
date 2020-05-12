cmake_minimum_required(VERSION 2.8.10)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME})

#----------------------------------------------------------------------------------------

MESSAGE("================================================================")
MESSAGE("             Welcome to build project:${PROJECT_NAME}")
MESSAGE("================================================================")
#MESSAGE(STATUS "Welcome to build yLib")


#----------------------------------------------------------------------------------------

if(WIN32)


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

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

elseif(ANDROID)
    
endif()


#----------------------------------------------------------------------------------------

################################################################
option(BUILD_STATIC_YLIB "Build static ylib " ON)
option(BUILD_SHARED_YLIB "Build shared ylib " ON)

#yLib built-in module
option(BUILD_MODULE_YCURL "Build module-ycurl " ON)
option(BUILD_MODULE_YLOG "Build module-ylog " ON)
option(BUILD_MODULE_YSHELL "Build module-yshell " ON)
option(BUILD_MODULE_YXML "Build module-yxml " ON)
option(BUILD_MODULE_YCONFIG "Build module-yconfig " ON)
option(BUILD_MODULE_YJSON "Build module-yjson " ON)
option(BUILD_MODULE_YSHAREDMEMORY "Build module-ysharedmemory " ON)
option(BUILD_MODULE_YBASICVALUE "Build module-ybasicvalue " "ON")

option(BUILD_ALL_MODULE "build all module flags" ON)
option(BUILD_YLIB_ARCH "build arch" x86)
################################################################
#arch should be x86 X86 x86_64 X86_64 armeabi armeabi-v7a arm64-v8a
set(BUILD_YLIB_ARCH x86_64 CACHE STRING "Arch of ylib" FORCE)

# On windows platefrom, now, Only support yLog and ySharedMemory
if(WIN32)

    set(BUILD_MODULE_YBASICVALUE "ON")
    set(BUILD_MODULE_YCURL "OFF")
    set(BUILD_MODULE_YLOG "ON")
    set(BUILD_MODULE_YSHELL "OFF")
    set(BUILD_MODULE_YXML "OFF")
    set(BUILD_MODULE_YCONFIG  "ON")
    set(BUILD_MODULE_YJSON  "ON")
    set(BUILD_MODULE_YSHAREDMEMORY "ON")


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_ALL_MODULE)

        set(BUILD_STATIC_YLIB "ON")
        set(BUILD_SHARED_YLIB "ON")
        set(BUILD_MODULE_YBASICVALUE "ON")
        set(BUILD_MODULE_YLOG "ON")
        set(BUILD_MODULE_YSHELL "ON")
        set(BUILD_MODULE_YXML "ON")
        set(BUILD_MODULE_YCONFIG "ON")
        set(BUILD_MODULE_YJSON "ON")
        set(BUILD_MODULE_YSHAREDMEMORY "ON")
    endif()

elseif(ANDROID)
    
endif()




#----------------------------------------------------------------------------------------
if(WIN32)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows/libconfig)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux/libconfig)
elseif(ANDROID)
    
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/Catch2/inc)
#----------------------------------------------------------------------------------------

add_subdirectory(src)

add_subdirectory(examples)

add_subdirectory(tests)
