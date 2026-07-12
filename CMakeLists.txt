cmake_minimum_required(VERSION 4.3.1)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(engine
  # main
    src/main.cpp

    # Renderer
    src/Renderer/Shader.cpp
    
    src/Renderer/Texture.cpp

    src/Renderer/VAO.cpp

    src/Renderer/VBO.cpp

    src/Renderer/EBO.cpp

    # Core
    src/Core/Camera.cpp

    src/Core/Window.cpp    


    # Graphics
    include/Graphics/Geometry.hpp

    # third party
    third_party/stb/Core/stb_image.cpp
    
    third_party/glad/src/glad.c
)

target_include_directories(engine PRIVATE
    include
    third_party/glad/include
    third_party/stb/include
  )

target_link_libraries(engine
    OpenGL::GL
    glfw
)

