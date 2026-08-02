set (GKIT_CORE "gkit_core")

set (CORE_SRC
    # Input module
    "./input/action.cpp"
    "./input/cache.cpp"
    "./input/input.cpp"

    # Misc module
    "./misc/sdl_event_dispatcher.cpp"

    # Reflect module
    "./reflect/registry.cpp"
    "./reflect/seralize.cpp"

    "./log.cpp"
    "./object_id.cpp"
    "./object_pool.cpp"
    "./object.cpp"
    "./processer.cpp"
    "./unique_object.cpp"
    "./value.cpp"
)

add_library(${GKIT_CORE} OBJECT ${CORE_SRC})

target_include_directories(${GKIT_CORE} PRIVATE ${GKIT_SOURCE_DIR}/include)  # include for header file
target_include_directories(${GKIT_CORE} PRIVATE ${GKIT_SOURCE_DIR}/src)      # top include for source file