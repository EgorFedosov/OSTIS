file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp" "agent/*.cpp" "agent/*.hpp" "keynodes/*.hpp")

add_library(partners_module SHARED ${SOURCES})

target_link_libraries(partners_module LINK_PUBLIC sc-machine::sc-memory)
target_include_directories(partners_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(GTest REQUIRED) 
include(GoogleTest) 

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "test/*.cpp")

add_executable(partners_module-tests ${TEST_SOURCES})

target_link_libraries(partners_module-tests LINK_PRIVATE GTest::gtest_main LINK_PRIVATE partners_module)
target_include_directories(partners_module-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT DEFINED SC_EXTENSIONS_DIRECTORY)
    set(SC_EXTENSIONS_DIRECTORY "${CMAKE_BINARY_DIR}/lib/extensions")
endif()

set_target_properties(partners_module PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${SC_EXTENSIONS_DIRECTORY}")

gtest_discover_tests(partners_module-tests TEST_LIST ${TEST_SOURCES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)