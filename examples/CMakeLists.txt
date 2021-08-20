
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

#----------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------

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

if(BUILD_MODULE_YMAIN)

    #build program frame example
    add_executable(test_ymain_s
            ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ymain.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ystart.cpp
            )

    add_executable(test_ymain
            ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ymain.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ProgramFrameExamples/ystart.cpp
            )
    # we verify shared-ylib in unit-tests
    if(BUILD_SHARED_YLIB)

        target_link_libraries(test_ymain ylib)
        add_dependencies(test_ymain ylib)

    endif()

    if(BUILD_STATIC_YLIB)

        target_link_libraries(test_ymain_s ylib_s -pthread)
        add_dependencies(test_ymain_s ylib_s)

    endif()
      
    INSTALL(PROGRAMS 
        ${CMAKE_CURRENT_BINARY_DIR}/test_ymain
        DESTINATION examples/ProgramFrameExamples
    )
    INSTALL(PROGRAMS 
        ${CMAKE_CURRENT_BINARY_DIR}/test_ymain_s
        DESTINATION examples/ProgramFrameExamples
    )

endif()

