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

elseif(ANDROID)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/android)
endif()
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
        
    if(BUILD_SHARED_YLIB)
        target_link_libraries(test_${module_name} libyLib.so ${DEP_LIBS})
    else()
        target_link_libraries(test_${module_name} libyLib_s.a ${DEP_LIBS})
    endif()


endfunction()
#----------------------------------------------------------------------------------------

if(BUILD_MODULE_YCURL)

    build_module_examples(YCURL libcurl.so)

endif()

##########################################################################
if(BUILD_MODULE_YLOG)

    build_module_examples(YLOG)

endif()

##########################################################################

if(BUILD_MODULE_YSHELL)

    build_module_examples(YSHELL)

endif()


##########################################################################

if(BUILD_MODULE_YXML)

    build_module_examples(YXML libxml2.so)

    add_custom_command(TARGET test_yxml
        PRE_BUILD 
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test.xml ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Now, we will copy test.xml for example test_yxml"
        )
endif()

