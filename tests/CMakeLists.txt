
#----------------------------------------------------------------------------------------
#add include dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)


#----------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------
#add ylib link dir
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)

#set(CMAKE_SKIP_BUILD_RPATH FALSE)                 # 编译时加上RPATH  
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)         # 编译时RPATH不使用安装的RPATH  
#set(CMAKE_INSTALL_RPATH "")                       # 安装RPATH为空  
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)      # 安装的执行文件不加上RPATH
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

#set(CMAKE_SKIP_RPATH TRUE)
#set(CMAKE_SKIP_INSTALL_RPATH TRUE)


# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-rpath=../lib")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath=../lib")

set(CMAKE_INSTALL_RPATH "../lib")     

#----------------------------------------------------------------------------------------

#Optimize compile-speed, we set a shared-target
add_library(YLIB_TESTS_SHARED_MAIN STATIC ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp)
set(DEP_LIBS
    YLIB_TESTS_SHARED_MAIN)

#----------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------
# define some macroes
function(AppendBuiltTestList val)

    set(BUILTIN_TEST_LIST ${BUILTIN_TEST_LIST} ${val} CACHE INTERNAL "set builtin test list" FORCE)

endfunction()

function(AddTwoProcessUnitTest test_name proc1_target proc2_target)

add_test(NAME ${test_name}
    COMMAND ${CMAKE_COMMAND} 
    -DTEST_EXECUTABLE=${CMAKE_CURRENT_SOURCE_DIR}/run_multi_process.sh 
    -DTEST_ARG0=$<TARGET_FILE:${proc1_target}> 
    -DTEST_ARG1=$<TARGET_FILE:${proc2_target}>
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmakes/execute_test.cmake)

endfunction()

function(build_tests group_name_ sub_dir_name_ module_name_)

    string(TOLOWER ${group_name_} group_name)
    
    string(TOLOWER ${module_name_} module_name)

    if("${sub_dir_name_}" STREQUAL "")

        set(SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/${group_name}/${module_name}_tests.cpp)

    else()  

        string(TOLOWER ${sub_dir_name_} sub_dir_name)
        set(SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/${group_name}/${sub_dir_name}/${module_name}_tests.cpp)

    endif()

    set(unit_test_target_name "unit_test_${module_name}")
    
    foreach(param ${ARGN})

        set(DEP_LIBS
            YLIB_TESTS_SHARED_MAIN
            ${DEP_LIBS}
            ${param})

    endforeach()

    if(BUILD_SHARED_YLIB)
        if(WIN32)


        elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

            set(DEP_LIBS
                ${DEP_LIBS}
                -pthread)
        elseif(CMAKE_SYSTEM_NAME MATCHES "Android")

            set(DEP_LIBS
                ${DEP_LIBS}
                -pthread log)
        endif()
        add_executable(${unit_test_target_name} ${SRC})  

        # we skip and specially process:
        # unit_test_ytcpclient/unit_test_ytcpserver
        # unit_test_yudpclient/unit_test_yudpserver
        # unit_test_ysharedmemory_r/unit_test_ysharedmemory_w
        if (${unit_test_target_name} MATCHES "yudp")
        elseif (${unit_test_target_name} MATCHES "ytcp")
        elseif (${unit_test_target_name} MATCHES "ysharedmemory")
        else()
            add_test(NAME ${unit_test_target_name} COMMAND ${CMAKE_COMMAND} -DTEST_EXECUTABLE=$<TARGET_FILE:${unit_test_target_name}> -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmakes/execute_test.cmake)
        endif()

        
        target_link_libraries(${unit_test_target_name} ylib ${DEP_LIBS})
        add_dependencies(${unit_test_target_name} ylib YLIB_TESTS_SHARED_MAIN)

        INSTALL(TARGETS  ${unit_test_target_name}
            RUNTIME DESTINATION tests 
        )
            
    endif()

    # we verify static ylib in test_ymain
    if(BUILD_STATIC_YLIB)

        if(WIN32)


        elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

            set(DEP_LIBS
                ${DEP_LIBS}
                -pthread)
        elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
            set(DEP_LIBS
                ${DEP_LIBS}
                -pthread log)
        endif()

        add_executable(${unit_test_target_name}_s ${SRC}) 
        set_target_properties(${unit_test_target_name}_s PROPERTIES COMPILE_DEFINITIONS "BUILD_YLIB_STATIC_LIB")
        #add_test(NAME ${unit_test_target_name}_s COMMAND ${CMAKE_COMMAND} -DTEST_EXECUTABLE=$<TARGET_FILE:${unit_test_target_name}_s> -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmakes/execute_test.cmake)
        
        if(WIN32)
            target_link_libraries(${unit_test_target_name}_s ylib_s ${DEP_LIBS} Ws2_32)
        elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
            target_link_libraries(${unit_test_target_name}_s ylib_s ${DEP_LIBS})

        elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
            target_link_libraries(${unit_test_target_name}_s ylib_s ${DEP_LIBS} log)
        endif()

        add_dependencies(${unit_test_target_name}_s ylib_s YLIB_TESTS_SHARED_MAIN)

        INSTALL(TARGETS  ${unit_test_target_name}_s
            RUNTIME DESTINATION tests 
        )
    endif()

    AppendBuiltTestList(${unit_test_target_name})
