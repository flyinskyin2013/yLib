# The CMAKE_SYSTEM_NAME is the CMake-identifier of the target platform to build for.
SET ( CMAKE_SYSTEM_NAME Linux )
SET ( CMAKE_SYSTEM_PROCESSOR aarch64 )

# set CMAKE_SYSROOT by command-line
# set(CMAKE_SYSROOT /xxx)

# The CMAKE_<LANG>_COMPILER variables may be set to full paths, or to names of compilers to search for in standard locations.
# set CMAKE_C_COMPILER  and CMAKE_CXX_COMPILER by command-line
# SET ( CMAKE_C_COMPILER "aarch64-linux-gnu-gcc" )
# SET ( CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++" )

# set searching rules for CMAKE_SYSROOT
SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

# set CMAKE_C_FLAGS and CMAKE_CXX_FLAGS for compiler's flags
# SET ()

# other settings
#add_definitions(-D)
set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")

set(CMAKE_C_FLAGS " -march=armv8-a")
set(CMAKE_CXX_FLAGS " -march=armv8-a")

# Set the given cache <variable> (cache entry). Since cache entries are meant to provide user-settable 
# values this does not overwrite existing cache entries by default. Use the FORCE option to overwrite existing entries.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "pre c flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "pre c++ flags")