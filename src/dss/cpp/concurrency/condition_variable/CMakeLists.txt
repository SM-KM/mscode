cmake_minimum_required(VERSION 3.14)
project(condition_variable VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing linting using clang-tidy
add_executable(cv_td ./cv_td.cpp)
set_target_properties(cv_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

option(BUILD_TESTS "Build the unit tests" ON)
if(BUILD_TESTS)
    add_executable(cv_tests ./tests/cv_tests.cc)
    target_link_libraries(cv_tests gtest_main gtest)
endif()
