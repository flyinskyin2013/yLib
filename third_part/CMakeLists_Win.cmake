if (NOT DEFINED compile_tool_set)
message(FATAL_ERROR  "must set -Dcompile_tool_set, eg: v140/v141/v142/v143")
endif ()

if (NOT DEFINED compile_tool_arch)
message(FATAL_ERROR  "must set -Dcompile_tool_arch, eg: Win32/x64")
endif ()


# JSONCPP
ExternalProject_Add(
    jsoncpp
    URL https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz
    URL_HASH MD5=fa47a3ab6b381869b6a5f20811198662
    CONFIGURE_COMMAND cmake -T ${compile_tool_set},host=x64 -A ${compile_tool_arch} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} <SOURCE_DIR>
    BUILD_COMMAND cmake --build . --config Release --target install
    INSTALL_COMMAND echo "install step: donothing ... ..."
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# CURL
# need sed
ExternalProject_Add(
    curl
    URL https://github.com/curl/curl/releases/download/curl-7_55_1/curl-7.55.1.tar.gz
    URL_HASH MD5=3b832160a8c9c40075fd71191960307c
    PATCH_COMMAND sed -i "s/find_package(Perl REQUIRED)/#find_package(Perl REQUIRED)/g" <SOURCE_DIR>/CMakeLists.txt
    CONFIGURE_COMMAND cmake -T ${compile_tool_set},host=x64 -A ${compile_tool_arch} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DHTTP_ONLY=ON -DBUILD_TESTING=OFF -DCURL_STATICLIB=ON -DBUILDING_LIBCURL=1 <SOURCE_DIR>
    BUILD_COMMAND cmake --build . --config Release --target install
    INSTALL_COMMAND echo "install step: donothing ... ..."
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# LIBXML2
ExternalProject_Add(
    libxml2
    URL https://github.com/GNOME/libxml2/archive/v2.9.9.tar.gz
    URL_HASH MD5=435110d1f26268f1952350b344a3a934
    CONFIGURE_COMMAND cd <SOURCE_DIR>\\win32 && cscript configure.js compiler=msvc  incdir=${CMAKE_INSTALL_PREFIX}/include libdir=${CMAKE_INSTALL_PREFIX}/lib static=yes iconv=no
    BUILD_COMMAND cd <SOURCE_DIR>\\win32 && nmake /f Makefile.msvc 
    INSTALL_COMMAND powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ${CMAKE_CURRENT_SOURCE_DIR}/install_libxml_win.ps1 <SOURCE_DIR> ${CMAKE_INSTALL_PREFIX}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
