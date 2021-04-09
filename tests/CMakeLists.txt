#----------------------------------------------------------------------------------------
#add include dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#----------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------
#add link dir
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)

if(WIN32)

    if(${BUILD_YLIB_ARCH} MATCHES "(x86_64|X86_64)")
        #windows 64bits ylib
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows/x86_64)
    else()
        #windows 32bits ylib
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

#For compile-speed, we set a shared-target
add_library(YLIB_TESTS_SHARED_MAIN STATIC ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp)
set(DEP_LIBS
YLIB_TESTS_SHARED_MAIN)

function(AppendBUILTIN_TEST_LIST val)

    set(BUILTIN_TEST_LIST ${BUILTIN_TEST_LIST} ${val} CACHE STRING INTERNAL FORCE)
endfunction()


function(build_basic_algorithm name)

    string(TOLOWER ${name} module_name)
    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/basic_algorithm/${module_name}_tests.cpp)

    add_executable(test_algorithm_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        YLIB_TESTS_SHARED_MAIN
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_algorithm_${module_name} ylib ws2_32  ${DEP_LIBS})
            add_dependencies(test_algorithm_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_algorithm_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_algorithm_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_algorithm_${module_name}.exe
            DESTINATION tests/basic_algorithm/
        )

        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name}.exe)
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_algorithm_${module_name}.exe" CACHE STRING INSTERNAL FORCE)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_algorithm_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
            target_link_libraries(test_algorithm_${module_name} libylib.so -pthread  ${DEP_LIBS})
            add_dependencies(test_algorithm_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_algorithm_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
            add_dependencies(test_algorithm_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_algorithm_${module_name}
            DESTINATION tests/basic_algorithm/
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_algorithm_${module_name}" CACHE STRING INSTERNAL FORCE)
        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name})
    elseif(ANDROID)
        
    endif()       

endfunction()





function(build_module_examples class_name name)

    # parse args
    string(TOLOWER ${name} module_name)
    string(TOLOWER ${class_name} class_dir_name)

    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/${class_dir_name}/${module_name}/${module_name}_tests.cpp)

    add_executable(test_unit_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        YLIB_TESTS_SHARED_MAIN
        ${DEP_LIBS}
        ${param})
    endforeach()
    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_unit_${module_name} ylib ws2_32  ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_unit_${module_name}.exe
            DESTINATION tests
        )

        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${module_name}.exe" CACHE STRING INSTERNAL FORCE )
        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name}.exe)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_unit_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
            target_link_libraries(test_unit_${module_name} libylib.so -pthread  ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_unit_${module_name}
            DESTINATION tests
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${module_name}" CACHE STRING INSTERNAL FORCE )
        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name})
    elseif(ANDROID)
        
    endif()       

endfunction()

