if(POLICY CMP0076)
    cmake_policy(SET CMP0076 OLD)
endif()

target_sources(example_cmake PRIVATE src/utils/logger.c)
