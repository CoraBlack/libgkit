set (LIBRARY_OUTPUT_PATH ${BIN_FOLDER}/third_party)

# build sdl (build for static lib)
set (SDL_STATIC ON)
set (SDL_SHARED OFF)
set (SDL_BUILD_SHARED_LIBS OFF)
set (SDL_STATIC_PIC ON)
set (SDL_BUILD_TESTS OFF)
set (SDL_INSTALL OFF)
set (SDL_TEST_LIBRARY OFF)
set (SDL_TESTS OFF)
add_subdirectory(sdl)

# build sdl_image
set (BUILD_SHARED_LIBS OFF)
set (SDLIMAGE_INSTALL OFF)
set (SDLIMAGE_BUILD_SHARED_LIBS OFF)
set (SDLIMAGE_TESTS OFF)
add_subdirectory(sdl_image EXCLUDE_FROM_ALL)