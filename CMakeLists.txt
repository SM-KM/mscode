cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_REQUIRED 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# target the go and rust compilers to handle them as well, they need special
# configuration
project(mscode VERSION 0.0.1 LANGUAGES C CXX)
find_program(CLANG_TIDY_EXE "clang-tidy")

if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_EXE})
else()
    message(WARNING "clang-tidy not found")
endif()

# set the option to run the tests on any internal 
# cmake file on the dss
option(BUILD_TESTS "Build with tests globally" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
endif()

option(BUILD_CPP "Build the cpp portion" OFF)
if(BUILD_CPP)
    # handle just building the parts of the dss that i want
    # and just the dss that i want, with tests or not
    add_subdirectory(./src/dss/cpp/)
endif()
