cmake_minimum_required(VERSION 3.17)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext/include)

add_executable(learn_opengl
    main.cpp
    shader.cpp
    camera.cpp
    mesh.cpp
    model.cpp
    ext/src/glad.c
)

# Get full path to ext/lib/libglfw3.a
find_library(GLFW3_LIB glfw3 ext/lib)

# Get full path to ext/lib/libassimp.a
find_library(ASSIMP_LIB assimp ext/lib)

# Link libraries
target_link_libraries(learn_opengl LINK_PUBLIC -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lz)
target_link_libraries(learn_opengl LINK_PUBLIC ${GLFW3_LIB})
target_link_libraries(learn_opengl LINK_PUBLIC ${ASSIMP_LIB})

if (WIREFRAME)
    target_compile_definitions(learn_opengl PUBLIC WIREFRAME=1)
endif (WIREFRAME)
