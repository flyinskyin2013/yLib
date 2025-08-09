# A minimum required CMake version must be specified.cmake_minimum_required(VERSION <major>.<minor>)
# CMP0000: This policy was introduced in CMake version 2.6.0.
cmake_policy(SET CMP0000 NEW)
cmake_minimum_required(VERSION 3.15)


# enable feature project(<PROJECT-NAME> [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]][LANGUAGES <language-name>...])
# CMP0048: This policy was introduced in CMake version 3.0.
cmake_policy(SET CMP0048 NEW)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME} VERSION 0.2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



#-----------------------------------start------------------------------------------------

MESSAGE("================================================================")
MESSAGE("                  Welcome to build: ${PROJECT_NAME}")
MESSAGE("================================================================")

#----------------------------------------------------------------------------------------




#-----------------------------------load-cmake-file--------------------------------------

# import options.cmake
include(${PROJECT_SOURCE_DIR}/cmakes/options_and_vars.cmake)

# import commons.cmake
include(${PROJECT_SOURCE_DIR}/cmakes/commons.cmake)

#----------------------------------------------------------------------------------------




#----------------------------check-camke-necessary-args----------------------------------
ylib_precheck_cmake_necessary_args()
#----------------------------------------------------------------------------------------





#---------------set plateform, lib-type, common-compile-flags and so on------------------

# get current system-timestamp
string(TIMESTAMP BUILD_TIMESTAMP "%a %b %d %H:%M:%S %Y")

# get current version info
set(BUILD_VERSION "V${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# get current git-commit-id
execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE BUILD_GIT_COMMIT_ID)
string(REPLACE "\n" "" BUILD_GIT_COMMIT_ID ${BUILD_GIT_COMMIT_ID})

# set install path
if (CMAKE_INSTALL_PREFIX MATCHES "")

    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

endif()


# WIN32: True on windows systems, including win64.
if(WIN32)

    set(BUILD_PLATFORM "Windows")
    ylib_check_msvc_version_for_cxx11()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    set(BUILD_PLATFORM "Linux")
    ylib_check_gcc_or_clang_version_for_cxx11()
    ylib_set_compile_options()

elseif(ANDROID)
    ylib_check_gcc_or_clang_version_for_cxx11()
    ylib_set_compile_options()
    
    # #-s, --strip-all             Strip all symbols
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-s")
    # #-s, --strip-all             Strip all symbols
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-s")

    set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-s")
    set (CMAKE_EXE_LINKER_FLAGS "-Wl,-s")

    # if gcc > 4.9 add sanitizers
    # gcc 5.x has bug for "ld: unrecognized option '--push-state--no-as-needed'", we should use gcc 7.5
    if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.3)

        if (ENABLE_GNU_SANITIZER)

            message(STATUS "Enable AddressSanitizer MemorySanitizer")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined")
            
        endif()

    else()

        set(ENABLE_GNU_SANITIZER OFF)

    endif()
else()
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")
endif()

#----------------------------------------------------------------------------------------

#----------------------------choose build module-----------------------------------------


# On windows platefrom
if(WIN32)

    # UtilityGroup
    set(BUILD_MODULE_YHTTP "ON")
    set(BUILD_MODULE_YLOG "ON")
    set(BUILD_MODULE_YSHELL "ON")
    set(BUILD_MODULE_YXML "ON")
    set(BUILD_MODULE_YCONFIG  "ON")
    set(BUILD_MODULE_YJSON  "ON")

    # IpcGroup
    set(BUILD_MODULE_YSHAREDMEMORY "ON")

    # NetworkGroup
    set(BUILD_MODULE_YTCPSOCKET "OFF")
    set(BUILD_MODULE_YUDPSOCKET "OFF")

    #yLib Framework
    set(BUILD_MODULE_YTEMP "ON")
    set(BUILD_MODULE_YMAIN "OFF")

    # static and shared can't build at same time.
    set(BUILD_SHARED_YLIB ON)
    set(BUILD_STATIC_YLIB ON)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

elseif(ANDROID)
    # UtilityGroup
    set(BUILD_MODULE_YHTTP "OFF")
    set(BUILD_MODULE_YLOG "ON")
    set(BUILD_MODULE_YSHELL "ON")
    set(BUILD_MODULE_YXML "OFF")
    set(BUILD_MODULE_YCONFIG  "ON")
    set(BUILD_MODULE_YJSON  "OFF")

    # IpcGroup
    set(BUILD_MODULE_YSHAREDMEMORY "OFF")

    # NetworkGroup
    set(BUILD_MODULE_YTCPSOCKET "ON")
    set(BUILD_MODULE_YUDPSOCKET "ON")

    #yLib Framework
    set(BUILD_MODULE_YTEMP "ON")
    set(BUILD_MODULE_YMAIN "ON")

    # static and shared can't build at same time.
    set(BUILD_SHARED_YLIB ON)
    set(BUILD_STATIC_YLIB ON)
else()
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")    
endif()

#----------------------------------------------------------------------------------------




#------------------------------set include dir-----------------------------------
if(WIN32)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux)

elseif(ANDROID)
else()
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")    
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/Catch2/inc)
#----------------------------------------------------------------------------------------

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(examples)
endif()


if(BUILD_UNIT_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()


# infomation summary
MESSAGE("----------------------------------------------------------------")
MESSAGE("             ${PROJECT_NAME}'s Configure Information")
MESSAGE("----------------------------------------------------------------")
MESSAGE("BUILD_TYPE             : ${CMAKE_BUILD_TYPE}")
MESSAGE("BUILD_ARCH             : ${BUILD_YLIB_ARCH}")
MESSAGE("BUILD_PLATFORM         : ${BUILD_PLATFORM}")
MESSAGE("BUILD_VERSION          : ${BUILD_VERSION}")
MESSAGE("BUILD_TIMESTAMP        : ${BUILD_TIMESTAMP}")
MESSAGE("BUILD_GIT_COMMIT_ID    : ${BUILD_GIT_COMMIT_ID}")
MESSAGE("BUILD_INSTALL_PATH     : ${CMAKE_INSTALL_PREFIX}")
MESSAGE("----------------------------------------------------------------")
MESSAGE("Builtin IPC Moudle List        : ")
foreach(item ${IPC_MODULE_LIST})
    MESSAGE("           <${item}>")
endforeach()
MESSAGE("----------------------------------------------------------------")
MESSAGE("Builtin Network Moudle List    : ")
foreach(item ${NETWORK_MODULE_LIST})
    MESSAGE("           <${item}>")
endforeach()
MESSAGE("----------------------------------------------------------------")
MESSAGE("Builtin Utility Moudle List    : ")
foreach(item ${UTILITY_MODULE_LIST})
    MESSAGE("           <${item}>")
endforeach()
MESSAGE("----------------------------------------------------------------")
MESSAGE("Builtin Test List      : ")
foreach(item ${BUILTIN_TEST_LIST})
    MESSAGE("           <${item}>")
endforeach()
MESSAGE("----------------------------------------------------------------")