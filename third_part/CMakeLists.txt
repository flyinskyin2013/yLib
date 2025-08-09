cmake_minimum_required(VERSION 3.15)

project(ylib-third-part)

include(ExternalProject)


if (WIN32)
    include(CMakeLists_Win.cmake)
else()
    include(CMakeLists_Unix.cmake)
endif()