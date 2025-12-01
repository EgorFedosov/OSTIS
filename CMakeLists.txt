cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
project(ostis-example-app VERSION 0.10.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(AUTO_CCACHE "Use ccache to speed up rebuilds" ON)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM AND ${AUTO_CCACHE})
    message(STATUS "Using ${CCACHE_PROGRAM} as compiler launcher")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

find_package(sc-machine REQUIRED)
find_package(scl-machine REQUIRED)
find_package(ps-common-lib REQUIRED)

add_subdirectory(problem-solver)