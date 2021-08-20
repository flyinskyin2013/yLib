# A minimum required CMake version must be specified.cmake_minimum_required(VERSION <major>.<minor>)
# CMP0000: This policy was introduced in CMake version 2.6.0.
cmake_policy(SET CMP0000 NEW)
cmake_minimum_required(VERSION 3.0)


# enable feature project(<PROJECT-NAME> [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]][LANGUAGES <language-name>...])
# CMP0048: This policy was introduced in CMake version 3.0.
cmake_policy(SET CMP0048 NEW)

SET(PROJECT_NAME yLib)
project(${PROJECT_NAME} VERSION 0.1.0.0)



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

    if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")

        #release mode 

    else()

        #debug mode
        # add_compile_options(-g)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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

elseif(ANDROID)
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")  
else()
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")
endif()

#----------------------------------------------------------------------------------------

#----------------------------choose build module-----------------------------------------


# On windows platefrom, now, Only support yLog and ySharedMemory
if(WIN32)

    # UtilityGroup
    set(BUILD_MODULE_YCURL "OFF")
    set(BUILD_MODULE_YHTTP "OFF")
    set(BUILD_MODULE_YLOG "ON")
    set(BUILD_MODULE_YSHELL "OFF")
    set(BUILD_MODULE_YXML "OFF")
    set(BUILD_MODULE_YCONFIG  "ON")
    set(BUILD_MODULE_YJSON  "ON")

    # IpcGroup
    set(BUILD_MODULE_YSHAREDMEMORY "ON")

    # NetworkGroup
    set(BUILD_MODULE_YTCPSERVER "OFF")
    set(BUILD_MODULE_YTCPSOCKET "OFF")
    set(BUILD_MODULE_YUDPSERVER "OFF")
    set(BUILD_MODULE_YUDPSOCKET "OFF")

    #yLib Framework
    set(BUILD_MODULE_YTEMP "ON")
    set(BUILD_MODULE_YMAIN "OFF")

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_ALL_MODULE)

        # UtilityGroup
        set(BUILD_MODULE_YCURL "ON")
        set(BUILD_MODULE_YHTTP "ON")
        set(BUILD_MODULE_YLOG "ON")
        set(BUILD_MODULE_YSHELL "ON")
        set(BUILD_MODULE_YXML "ON")
        set(BUILD_MODULE_YCONFIG "ON")
        set(BUILD_MODULE_YJSON "ON")

        # IpcGroup
        set(BUILD_MODULE_YSHAREDMEMORY "ON")

        # NetworkGroup
        set(BUILD_MODULE_YTCPSERVER "ON")
        set(BUILD_MODULE_YTCPSOCKET "ON")
        set(BUILD_MODULE_YUDPSERVER "ON")
        set(BUILD_MODULE_YUDPSOCKET "ON")

        #yLib Framework
        set(BUILD_MODULE_YTEMP "ON")
        set(BUILD_MODULE_YMAIN "ON")
    endif()

elseif(ANDROID)
else()
    message(FATAL_ERROR "Don't support this system ${CMAKE_SYSTEM_NAME}.")    
endif()

#----------------------------------------------------------------------------------------


#------------------------------set include dir-----------------------------------
if(WIN32)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/windows/libconfig)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/third_part/linux/libconfig)

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