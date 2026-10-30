cmake_minimum_required(VERSION 3.14)
project(pair VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing linting using clang-tidy
add_executable(pair_td ./pair_td.cpp)
set_target_properties(pair_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

option(BUILD_TESTS "Build the unit tests" ON)
if(BUILD_TESTS)
    add_executable(pair_test ./tests/pair.cc)
    target_link_libraries(pair_test gtest_main gtest)
endif()
