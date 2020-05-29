#----------------------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
function(build_basic_algorithm name)

    string(TOLOWER ${name} module_name)
    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/basic_algorithm/${module_name}_tests.cpp)

    add_executable(test_algorithm_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_algorithm_${module_name} ylib ws2_32  ${DEP_LIBS})
        else()
            target_link_libraries(test_algorithm_${module_name} ylib_s   ${DEP_LIBS})
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_algorithm_${module_name}.exe
            DESTINATION tests/basic_algorithm/
        )

    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_algorithm_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
        else()
            target_link_libraries(test_algorithm_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_algorithm_${module_name}
            DESTINATION tests/basic_algorithm/
        )

    elseif(ANDROID)
        
    endif()       





endfunction()





function(build_module_examples name)

    string(TOLOWER ${name} module_name)
    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${module_name}/${module_name}_tests.cpp)

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
if(BUILD_MODULE_YSHELL)

    build_module_examples(YSHELL)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/yshell/shell_test.txt.example shell_test.txt @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/shell_test.txt
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YLOG)

    build_module_examples(YLOG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ylog/log4cplus.properties.example log4cplus.properties @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/log4cplus.properties
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YCONFIG)

    build_module_examples(YCONFIG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/yconfig/test.conf.example test.conf @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.conf
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YJSON)

    build_module_examples(YJSON)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/yjson/test.json.example test.json @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.json
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YXML)

    build_module_examples(yXML)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/yxml/test.xml.example test.xml @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.xml
        DESTINATION 
        tests
    )
endif()

##########################################################################


##########################################################################

build_module_examples(yObject)

build_module_examples(yException)

build_module_examples(yBasicValue)

build_module_examples(yCommon)

build_module_examples(yHttp)

build_module_examples(all_in_one)

##########################################################################

##########################################################################
build_basic_algorithm(ySequenceList)
##########################################################################