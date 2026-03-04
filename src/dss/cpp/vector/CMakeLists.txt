cmake_minimum_required(VERSION 3.14)
project(vector VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing linting using clang-tidy
add_executable(vector_td ./vector_td.cpp)
set_target_properties(vector_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

if(BUILD_TESTS) 
    add_executable(vector_test ./tests/vector.cc)
    target_link_libraries(vector_test gtest_main gtest)
endif()


