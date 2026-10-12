cmake_minimum_required(VERSION 3.14)
project(semaphore VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing linting using clang-tidy
add_executable(semaphore_td ./semaphore_td.cpp)
set_target_properties(semaphore_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

option(BUILD_TESTS "Build the unit tests" ON)
if(BUILD_TESTS) 
    add_executable(semaphore_test ./tests/semaphore.cc)
    target_link_libraries(semaphore_test gtest_main gtest)
endif()

