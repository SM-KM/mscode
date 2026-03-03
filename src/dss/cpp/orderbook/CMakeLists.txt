cmake_minimum_required(VERSION 3.14)
project(vector VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_REQUIRED 23)

if(BUILD_TESTS) 
    add_executable(orderbook_test ./tests/orderbook.cc)
    target_link_libraries(orderbook_test gtest gtest_main)
endif()


