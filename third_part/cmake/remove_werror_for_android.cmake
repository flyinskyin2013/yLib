    string(REPLACE "-Werror=format-security" "" NEW_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${NEW_CMAKE_CXX_FLAGS}")

    string(REPLACE "-Wformat" "" NEW_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${NEW_CMAKE_CXX_FLAGS} -Wall -Wno-error=implicit-int-float-conversion")

    string(REPLACE "-Werror=format-security" "" NEW_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "${NEW_CMAKE_C_FLAGS}")

    string(REPLACE "-Wformat" "" NEW_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "${NEW_CMAKE_C_FLAGS} -Wall -Wno-error=implicit-int-float-conversion")