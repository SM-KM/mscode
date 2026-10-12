cmake_minimum_required(VERSION 3.14)
project(jthread VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing linting using clang-tidy
add_executable(jthread_td ./jthread_td.cpp)
set_target_properties(jthread_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

option(BUILD_TESTS "Build the unit tests" ON)
if(BUILD_TESTS) 
    add_executable(jthread_test ./tests/jthread.cc)
    target_link_libraries(jthread_test gtest_main gtest)
endif()


