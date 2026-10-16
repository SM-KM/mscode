function(add_rust_test TEST_NAME TEST_SOURCE)
    set(TEST_BINARY "${RUST_BUILD_DIR}/${TEST_NAME}")
    add_custom_command(
        OUTPUT "${TEST_BINARY}"
        COMMAND "${RUSTC_EXECUTABLE}"
            --edition=${RUST_EDITION}
            --test
            "${TEST_SOURCE}"
            --extern "stdr=${RUST_LIBRARY}"
            -L "dependency=${RUST_BUILD_DIR}"
            -o "${TEST_BINARY}"
        DEPENDS
            "${TEST_SOURCE}"
            ${RUST_LIBRARY_SOURCES}
            "${RUST_LIBRARY}"
        VERBATIM
    )

    add_custom_target(${TEST_NAME}_build ALL
        DEPENDS "${TEST_BINARY}"
    )
    add_test(
        NAME ${TEST_NAME}
        COMMAND "${TEST_BINARY}"
    )
endfunction()


function(add_rust_executable TARGET_NAME SOURCE_FILE)
    set(TARGET_BINARY "${RUST_BUILD_DIR}/${TARGET_NAME}")
    add_custom_command(
        OUTPUT "${TARGET_BINARY}"
        COMMAND "${RUSTC_EXECUTABLE}"
            --edition=${RUST_EDITION}
            "${SOURCE_FILE}"
            --extern "stdr=${RUST_LIBRARY}"
            -L "dependency=${RUST_BUILD_DIR}"
            -o "${TARGET_BINARY}"
        DEPENDS
            "${SOURCE_FILE}"
            ${RUST_LIBRARY_SOURCES}
            "${RUST_LIBRARY}"
        VERBATIM
    )
    add_custom_target(${TARGET_NAME}
        DEPENDS "${TARGET_BINARY}"
    )
endfunction()
