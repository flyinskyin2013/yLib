#----------------------------------------------------------------------------------------
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/ycommon.cpp)

#----------------------------------------------------------------------------------------
if(BUILD_MODULE_YCURL)

set(SRC
    ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/ycurl.cpp)

endif()


if(BUILD_MODULE_YLOG)

set(SRC
    ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/ylog.cpp)

endif()


if(BUILD_MODULE_YLOG)

set(SRC
    ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/yshell.cpp)

endif()

#----------------------------------------------------------------------------------------

if(WIN32)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows_x86/)


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux_x86/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux_amd64/)

endif()

#----------------------------------------------------------------------------------------

if(BUILD_STATIC_YLIB)

add_library(yLib_s STATIC ${SRC})

endif()

if(BUILD_SHARED_YLIB)

add_library(yLib SHARED ${SRC})

endif()

if(WIN32)


elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")


if(BUILD_STATIC_YLIB)

target_link_libraries( yLib_s
                       libcurl.so )

endif()

if(BUILD_SHARED_YLIB)

target_link_libraries( yLib
                       libcurl.so )

endif()



endif()