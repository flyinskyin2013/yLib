#------------------------------------option----------------------------------------------

#yLib lib-type
option(BUILD_STATIC_YLIB "Build static yLib " ON)
option(BUILD_SHARED_YLIB "Build shared yLib " ON)

#yLib build flags
#only used for utest
option(ENABLE_GNU_SANITIZER "enable gnu-sanitizer " OFF)
option(ENABLE_YLIB_COVERAGE "enable ylib code coverage " OFF)

#yLib built-in module
option(BUILD_MODULE_YHTTP "Build module-yhttp " ON)
option(BUILD_MODULE_YLOG "Build module-ylog " ON)
option(BUILD_MODULE_YSHELL "Build module-yshell " ON)
option(BUILD_MODULE_YXML "Build module-yxml " ON)
option(BUILD_MODULE_YCONFIG "Build module-yconfig " ON)
option(BUILD_MODULE_YJSON "Build module-yjson " ON)
option(BUILD_MODULE_YSHAREDMEMORY "Build module-ysharedmemory " ON)
option(BUILD_MODULE_YTCPSOCKET "Build module-ytcpsocket " ON)
option(BUILD_MODULE_YUDPSOCKET "Build module-yudpsocket " ON)

# yLib groups
option(BUILD_GROUP_IPC "Build group-ipc" ON)
option(BUILD_GROUP_NETWORK "Build group-network" ON)
option(BUILD_GROUP_UTILITY "Build group-utility" ON)

#yLib Framework
option(BUILD_MODULE_YTEMP "Build example module-ytemp" ON)
option(BUILD_MODULE_YMAIN "Build example module-ymain" ON)

option(BUILD_EXAMPLE "build example flags" ON)
option(BUILD_UNIT_TEST "build unit-test flags" ON)


#----------------------------------------------------------------------------------------

#------------------------------------global-vars-----------------------------------------
#define some var
# set(<variable> <value>[[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
# <variable> can be an environment variable such as: set( ENV{PATH} /home/martink )

#declare and clean var 
set(BUILD_PLATFORM  "" CACHE INTERNAL "build current platform" FORCE)
set(BUILD_VERSION "" CACHE INTERNAL "build current version" FORCE)
set(BUILD_TIMESTAMP "" CACHE INTERNAL "build current timestamp" FORCE)
set(BUILD_GIT_COMMIT_ID "" CACHE INTERNAL "build current git-commit-id" FORCE)


set(IPC_MODULE_LIST "" CACHE INTERNAL "ipc module list" FORCE)
set(NETWORK_MODULE_LIST "" CACHE INTERNAL "network module list" FORCE)
set(UTILITY_MODULE_LIST "" CACHE INTERNAL "utility module list" FORCE)

set(BUILTIN_TEST_LIST "" CACHE INTERNAL "build test list" FORCE)

if(NOT DEFINED STRIP)
    set(STRIP "strip" CACHE INTERNAL "strip command" FORCE)
endif()
#----------------------------------------------------------------------------------------