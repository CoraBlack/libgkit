set (GKIT_SCENE "gkit_scene")

set (SCENE_SRC
    "./node.cpp"

    # 2D scene
    

    # 3D scene
)

add_library(${GKIT_SCENE} OBJECT ${SCENE_SRC})

target_include_directories(${GKIT_SCENE} PRIVATE ${GKIT_SOURCE_DIR}/include)  # include for header file
target_include_directories(${GKIT_SCENE} PRIVATE ${GKIT_SOURCE_DIR}/src)      # top include for source file