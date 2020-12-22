#link other whole-archive to libylib.so or libylib.a
#set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive")


if(WIN32)



elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")

        #release mode 

    else()

        #debug mode
        add_compile_options(-g)
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

    endif()

elseif(ANDROID)
    
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} )
# set(SRC
#     ${SRC} ${CMAKE_CURRENT_BINARY_DIR}/ylib_extra_infos.cpp)
# message(${SRC})
#----------------------------------------------------------------------------------------
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ycommon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/yobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/yexception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ybasicvalue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ylinuxerrnohelper.cpp

    #for network module
    ${CMAKE_CURRENT_SOURCE_DIR}/network/yabstractsocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/yabstractsocketserver.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/ylib_extra_infos.cpp
    )

#----------------------------------------------------------------------------------------
#define macro to add module source code
macro(ylib_build_module name)

    string(TOLOWER ${name} module_name)

    set(SRC
        ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${module_name}.cpp)

    message(STATUS "yLib will include ${name} module")

endmacro()


macro(ylib_build class_name name)

    string(TOLOWER ${name} module_name)
    string(TOLOWER ${class_name} class_dir_name)
    set(SRC
        ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${class_dir_name}/${module_name}.cpp)

    message(STATUS "yLib will include ${class_name}-${name} module")

    set(BUILTIN_MODULE_LIST ${BUILTIN_MODULE_LIST} ${class_name}-${name}  CACHE STRING INTERNAL FORCE)
endmacro()


if(BUILD_MODULE_YCURL)

    ylib_build(UTILITY YCURL)

endif()


if(BUILD_MODULE_YHTTP)

    ylib_build(UTILITY YHTTP)

endif()

if(BUILD_MODULE_YLOG)

    ylib_build(UTILITY YLOG)

endif()


if(BUILD_MODULE_YSHELL)

    ylib_build(UTILITY YSHELL)

endif()

if(BUILD_MODULE_YXML)

    ylib_build(UTILITY YXML)

endif()

if(BUILD_MODULE_YCONFIG)

    ylib_build(UTILITY YCONFIG)

endif()

if(BUILD_MODULE_YJSON)

    ylib_build(UTILITY YJSON)

endif()


if(BUILD_MODULE_YSHAREDMEMORY)

    ylib_build(IPC YSHAREDMEMORY)

endif()


if(BUILD_MODULE_YTCPSERVER)

    ylib_build(NETWORK YTCPSERVER)

endif()


if(BUILD_MODULE_YTCPSOCKET)

    ylib_build(NETWORK YTCPSOCKET)

endif()


if(BUILD_MODULE_YUDPSERVER)

    ylib_build(NETWORK YUDPSERVER)

endif()


if(BUILD_MODULE_YUDPSOCKET)

    ylib_build(NETWORK YUDPSOCKET)

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
    #message(${BUILD_YLIB_ARCH})
    if("${BUILD_YLIB_ARCH}" MATCHES "^(x86_64|X86_64)$")
        #linux 64bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/x86_64)
    elseif(${BUILD_YLIB_ARCH} MATCHES "^(x86|X86)$")
        #linux 32bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/x86)
    elseif(${BUILD_YLIB_ARCH} MATCHES "^armeabi$")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi)
    elseif(${BUILD_YLIB_ARCH} MATCHES "^armeabihf$")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabihf)
    elseif(${BUILD_YLIB_ARCH} MATCHES "aarch64")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/aarch64)
    endif()

elseif(ANDROID)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/android)
endif()



#----------------------------------------------------------------------------------------



if(WIN32)
    #if(BUILD_STATIC_YLIB)

    #    add_library(ylib_s STATIC ${SRC})

    #endif()

    #if(BUILD_SHARED_YLIB)

    #    add_library(ylib SHARED ${SRC})

    #endif()
    add_library(ylib SHARED ${SRC})
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_link_libraries( ylib log4cppD.lib jsoncppD.lib libconfig++D.lib  ws2_32 Shlwapi)
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
        target_link_libraries( ylib log4cpp.lib jsoncpp.lib libconfig++.lib  ws2_32 Shlwapi)
    endif()





elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_STATIC_YLIB)

        add_library(ylib_s STATIC ${SRC})

    endif()

    if(BUILD_SHARED_YLIB)

        add_library(ylib SHARED ${SRC})

    endif()


    if(BUILD_STATIC_YLIB)

        target_link_libraries( ylib_s
                            libcurl.a libxml2.a liblog4cpp.a liblibconfig++.a libjsoncpp.a)

    endif()

    if(BUILD_SHARED_YLIB)

        target_link_libraries( ylib
                            libcurl.a libxml2.a liblog4cpp.a liblibconfig++.a libjsoncpp.a)

    endif()

elseif(ANDROID)


endif()


#set(CMAKE_SKIP_BUILD_RPATH FALSE)                 # 编译时加上RPATH  
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)         # 编译时RPATH不使用安装的RPATH  
#set(CMAKE_INSTALL_RPATH "")                       # 安装RPATH为空  
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)      # 安装的执行文件不加上RPATH
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


if(WIN32)
    #for lib files
    INSTALL(TARGETS  ylib 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ylib.dll
    DESTINATION examples
    )
    INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ylib.dll
    DESTINATION lib
    )
    INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ylib.dll
    DESTINATION tests
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    #for lib files
    INSTALL(TARGETS  ylib ylib_s
        #RUNTIME DESTINATION
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )

elseif(ANDROID)


endif()


#for core header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/core DESTINATION include/yLib

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

#for ipc header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ipc DESTINATION include/yLib

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

#for utility header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/utility DESTINATION include/yLib

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

#for network header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/network DESTINATION include/yLib

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/basic_algorithm DESTINATION include/yLib

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/ylib.hpp
    DESTINATION include/yLib
)

#INSTALL(SCRIPT

#    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/FindyLibConfig.cmake.example
#    CODE
#    "MESSAGE(\"Installing ylib.cmake file\")"
#)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/yLibConfig.cmake.example yLibConfig.cmake @ONLY)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/yLibConfig.cmake
    DESTINATION cmake
)




#-----------------------------------generate ylib_build_info.hpp-------------------------
set(DECELARE_YLIB_BUILD_TYPE            "const static std::string G_YLIB_BUILD_TYPE_DEFINE       = \"${CMAKE_BUILD_TYPE}\";")
set(DECELARE_YLIB_BUILD_ARCH            "const static std::string G_YLIB_BUILD_ARCH_DEFINE       = \"${BUILD_YLIB_ARCH}\";")
set(DECELARE_YLIB_BUILD_PLATFORM        "const static std::string G_YLIB_BUILD_PLATFORM_DEFINE   = \"${BUILD_PLATFORM}\";")

#set(DECELARE_YLIB_BUILD_VERSION         "const static std::string G_YLIB_BUILD_VERSION_DEFINE    = ${BUILD_VERSION}")
set(DECELARE_YLIB_BUILD_VERSION_MAJOR   "const static int G_YLIB_BUILD_VERSION_MAJOR_DEFINE      = ${BUILD_VERSION_MAJOR};")
set(DECELARE_YLIB_BUILD_VERSION_MINOR   "const static int G_YLIB_BUILD_VERSION_MINOR_DEFINE      = ${BUILD_VERSION_MINOR};")
set(DECELARE_YLIB_BUILD_VERSION_PATCH   "const static int G_YLIB_BUILD_VERSION_PATCH_DEFINE      = ${BUILD_VERSION_PATCH};")

set(DECELARE_YLIB_BUILD_TIMESTAMP       "const static std::string G_YLIB_BUILD_TIMESTAMP_DEFINE  = \"${BUILD_TIMESTAMP}\";")
set(DECELARE_YLIB_BUILD_GIT_COMMIT_ID   "const static std::string G_YLIB_BUILD_COMMIT_ID_DEFINE  = \"${BUILD_GIT_COMMIT_ID}\";")
set(DECELARE_YLIB_BUILTIN_MODULE        "const static std::string G_YLIB_BUILTIN_MODULE_DEFINE   = \"${BUILTIN_MODULE_LIST}\";")


configure_file(ylib_extra_infos.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/ylib_extra_infos.cpp @ONLY)
