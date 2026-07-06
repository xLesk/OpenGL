cmake_minimum_required(VERSION 4.3.1)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(engine
    src/main.cpp
    src/glad.c

    include/Core/Window.hpp
    src/Core/Window.cpp

    include/Core/Shader.hpp
    src/Core/Shader.cpp

    include/Core/camera.hpp
    src/Core/camera.cpp

    include/Core/VAO_VBO_EBO.hpp
    src/Core/VAO_VBO_EBO.cpp

    include/Graphics/Geometry.hpp

    include/Core/stb_image.hpp
    src/Core/stb_image.cpp
)

target_include_directories(engine PRIVATE include)

target_link_libraries(engine
    OpenGL::GL
    glfw
)

