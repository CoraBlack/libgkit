cmake_minimum_required(VERSION 3.20)

set (GKIT_OBJS)

add_subdirectory(core)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_core>)

add_subdirectory(math)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_math>)

add_subdirectory(resource)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_resource>)

add_subdirectory(scene)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_scene>)

add_subdirectory(utils)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_utils>)

add_subdirectory(graphic)
list (APPEND GKIT_OBJS $<TARGET_OBJECTS:gkit_graphic>)

set (GKIT_LIBS)
if(GKIT_SHARED_BUILD)
    add_library(${PROJECT_NAME} SHARED ${GKIT_OBJS})
    list (APPEND GKIT_LIBS ${PROJECT_NAME})
endif()

if(GKIT_STATIC_BUILD)
    add_library(${PROJECT_NAME}_static STATIC ${GKIT_OBJS})
    list (APPEND GKIT_LIBS ${PROJECT_NAME}_static)
endif()

# link third party static libraries
foreach(GKIT_LIB ${GKIT_LIBS})
    target_link_libraries(${GKIT_LIB} PRIVATE
        SDL3::SDL3-static
        SDL3_image::SDL3_image-static
        glad_gl
    )
endforeach()
