#link other whole-archive to libylib.so or libylib.a
#set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive")

#----------------------------------------set core source---------------------------------
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ycommon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/yobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/yexception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ybasicvalue.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/ylib_extra_infos.cpp
    )

#----------------------------------------------------------------------------------------

#----------------------------------------define cmake macroes----------------------------
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

    if(${class_dir_name} MATCHES "ipc")
        set(IPC_MODULE_LIST ${IPC_MODULE_LIST} ${name}  CACHE STRING "set ipc-module-list" FORCE)
    elseif(${class_dir_name} MATCHES "network")
        set(NETWORK_MODULE_LIST ${NETWORK_MODULE_LIST} ${name}  CACHE STRING "set network-module-list" FORCE)
    elseif(${class_dir_name} MATCHES "utility")
        set(UTILITY_MODULE_LIST ${UTILITY_MODULE_LIST} ${name}  CACHE STRING "set utility-module-list" FORCE)
    endif()

   
endmacro()

#----------------------------------------------------------------------------------------

#----------------------------------------check utility group-----------------------------

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

ylib_build(UTILITY yTimer)
#----------------------------------------------------------------------------------------

#----------------------------------------check ipc group---------------------------------
if(BUILD_MODULE_YSHAREDMEMORY)

    ylib_build(IPC YSHAREDMEMORY)

endif()

#----------------------------------------------------------------------------------------

#----------------------------------------check network group-----------------------------
if(WIN32)


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    ylib_build(NETWORK YABSTRACTSOCKET)

elseif(ANDROID)
    
endif()

if(BUILD_MODULE_YTCPSERVER)

    ylib_build(NETWORK YTCPSERVER)

endif()


if(BUILD_MODULE_YTCPSOCKET)

    ylib_build(NETWORK YTCPSOCKET)

endif()

if(BUILD_MODULE_YUDPSOCKET)

    ylib_build(NETWORK YUDPSOCKET)

endif()
#----------------------------------------------------------------------------------------

#----------------------------------set link dir------------------------------------------

if(WIN32)

    if(${BUILD_YLIB_ARCH} MATCHES "(x86_64|x86)")

        #64bits ylib x86_64
        #32bits ylib x86
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows/${BUILD_YLIB_ARCH})

    else()

        message(FATAL_ERROR "Don't support this arch ${BUILD_YLIB_ARCH} in windows.")

    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if("${BUILD_YLIB_ARCH}" MATCHES "^(x86_64|x86|armeabi|armeabihf|aarch64)$")

        #linux 64bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/${BUILD_YLIB_ARCH})
    
    else()

        message(FATAL_ERROR "Don't support this arch ${BUILD_YLIB_ARCH} in linux.")

    endif()

elseif(ANDROID)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/android)

endif()

#----------------------------------------------------------------------------------------

#----------------------------------------set ylib property-------------------------------

