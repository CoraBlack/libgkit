set (EXECUTABLE_OUTPUT_PATH ${BIN_FOLDER}/test)

include_directories(${GKIT_SOURCE_DIR}/src)
include_directories(${GKIT_SOURCE_DIR}/include)
include_directories(${GKIT_SOURCE_DIR}/src/c_abi/include)
include_directories(${GKIT_SOURCE_DIR}/third_party/glad_generated/include)

# build core tests
file(GLOB_RECURSE TEST_SRCS
    "core/*.cpp"
    "math/*.cpp"
    "utils/*.cpp"
    "resource/*.cpp"
    "graphic/*.cpp"
)

foreach(test_file ${TEST_SRCS})
    get_filename_component(FILE_BASE_NAME ${test_file} NAME_WE)
    add_executable(${FILE_BASE_NAME} ${test_file})
    target_link_libraries(${FILE_BASE_NAME} PRIVATE gkit_static)
endforeach()