endfunction()

#----------------------------------------------------------------------------------------

#---------------------------------------Unit Test For Utility Group----------------------
##########################################################################
if(BUILD_MODULE_YSHELL)

    build_tests(UTILITY yShell yShell)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yshell/shell_test.txt.example shell_test.txt @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/shell_test.txt
        DESTINATION 
        tests
    )
endif()

##########################################################################



##########################################################################
if(BUILD_MODULE_YCONFIG)

    build_tests(UTILITY yConfig yConfig)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yconfig/test.conf.example test.conf @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yconfig/test_fail.conf.example test_fail.conf @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.conf
        ${CMAKE_CURRENT_BINARY_DIR}/test_fail.conf
        DESTINATION 
        tests
    )
endif()

##########################################################################

##########################################################################
if(BUILD_MODULE_YJSON)

    build_tests(UTILITY yJson yJson)

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

    build_tests(UTILITY yXml yXml)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utility/yxml/test.xml.example test.xml @ONLY)

    INSTALL(FILES

        ${CMAKE_CURRENT_BINARY_DIR}/test.xml
        DESTINATION 
        tests
    )

endif()

if(BUILD_MODULE_YHTTP)
    build_tests(UTILITY yHttp yHttp)
endif()
##########################################################################

build_tests(UTILITY yTimer yTimer)

#----------------------------------------------------------------------------------------

#---------------------------------------Unit Test For Network Group----------------------

if(BUILD_MODULE_YTCPSOCKET)
    build_tests(NETWORK TCP yTcpserver)
    build_tests(NETWORK TCP yTcpClient)

    AddTwoProcessUnitTest(unit_test_ytcp unit_test_ytcpserver unit_test_ytcpclient)
endif()

if(BUILD_MODULE_YUDPSOCKET)
    build_tests(NETWORK UDP yUdpClient)
    build_tests(NETWORK UDP yUdpServer)

    AddTwoProcessUnitTest(unit_test_yudp unit_test_yudpserver unit_test_yudpclient)
endif()

##########################################################################
#----------------------------------------------------------------------------------------

#----------------------------------Unit Test For IPC Group------------------------------
##########################################################################
if (BUILD_MODULE_YSHAREDMEMORY)
    build_tests(IPC ySharedMemory ySharedMemory_R)
    build_tests(IPC ySharedMemory ySharedMemory_W)

    AddTwoProcessUnitTest(unit_test_ysharedmemory unit_test_ysharedmemory_r unit_test_ysharedmemory_w)
endif()
##########################################################################
#----------------------------------------------------------------------------------------


#----------------------------------Unit Test For Core Group------------------------------

##########################################################################
# core built-in module
build_tests(CORE yObject yObject)

build_tests(CORE yException yException)

build_tests(CORE yCommon yCommon)

build_tests(CORE yBasicValue yBasicValue)

build_tests(CORE yLib_Extra_Infos yLib_Extra_Infos)

build_tests(CORE yAllocator yAllocator)

build_tests(CORE yThreadPool yThreadPool)

# for yLog
build_tests(CORE yLog yLog)
build_tests(CORE yLog yLog_Verify)
build_tests(CORE yLog ylog_file_bakup)
build_tests(CORE yLog ylog_capi)

##########################################################################

#----------------------------------------------------------------------------------------

##########################################################################
build_tests(all_in_one "" all_in_one)
##########################################################################



#----------------------------------Unit Test For Core Group------------------------------
##########################################################################
# basic_algorithm module
build_tests(Basic_Algorithm "" yLinearList)
build_tests(Basic_Algorithm "" yLinkedList)
build_tests(Basic_Algorithm "" yStack)
build_tests(Basic_Algorithm "" yQueue)
##########################################################################
#----------------------------------------------------------------------------------------



#----------------------------------Unit Test For TMP Group------------------------------
build_tests(tmp "" tmp)
#----------------------------------------------------------------------------------------




#----------------------------------install test scripts----------------------------------

INSTALL(FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/scripts_for_tests.sh
    DESTINATION 
    tests
    PERMISSIONS
    OWNER_READ
    OWNER_EXECUTE
)
#----------------------------------------------------------------------------------------