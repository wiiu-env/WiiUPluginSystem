cmake_minimum_required(VERSION 3.2)

# create a WUPS plugin
function(wups_create_plugin target)
    set(WUPS_ROOT "${DEVKITPRO}/wups")

    target_include_directories(${target} PRIVATE "${WUPS_ROOT}/include")
    target_link_directories(${target} PRIVATE "${WUPS_ROOT}/lib")
    target_link_libraries(${target} wups)
    target_link_options(${target} PRIVATE "-T${WUPS_ROOT}/share/wups.ld" "-specs=${WUPS_ROOT}/share/wups.specs")

    get_target_property(WUPS_OUTPUT_NAME ${target} OUTPUT_NAME)
    get_target_property(WUPS_BINARY_DIR  ${target} BINARY_DIR)
    if(NOT WUPS_OUTPUT_NAME)
        set(WUPS_OUTPUT_NAME "${target}")
    endif()
    set(WUPS_OUTPUT "${WUPS_BINARY_DIR}/${WUPS_OUTPUT_NAME}.wps")

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${WUT_ELF2RPL_EXE} "$<TARGET_FILE:${target}>" "${WUPS_OUTPUT}"
        COMMAND echo PL | dd of=${WUPS_OUTPUT} bs=1 seek=9 count=2 conv=notrunc status=none
        BYPRODUCTS "${WUPS_OUTPUT}"
        COMMENT "Converting ${target} to .wps format"
        VERBATIM
    )
endfunction()
