macro(ylib_precheck_cmake_necessary_args)
    
    # check necessary CMAKE-ARGS
    if (NOT DEFINED BUILD_YLIB_ARCH)
        message(FATAL_ERROR "User must define ARG: BUILD_YLIB_ARCH. The value of it can be one of 'x86|x86_64|armeabi|armeabihf|aarch64'")
    else()
        if(NOT BUILD_YLIB_ARCH  MATCHES "(x86|x86_64|armeabi|armeabihf|aarch64|android)")
            message(FATAL_ERROR "User must define ARG: BUILD_YLIB_ARCH. The value of it can be one of 'x86|x86_64|armeabi|armeabihf|aarch64'")
        endif()
    endif()


    if(NOT CMAKE_BUILD_TYPE  MATCHES "(Release|RELEASE|release|Debug|DEBUG|debug)")
        message(FATAL_ERROR "User must define ARG: CMAKE_BUILD_TYPE. The value of it can be one of 'Release|RELEASE|release|Debug|DEBUG|debug'")
    endif()
    
endmacro()


macro(ylib_precheck_compiler)

# CMAKE_<LANG>_COMPILER_ID
# Absoft = Absoft Fortran (absoft.com)
# ADSP = Analog VisualDSP++ (analog.com)
# AppleClang = Apple Clang (apple.com)
# Clang = LLVM Clang (clang.llvm.org)
# Cray = Cray Compiler (cray.com)
# Embarcadero, Borland = Embarcadero (embarcadero.com)
# G95 = G95 Fortran (g95.org)
# GNU = GNU Compiler Collection (gcc.gnu.org)
# HP = Hewlett-Packard Compiler (hp.com)
# Intel = Intel Compiler (intel.com)
# MIPSpro = SGI MIPSpro (sgi.com)
# MSVC = Microsoft Visual Studio (microsoft.com)
# PGI = The Portland Group (pgroup.com)
# PathScale = PathScale (pathscale.com)
# SDCC = Small Device C Compiler (sdcc.sourceforge.net)
# SunPro = Oracle Solaris Studio (oracle.com)
# TI = Texas Instruments (ti.com)
# TinyCC = Tiny C Compiler (tinycc.org)
# Watcom = Open Watcom (openwatcom.org)
# XL, VisualAge, zOS = IBM XL (ibm.com)

endmacro()

macro(ylib_check_msvc_version_for_cxx11)

    # (_MSC_VER = 1900, Visual Studio 2015), MSVC_VERSION is coming from macro '_MSC_VER' 
    if (MSVC_VERSION VERSION_LESS 1900)

        message(FATAL_ERROR "this msvc is older, please use vs2015+")

    endif()

endmacro()

macro(ylib_check_gcc_or_clang_version_for_cxx11)

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

        # https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)

            message(WARNING "some c++11 libraries may miss.")
            # https://gcc.gnu.org/projects/cxx-status.html#cxx11
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.1)

                message(FATAL_ERROR "this gcc doesn't support c++11.")

            endif()

        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

        # https://github.com/llvm-mirror/libcxx/commit/5fec82dc0db3623546038e4a86baa44f749e554f#diff-c330060c0d4b6fb493c2be0ff80a3f7e
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)

            message(WARNING "some c++11 libraries may miss.")
            # http://clang.llvm.org/cxx_status.html#cxx11
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)

                message(FATAL_ERROR "this clang doesn't support c++11.")

            endif()

        endif()
    else()

        message(FATAL_ERROR "Don't support this compiler=${CMAKE_CXX_COMPILER_ID} in Linux")

    endif()




endmacro()
