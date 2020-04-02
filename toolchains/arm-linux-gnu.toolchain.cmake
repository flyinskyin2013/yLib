# The CMAKE_SYSTEM_NAME is the CMake-identifier of the target platform to build for.
SET ( CMAKE_SYSTEM_NAME Linux )
SET ( CMAKE_SYSTEM_PROCESSOR arm )

# set CMAKE_SYSROOT by command-line
# set(CMAKE_SYSROOT /xxx)

# The CMAKE_<LANG>_COMPILER variables may be set to full paths, or to names of compilers to search for in standard locations.
# set CMAKE_C_COMPILER  and CMAKE_CXX_COMPILER by command-line
# SET ( CMAKE_C_COMPILER "arm-linux-gnu-gcc" )
# SET ( CMAKE_CXX_COMPILER "arm-linux-gnu-g++" )

# set searching rules for CMAKE_SYSROOT
SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

# set CMAKE_C_FLAGS and CMAKE_CXX_FLAGS for compiler's flags
# SET ()

# other settings
#add_definitions(-D)