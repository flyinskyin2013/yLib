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
        
    elseif(${BUILD_YLIB_ARCH} MATCHES "^armeabi$")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabi)
    elseif(${BUILD_YLIB_ARCH} MATCHES "^armeabihf$")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/armeabihf)
    elseif(${BUILD_YLIB_ARCH} MATCHES "aarch64")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/aarch64)
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
        ${CMAKE_CURRENT_SOURCE_DIR}/test_${module_name}.cpp)

    add_executable(test_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_${module_name} ylib ws2_32  ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib)
        else()
            target_link_libraries(test_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib_s)
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_${module_name}.exe
            DESTINATION examples
        )

    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            #target_link_libraries(test_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
            target_link_libraries(test_${module_name} libylib.so  -pthread  ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib)
        else()
            target_link_libraries(test_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib_s)
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_${module_name}
            DESTINATION examples
        )

    elseif(ANDROID)
        
    endif()       





endfunction()
#----------------------------------------------------------------------------------------

if(BUILD_MODULE_YCURL)

    build_module_examples(YCURL)

endif()
if(BUILD_MODULE_YHTTP)

    build_module_examples(YHTTP)

endif()

##########################################################################
if(BUILD_MODULE_YLOG)

    build_module_examples(YLOG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties.example log4cplus.properties @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/log4cplus.properties
        DESTINATION 
        examples
    )
endif()

##########################################################################

if(BUILD_MODULE_YSHELL)

    build_module_examples(YSHELL)

endif()


##########################################################################

if(BUILD_MODULE_YXML)

    build_module_examples(YXML)

    add_custom_command(TARGET test_yxml
        PRE_BUILD 
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test.xml ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Now, we will copy test.xml for example test_yxml"
        )
    
    INSTALL(FILES

        ${CMAKE_CURRENT_SOURCE_DIR}/test.xml
        DESTINATION 
        examples
    )
endif()




INSTALL(PROGRAMS 
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ylib_cmake_config_file.sh
    DESTINATION examples
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../examples/CMakeLists.txt.example CMakeLists.txt.example @ONLY)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt.example
    DESTINATION examples
)

##########################################################################
if(BUILD_MODULE_YCONFIG)

    build_module_examples(YCONFIG)

endif()


INSTALL(FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/test.conf
    DESTINATION 
    examples
)

##########################################################################
if(BUILD_MODULE_YJSON)

    build_module_examples(YJSON)

endif()


INSTALL(FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/test.json
    DESTINATION 
    examples
)
##########################################################################

if(BUILD_MODULE_YSHAREDMEMORY)

    add_executable(test_ysharedmemory_r test_ysharedmemory_r.cpp) 
    add_executable(test_ysharedmemory_w test_ysharedmemory_w.cpp) 


    # RUNTIME-rpath maybe be diffcult. 
    #    INSTALL(TARGETS  test_${module_name}
    #        RUNTIME DESTINATION examples
    #    )

    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_ysharedmemory_r ylib )
            target_link_libraries(test_ysharedmemory_w ylib )

            add_dependencies(test_ysharedmemory_r ylib)
            add_dependencies(test_ysharedmemory_w ylib)
        else()
            target_link_libraries(test_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib_s)
        endif()
        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_ysharedmemory_r.exe
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_ysharedmemory_w.exe
            DESTINATION examples
        )

    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_ysharedmemory_r ylib -pthread z lzma)
            # target_link_libraries(test_ysharedmemory_w ylib -pthread z lzma)
            target_link_libraries(test_ysharedmemory_r ylib -pthread )
            target_link_libraries(test_ysharedmemory_w ylib -pthread )
            add_dependencies(test_ysharedmemory_r ylib)
            add_dependencies(test_ysharedmemory_w ylib)
        else()
            target_link_libraries(test_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_${module_name} ylib_s)
        endif()

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_ysharedmemory_r
            ${CMAKE_CURRENT_BINARY_DIR}/test_ysharedmemory_w
            DESTINATION examples
        )

    elseif(ANDROID)
        
    endif()


endif()

##########################################################################
# set(BUILD_MODULE_YTEMP ON)
if(BUILD_MODULE_YTEMP)

    build_module_examples(YTEMP)

endif()



##########################################################################
# set(BUILD_MODULE_YMAIN ON)
if(BUILD_MODULE_YMAIN)

    #build program frame example
    add_executable(test_ymain
        ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ymain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ystart.cpp
        )
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            #target_link_libraries(test_ymain libylib.dll -lssl -lcrypto  -lz -pthread -lstdc++)
            target_link_libraries(test_ymain libylib.dll -lssl -lcrypto  -lz -pthread -lstdc++)
            add_dependencies(test_ymain ylib)
        else()
            target_link_libraries(test_ymain libylib_s.lib -lssl -lcrypto   -lz -pthread -lstdc++)
            add_dependencies(test_ymain ylib_s)
        endif()

    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_ymain libylib.so -lssl -lcrypto  -lz -pthread -lstdc++)
            target_link_libraries(test_ymain libylib.so -pthread -lstdc++)
            add_dependencies(test_ymain ylib)
        else()
            target_link_libraries(test_ymain libylib_s.so -lssl -lcrypto   -lz -pthread -lstdc++)
            add_dependencies(test_ymain ylib_s)
        endif()

    elseif(ANDROID)
        
    endif()


    INSTALL(PROGRAMS 
        ${CMAKE_CURRENT_BINARY_DIR}/test_ymain
        DESTINATION examples/ProgramFrameExamples
    )


endif()
