if(BUILD_MODULE_YCURL)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ycurl.cpp)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)
#message(${CMAKE_CURRENT_BINARY_DIR})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux_amd64/)

add_executable(test_ycurl ${SRC})  



if(BUILD_STATIC_YLIB)
target_link_libraries(test_ycurl libyLib_s.a libcurl.so)
else()
target_link_libraries(test_ycurl libyLib.so libcurl.so)
endif()

endif()

##########################################################################
if(BUILD_MODULE_YCURL)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ylog.cpp)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)
#message(${CMAKE_CURRENT_BINARY_DIR})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux_amd64/)

add_executable(test_ylog ${SRC})  



if(BUILD_STATIC_YLIB)
target_link_libraries(test_ylog libyLib_s.a )
else()
target_link_libraries(test_ylog libyLib.so )
endif()

endif()

##########################################################################

if(BUILD_MODULE_YSHELL)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/test_yshell.cpp)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/)
#message(${CMAKE_CURRENT_BINARY_DIR})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux_amd64/)

add_executable(test_yshell ${SRC})  



if(BUILD_STATIC_YLIB)
target_link_libraries(test_yshell libyLib_s.a )
else()
target_link_libraries(test_yshell libyLib.so )
endif()

endif()