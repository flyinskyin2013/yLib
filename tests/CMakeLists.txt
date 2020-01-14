#----------------------------------------------------------------------------------------
#add link dir
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)

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

    #set path to run test-examples
    link_directories(${CMAKE_INSTALL_PREFIX}/lib)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


elseif(ANDROID)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/android)
endif()



#set(CMAKE_SKIP_BUILD_RPATH FALSE)                 # 编译时加上RPATH  
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)         # 编译时RPATH不使用安装的RPATH  
#set(CMAKE_INSTALL_RPATH "")                       # 安装RPATH为空  
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)      # 安装的执行文件不加上RPATH
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

#set(CMAKE_SKIP_RPATH TRUE)
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)
#----------------------------------------------------------------------------------------
function(build_module_examples name)

    string(TOLOWER ${name} module_name)
    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_${module_name}/test_${module_name}.cpp)

    add_executable(test_unit_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_unit_${module_name} ylib ws2_32  ${DEP_LIBS})
        else()
            target_link_libraries(test_unit_${module_name} ylib_s   ${DEP_LIBS})
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_unit_${module_name}.exe
            DESTINATION tests
        )

    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_unit_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
        else()
            target_link_libraries(test_unit_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_unit_${module_name}
            DESTINATION tests
        )

    elseif(ANDROID)
        
    endif()       





endfunction()
#----------------------------------------------------------------------------------------


##########################################################################
if(BUILD_MODULE_YLOG)

    build_module_examples(YLOG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_ylog/log4cplus.properties.example log4cplus.properties @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/log4cplus.properties
        DESTINATION 
        tests
    )
endif()

##########################################################################
