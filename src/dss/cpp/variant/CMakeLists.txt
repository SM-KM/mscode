cmake_minimum_required(3.14)
project(vector VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(variant_td ./variant_td.cpp)
set_target_properties(variant_td PROPERTIES CXX_CLANG_TIDY 
	"${CLANG_TIDY_COMMAND}")

if(BUILD_TESTS) 
    add_executable(variant_test ./tests/variant.cc)
    target_link_libraries(variant_test gtest_main gtest)
endif()
