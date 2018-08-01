#----------------------------------------------------------------------------------------
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/ycommon.cpp)

#----------------------------------------------------------------------------------------
#define macro to add module source code
macro(ylib_build_module name)

    string(TOLOWER ${name} module_name)

    set(SRC
        ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${module_name}.cpp)

    message(STATUS "yLib will include ${name} module")

endmacro()


if(BUILD_MODULE_YCURL)

    ylib_build_module(YCURL)

endif()


if(BUILD_MODULE_YLOG)

    ylib_build_module(YLOG)

endif()


if(BUILD_MODULE_YSHELL)

    ylib_build_module(YSHELL)

endif()

if(BUILD_MODULE_YXML)

    ylib_build_module(YXML)

endif()

#----------------------------------------------------------------------------------------

if(WIN32)

    if(${BUILD_YLIB_ARCH} MATCHES "(x86_64|X86_64)")
        #64bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows/x86_64)
    else()
        #32bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows/x86)
    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(${BUILD_YLIB_ARCH} MATCHES "(x86_64|X86_64)")
        #linux 64bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/x86_64)
    elseif(${BUILD_YLIB_ARCH} MATCHES "(x86|X86)")
        #linux 32bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/x86)
    elseif(${BUILD_YLIB_ARCH} MATCHES "armeabi")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi)
    elseif(${BUILD_YLIB_ARCH} MATCHES "armeabi-v7a")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi-v7a)
    elseif(${BUILD_YLIB_ARCH} MATCHES "arm64-v8a")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/arme64-v8a)
    endif()

elseif(ANDROID)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/android)
endif()

#----------------------------------------------------------------------------------------

if(BUILD_STATIC_YLIB)

    add_library(yLib_s STATIC ${SRC})

endif()

if(BUILD_SHARED_YLIB)

    add_library(yLib SHARED ${SRC})

endif()

if(WIN32)


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")


    if(BUILD_STATIC_YLIB)

        target_link_libraries( yLib_s
                            libcurl.so libxml2.so )

    endif()

    if(BUILD_SHARED_YLIB)

        target_link_libraries( yLib
                            libcurl.so libxml2.so )

    endif()

elseif(ANDROID)


endif()