cmake_minimum_required(VERSION 3.17)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext/include)

add_executable(learn_opengl
    main.cpp
    shader.cpp
    ext/src/glad.c
)

# Get full path to ext/lib/libglfw3.a
find_library(GLFW3_LIB glfw3 ext/lib)

# Link libraries from OS and GLFW
target_link_libraries(learn_opengl LINK_PRIVATE -lGL -lX11 -lpthread -lXrandr -lXi -ldl ${GLFW3_LIB})

target_compile_definitions(learn_opengl PRIVATE STB_IMAGE_IMPLEMENTATION=1)

if (WIREFRAME)
    target_compile_definitions(learn_opengl PRIVATE WIREFRAME=1)
endif (WIREFRAME)
