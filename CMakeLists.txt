cmake_minimum_required(VERSION 3.10)
project(mscode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB CPP_EXPL "cpp/*.cpp")
set(SOURCES ${CPP_EXPL} main.cpp)

add_executable(main ${SOURCES})
