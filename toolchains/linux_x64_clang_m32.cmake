# On systems that have the uname command, this variable is set to the output of uname -s. 
# Linux, Windows, and Darwin for macOS are the values found on the big three operating systems.
SET ( CMAKE_HOST_SYSTEM_NAME Linux )


# On systems that support uname, this variable is set to the output of uname -p. 
# On Windows it is set to the value of the environment variable PROCESSOR_ARCHITECTURE.
SET ( CMAKE_HOST_SYSTEM_PROCESSOR x86_64 )


# This variable controls whether the CMAKE_FIND_ROOT_PATH and CMAKE_SYSROOT are used by find_program().
# If set to ONLY, then only the roots in CMAKE_FIND_ROOT_PATH will be searched. If set to NEVER, then 
# the roots in CMAKE_FIND_ROOT_PATH will be ignored and only the host system root will be used. If set 
# to BOTH, then the host system paths and the paths in CMAKE_FIND_ROOT_PATH will be searched.
SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )



# This variable controls whether the CMAKE_FIND_ROOT_PATH and CMAKE_SYSROOT are used by find_library().
# If set to ONLY, then only the roots in CMAKE_FIND_ROOT_PATH will be searched. If set to NEVER, 
# then the roots in CMAKE_FIND_ROOT_PATH will be ignored and only the host system root will be used. 
# If set to BOTH, then the host system paths and the paths in CMAKE_FIND_ROOT_PATH will be searched.
SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )



# This variable controls whether the CMAKE_FIND_ROOT_PATH and CMAKE_SYSROOT are used by find_file() 
# and find_path().
# If set to ONLY, then only the roots in CMAKE_FIND_ROOT_PATH will be searched. If set to NEVER, 
# then the roots in CMAKE_FIND_ROOT_PATH will be ignored and only the host system root will be used. 
# If set to BOTH, then the host system paths and the paths in CMAKE_FIND_ROOT_PATH will be searched.
SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )



# the gcc and g++ must be installed 
SET ( CMAKE_C_COMPILER "clang" )
SET ( CMAKE_CXX_COMPILER "clang++" )

# set c/c++ flags
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

# Set the given cache <variable> (cache entry). Since cache entries are meant to provide user-settable 
# values this does not overwrite existing cache entries by default. Use the FORCE option to overwrite existing entries.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "pre c flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "pre c++ flags")