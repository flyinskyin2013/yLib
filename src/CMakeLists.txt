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




#----------------------------------------------------------------------------------------
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/ycommon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yexception.cpp
    )

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


if(BUILD_MODULE_YHTTP)

    ylib_build_module(YHTTP)

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

if(BUILD_MODULE_YCONFIG)

    ylib_build_module(YCONFIG)

endif()

if(BUILD_MODULE_YJSON)

    ylib_build_module(YJSON)

endif()

if(BUILD_MODULE_YSHAREDMEMORY)

    ylib_build_module(YSHAREDMEMORY)

endif()

if(BUILD_MODULE_YBASICVALUE)

    ylib_build_module(YBASICVALUE)

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
    elseif(${BUILD_YLIB_ARCH} MATCHES "armeabi")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi)
    elseif(${BUILD_YLIB_ARCH} MATCHES "armeabi-v7a")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi-v7a)
    elseif(${BUILD_YLIB_ARCH} MATCHES "arm64-v8a")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/arm64-v8a)
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
    target_link_libraries( ylib
                                log4cppD.lib jsoncppD.lib libconfig++D.lib  ws2_32 Shlwapi)




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


#for header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/core DESTINATION include

    #PATTERN "curl" EXCLUDE
    #PATTERN "libxml" EXCLUDE
)

INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/ylib.hpp
    DESTINATION include
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