function(build_module_examples1 name)

    # parse args
    string(TOLOWER ${name} module_name)

    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/${module_name}/${module_name}_tests.cpp)

    add_executable(test_unit_${module_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        YLIB_TESTS_SHARED_MAIN
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_unit_${module_name} ylib ws2_32  ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${module_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_unit_${module_name}.exe
            DESTINATION tests
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${module_name}.exe"  CACHE STRING INSTERNAL FORCE)
        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name}.exe)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_unit_${module_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
            target_link_libraries(test_unit_${module_name} libylib.so -pthread  ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${module_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
            add_dependencies(test_unit_${module_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${module_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_unit_${module_name}
            DESTINATION tests
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${module_name}"  CACHE STRING INSTERNAL FORCE)
        AppendBUILTIN_TEST_LIST(test_algorithm_${module_name})
    elseif(ANDROID)
        
    endif()       

endfunction()


function(build_module_examples3 class_name_ module_name_ target_name_)

    # parse args
    string(TOLOWER ${target_name_} target_name)
    string(TOLOWER ${class_name_} class_name)
    string(TOLOWER ${module_name_} module_name)

    set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/${class_name}/${module_name}/${target_name}_tests.cpp)

    add_executable(test_unit_${target_name} ${SRC})  

    foreach(param ${ARGN})
        #message("dep lib  ${param}")
        set(DEP_LIBS
        YLIB_TESTS_SHARED_MAIN
        ${DEP_LIBS}
        ${param})
    endforeach()

    #message("dep lib  ${DEP_LIBS}")
    if(WIN32)

        if(BUILD_SHARED_YLIB)
            target_link_libraries(test_unit_${target_name} ylib ws2_32  ${DEP_LIBS})
            add_dependencies(test_unit_${target_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${target_name} ylib_s   ${DEP_LIBS})
            add_dependencies(test_unit_${target_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()

        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${target_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_unit_${target_name}.exe
            DESTINATION tests
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${target_name}.exe"  CACHE STRING INSTERNAL FORCE)
        AppendBUILTIN_TEST_LIST(test_algorithm_${target_name}.exe)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

        if(BUILD_SHARED_YLIB)
            # target_link_libraries(test_unit_${target_name} libylib.so  -lssl -lcrypto -lz -pthread  ${DEP_LIBS})
            target_link_libraries(test_unit_${target_name} libylib.so -pthread  ${DEP_LIBS})
            add_dependencies(test_unit_${target_name} ylib YLIB_TESTS_SHARED_MAIN)
        else()
            target_link_libraries(test_unit_${target_name} libylib_s.so  -lssl -lcrypto  -lz -pthread ${DEP_LIBS})
            add_dependencies(test_unit_${target_name} ylib_s YLIB_TESTS_SHARED_MAIN)
        endif()
        # RUNTIME-rpath maybe be diffcult.
        #    INSTALL(TARGETS  test_${target_name}
        #        RUNTIME DESTINATION examples
        #    )

        INSTALL(PROGRAMS 
            ${CMAKE_CURRENT_BINARY_DIR}/test_unit_${target_name}
            DESTINATION tests
        )
        #set(BUILTIN_TEST_LIST "${BUILTIN_TEST_LIST}\n test_unit_${target_name}"  CACHE STRING INSTERNAL FORCE)
        AppendBUILTIN_TEST_LIST(test_algorithm_${target_name})
    elseif(ANDROID)
        
    endif()       

endfunction()


#----------------------------------------------------------------------------------------
##########################################################################
if(BUILD_MODULE_YSHELL)

    build_module_examples(UTILITY YSHELL)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yshell/shell_test.txt.example shell_test.txt @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/shell_test.txt
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YLOG)

    build_module_examples(UTILITY YLOG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/ylog/log4cplus.properties.example log4cplus.properties @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/log4cplus.properties
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YCONFIG)

    build_module_examples(UTILITY YCONFIG)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yconfig/test.conf.example test.conf @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.conf
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YJSON)

    build_module_examples(UTILITY YJSON)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yjson/test.json.example test.json @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.json
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YXML)

    build_module_examples(UTILITY yXML)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yxml/test.xml.example test.xml @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.xml
        DESTINATION 
        tests
    )
endif()

if(BUILD_MODULE_YHTTP)
    build_module_examples(UTILITY yHttp)
endif()
##########################################################################

if(BUILD_MODULE_YTCPSERVER)
    build_module_examples3(NETWORK TCP yTcpserver)
endif()

if(BUILD_MODULE_YTCPSOCKET)
    build_module_examples3(NETWORK TCP yTcpClient)
endif()

if(BUILD_MODULE_YUDPSERVER)
    build_module_examples3(NETWORK UDP yUdpServer)
endif()

if(BUILD_MODULE_YUDPSOCKET)
    build_module_examples3(NETWORK UDP yUdpClient)
endif()














##########################################################################
# core built-in module
build_module_examples(CORE yObject)

build_module_examples(CORE yException)

build_module_examples(CORE yCommon)

build_module_examples(CORE yBasicValue)

build_module_examples(UTILITY yTimer)

##########################################################################
# core module

    





build_module_examples1(all_in_one)


##########################################################################

##########################################################################
# basic_algorithm module
build_basic_algorithm(yLinearList)
build_basic_algorithm(yLinkedList)
build_basic_algorithm(yStack)
build_basic_algorithm(yQueue)
##########################################################################

INSTALL(FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/scripts_for_tests.sh
    DESTINATION 
    tests
    PERMISSIONS
    OWNER_READ
    OWNER_EXECUTE
)

INSTALL(FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/scripts_for_all_tests.sh
    DESTINATION 
    tests
    PERMISSIONS
    OWNER_READ
    OWNER_EXECUTE
)