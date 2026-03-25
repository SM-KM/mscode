# mscode

implmentation of a good part of the std library of c++23, along side 
interesting parts of the language, and a good bunch of [metaprogramming](/src/cpp/templates.cpp)

## Compiling
can be done with the included CMakeLists.txt in the root of
the project to compile everything at once, with or without tests, using
the option variable defined on the root cmake file.

```cmake
option(BUILD_TESTS "Build with tests globally" ON)
```

### Compiling specific parts
to compile specific part of the project, use the included CMakeLists.txt 
files included along side the implementation of each part of dss,
and the tests for them can be disabled per part.

minimal example of the cmake file for vector:
```cmake
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
```