if(WIN32)

    if(BUILD_STATIC_YLIB)

        add_library(ylib_s STATIC ${SRC})

        if(CMAKE_BUILD_TYPE MATCHES "Debug")

            target_link_libraries( ylib_s PRIVATE jsoncppD.lib libconfig++D.lib libcurlD.lib libxml2D_a.lib  ws2_32 Shlwapi)
            add_custom_command(TARGET ylib_s POST_BUILD
                COMMAND lib.exe /out:ylib_s_new.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/jsoncppD.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libconfig++D.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libcurlD.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libxml2D_a.lib ${CMAKE_CURRENT_BINARY_DIR}/Debug/ylib_s.lib
                COMMAND ${CMAKE_COMMAND} -E rename ylib_s_new.lib ${CMAKE_CURRENT_BINARY_DIR}/Debug/ylib_s.lib
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            )
        elseif (CMAKE_BUILD_TYPE MATCHES "Release")

            target_link_libraries( ylib_s PRIVATE jsoncpp.lib libconfig++.lib libcurl.lib libxml2_a.lib ws2_32 Shlwapi)
            add_custom_command(TARGET ylib_s POST_BUILD
                COMMAND lib.exe /out:ylib_s_new.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/jsoncpp.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libconfig++.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libcurl.lib ${PROJECT_SOURCE_DIR}/lib/windows/${BUILD_YLIB_ARCH}/libxml2_a.lib ${CMAKE_CURRENT_BINARY_DIR}/Release/ylib_s.lib
                COMMAND ${CMAKE_COMMAND} -E rename ylib_s_new.lib ${CMAKE_CURRENT_BINARY_DIR}/Release/ylib_s.lib
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            )
        endif()
        
        set_target_properties(ylib_s PROPERTIES COMPILE_DEFINITIONS "CURL_STATICLIB;BUILD_YLIB_WITH_EXPORT")
        # set_target_properties(ylib_s PROPERTIES COMPILE_DEFINITIONS CURL_STATICLIB)
        # set_target_properties(ylib_s PROPERTIES COMPILE_DEFINITIONS BUILD_YLIB_WITH_EXPORT)
        
    endif()

    if(BUILD_SHARED_YLIB)

        add_library(ylib SHARED ${SRC})
        if(CMAKE_BUILD_TYPE MATCHES "Debug")

            target_link_libraries( ylib PRIVATE jsoncppD.lib libconfig++D.lib libcurlD.lib libxml2D_a.lib  ws2_32 Shlwapi)

        elseif (CMAKE_BUILD_TYPE MATCHES "Release")

            target_link_libraries( ylib PRIVATE jsoncpp.lib libconfig++.lib libcurl.lib libxml2_a.lib ws2_32 Shlwapi)

        endif()
        set_target_properties(ylib PROPERTIES COMPILE_DEFINITIONS "CURL_STATICLIB;BUILD_YLIB_WITH_EXPORT")
        # set_target_properties(ylib PROPERTIES COMPILE_DEFINITIONS CURL_STATICLIB)
        # set_target_properties(ylib PROPERTIES COMPILE_DEFINITIONS BUILD_YLIB_WITH_EXPORT)
    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_STATIC_YLIB)

        add_library(ylib_s STATIC ${SRC})
        target_link_libraries( ylib_s
                                    PRIVATE 
                                    libcurl.a
                                    libxml2.a 
                                    liblibconfig++.a 
                                    libjsoncpp.a)

        # add_custom_command(TARGET target
        # PRE_BUILD | PRE_LINK | POST_BUILD
        # COMMAND command1 [ARGS] [args1...]
        # [COMMAND command2 [ARGS] [args2...] ...]
        # [WORKING_DIRECTORY dir]
        # [COMMENT comment] [VERBATIM])
        add_custom_command(TARGET ylib_s
                            PRE_BUILD
                            COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/3rd_all_objs
                            COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/3rd_all_objs
                            COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/libylib_s.a
                            )

        add_custom_command(TARGET ylib_s
                            POST_BUILD
                            COMMAND ar -x ${PROJECT_SOURCE_DIR}/lib/linux/${BUILD_YLIB_ARCH}/libcurl.a
                            COMMAND ar -x ${PROJECT_SOURCE_DIR}/lib/linux/${BUILD_YLIB_ARCH}/libxml2.a
                            COMMAND ar -x ${PROJECT_SOURCE_DIR}/lib/linux/${BUILD_YLIB_ARCH}/liblibconfig++.a
                            COMMAND ar -x ${PROJECT_SOURCE_DIR}/lib/linux/${BUILD_YLIB_ARCH}/libjsoncpp.a
                            COMMAND ar -x ${CMAKE_CURRENT_BINARY_DIR}/libylib_s.a
                            COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/libylib_s.a
                            COMMAND ar -cr ../libylib_s.a ./*.o
                            # update symbol
                            COMMAND ranlib ../libylib_s.a 
                            COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/3rd_all_objs
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/3rd_all_objs
                            )
    endif()

    if(BUILD_SHARED_YLIB)

        add_library(ylib SHARED ${SRC})
        target_compile_options(ylib PRIVATE -fPIC)
        if(ENABLE_YLIB_COVERAGE)

            # -rdynamic for yException
            target_compile_options(ylib PRIVATE -coverage -fprofile-arcs -ftest-coverage -rdynamic)
            # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -coverage -fprofile-arcs -ftest-coverage")
            # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage -fprofile-arcs -ftest-coverage")
            target_link_libraries(ylib PRIVATE -coverage -lgcov libcurl.a libxml2.a liblibconfig++.a libjsoncpp.a)
        
        else()

            target_link_libraries(ylib
                            PRIVATE libcurl.a libxml2.a liblibconfig++.a libjsoncpp.a)
        endif()

        set_target_properties(ylib PROPERTIES VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" SOVERSION ${PROJECT_VERSION_MAJOR})

        # strip lib
        add_custom_command(TARGET ylib
            POST_BUILD
            COMMAND strip libylib.so
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
        )
    endif()

elseif(ANDROID)
else()
endif()

#----------------------------------------------------------------------------------------

#set(CMAKE_SKIP_BUILD_RPATH FALSE)                 # 编译时加上RPATH  
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)         # 编译时RPATH不使用安装的RPATH  
#set(CMAKE_INSTALL_RPATH "")                       # 安装RPATH为空  
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)      # 安装的执行文件不加上RPATH
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

#----------------------------------------set install property----------------------------
if(WIN32)

    if(BUILD_STATIC_YLIB)

        # For lib files
        # Executables are treated as RUNTIME targets, except that those marked with the MACOSX_BUNDLE property are treated as BUNDLE targets on OS X
        # Static libraries are always treated as ARCHIVE targets.
        # Module libraries are always treated as LIBRARY targets. 
        INSTALL(TARGETS  ylib_s
                ARCHIVE DESTINATION lib
        )

    endif()

    if(BUILD_SHARED_YLIB)

        # For lib files
        # Executables are treated as RUNTIME targets, except that those marked with the MACOSX_BUNDLE property are treated as BUNDLE targets on OS X
        # Static libraries are always treated as ARCHIVE targets.
        # Module libraries are always treated as LIBRARY targets. 
        INSTALL(TARGETS  ylib
                LIBRARY DESTINATION lib
        )

        INSTALL(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/ylib.dll
                DESTINATION examples
        )

        INSTALL(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/ylib.dll
                DESTINATION lib
        )

        INSTALL(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/ylib.dll
                DESTINATION tests
        )

    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    if(BUILD_STATIC_YLIB)

        #for lib files
        INSTALL(TARGETS  ylib ylib_s
            #RUNTIME DESTINATION
            LIBRARY DESTINATION lib 
            ARCHIVE DESTINATION lib
        )

    endif()


    INSTALL(TARGETS  ylib
        #RUNTIME DESTINATION
        LIBRARY DESTINATION tests 
    )

    INSTALL(TARGETS  ylib
        #RUNTIME DESTINATION
        LIBRARY DESTINATION examples 
    )

elseif(ANDROID)


endif()


#for core header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/core DESTINATION include/yLib)

#for ipc header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ipc DESTINATION include/yLib)

#for utility header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/utility DESTINATION include/yLib)

#for network header files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/network DESTINATION include/yLib)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/basic_algorithm DESTINATION include/yLib)

INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/ylib.hpp
    DESTINATION include/yLib
)

#for yLibConfig.cmake
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmakes/yLibConfig.cmake.example yLibConfig.cmake @ONLY)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/yLibConfig.cmake
    DESTINATION cmake
)
#----------------------------------------------------------------------------------------



#-----------------------------------generate ylib_build_info.hpp-------------------------
set(DECELARE_YLIB_BUILD_TYPE                "const static std::string G_YLIB_BUILD_TYPE_DEFINE               = \"${CMAKE_BUILD_TYPE}\";")
set(DECELARE_YLIB_BUILD_ARCH                "const static std::string G_YLIB_BUILD_ARCH_DEFINE               = \"${BUILD_YLIB_ARCH}\";")
set(DECELARE_YLIB_BUILD_PLATFORM            "const static std::string G_YLIB_BUILD_PLATFORM_DEFINE           = \"${BUILD_PLATFORM}\";")
            
#set(DECELARE_YLIB_BUILD_VERSION             "const static std::string G_YLIB_BUILD_VERSION_DEFINE            = ${BUILD_VERSION}")
set(DECELARE_YLIB_BUILD_VERSION_MAJOR       "const static int G_YLIB_BUILD_VERSION_MAJOR_DEFINE              = ${PROJECT_VERSION_MAJOR};")
set(DECELARE_YLIB_BUILD_VERSION_MINOR       "const static int G_YLIB_BUILD_VERSION_MINOR_DEFINE              = ${PROJECT_VERSION_MINOR};")
set(DECELARE_YLIB_BUILD_VERSION_PATCH       "const static int G_YLIB_BUILD_VERSION_PATCH_DEFINE              = ${PROJECT_VERSION_PATCH};")
            
set(DECELARE_YLIB_BUILD_TIMESTAMP           "const static std::string G_YLIB_BUILD_TIMESTAMP_DEFINE          = \"${BUILD_TIMESTAMP}\";")
set(DECELARE_YLIB_BUILD_GIT_COMMIT_ID       "const static std::string G_YLIB_BUILD_COMMIT_ID_DEFINE          = \"${BUILD_GIT_COMMIT_ID}\";")
set(DECELARE_YLIB_BUILTIN_IPC_MODULE        "const static std::string G_YLIB_BUILTIN_IPC_MODULE_DEFINE       = \"${IPC_MODULE_LIST}\";")
set(DECELARE_YLIB_BUILTIN_NETWORK_MODULE    "const static std::string G_YLIB_BUILTIN_NETWORK_MODULE_DEFINE   = \"${NETWORK_MODULE_LIST}\";")
set(DECELARE_YLIB_BUILTIN_UTILITY_MODULE    "const static std::string G_YLIB_BUILTIN_UTILITY_MODULE_DEFINE   = \"${UTILITY_MODULE_LIST}\";")

configure_file(ylib_extra_infos.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/ylib_extra_infos.cpp @ONLY)
#----------------------------------------------------------------------------------------

if(BUILD_STATIC_YLIB)

# 
# clean ylib_s dep-lib-list for example and tests
# get_target_property(VARMY ylib_s  INTERFACE_LINK_LIBRARIES)
# message(${VARMY})
set_target_properties(ylib_s PROPERTIES INTERFACE_LINK_LIBRARIES "")
# 

endif()