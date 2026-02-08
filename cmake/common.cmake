# C standard
set(CMAKE_C_Standard 11)
set(CMAKE_C_Standard_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIBRARY_OUTPUT_PATH ${BIN_FOLDER})
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Add third party include paths
include_directories(${CMAKE_SOURCE_DIR}/third_party/sdl/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/sdl_image/include)

# Link third party libraries
link_directories(${BIN_FOLDER}/third_party)