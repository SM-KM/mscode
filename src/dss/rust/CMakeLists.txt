cmake_minimum_required(VERSION 3.20)
project(stdr LANGUAGES NONE)

find_program(RUSTC_EXECUTABLE rustc REQUIRED)
enable_testing()

set(RUST_EDITION 2024)
set(RUST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUST_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(RUST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/rust")

file(MAKE_DIRECTORY "${RUST_BUILD_DIR}")

file(GLOB_RECURSE RUST_LIBRARY_SOURCES
    CONFIGURE_DEPENDS
    "${RUST_SOURCE_DIR}/*.rs"
)

set(RUST_LIBRARY
    "${RUST_BUILD_DIR}/libstdr.rlib"
)

add_custom_command(
    OUTPUT "${RUST_LIBRARY}"
    COMMAND "${RUSTC_EXECUTABLE}"
        --edition=${RUST_EDITION}
        --crate-name stdr
        --crate-type rlib
        "${RUST_SOURCE_DIR}/lib.rs"
        -o "${RUST_LIBRARY}"
    DEPENDS ${RUST_LIBRARY_SOURCES}
    VERBATIM
)

add_custom_target(stdr ALL DEPENDS "${RUST_LIBRARY}")
include(cmake/RunTests.cmake)
add_rust_test(vector_test "${RUST_TEST_DIR}/vector/vector_test.rs")
add_rust_executable(vector "${CMAKE_CURRENT_SOURCE_DIR}/src/collections/vector.rs")
