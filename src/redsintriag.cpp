#include "Core/include.hpp"
#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include <cmath>

int main() {
  assert(glfwInit());
  glfwhints();

  // Creating the window and checking if it works
  GLFWwindow *window = glfwCreateWindow(1100, 900, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  create_window(window);

  // Normalized device coordinates
  float vertices[] = {
      -1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 1.0f, 0.0f, // top first triangle
      0.0f,  0.0f, 0.0f, // center
      1.0f,  0.0f, 0.0f, // top second triangle
      0.5f,  1.0f, 0.0f, // bottom right
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 2, // first triangle
      2, 3, 4, // second triangle
  };

  // VAO
  unsigned int VAO;
  gen_id_and_bind_VAO(VAO);

  // vertex buffer object
  unsigned int VBO;
  gen_id_and_bind_VBO(VBO, vertices, sizeof(vertices));

  // element buffer object
  unsigned int EBO;
  gen_id_and_bind_EBO(EBO, indices, sizeof(indices));

  // vertex shader
  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "out vec4 vertexColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(aPos, 1.0);\n"
                                   "vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
                                   "}\0";
  unsigned int vertexShader = compile_vert_shader(vertexShaderSource);
  check_shader(vertexShader);

  // fragment shader
  const char *fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "uniform vec4 ourColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     " FragColor = ourColor;\n"
                                     "}\0";
  unsigned int fragmentShader = compile_frag_shader(fragmentShaderSource);
  check_shader(fragmentShader);

  const char *yellowShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      " FragColor = vec4(1.0f, 1.0f,0.0f , 1.0f);\n"
      "}\0";
  unsigned int yellowShader = compile_frag_shader(yellowShaderSource);
  check_shader(yellowShader);

  const char *wireframeShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      " FragColor = vec4(1.0f, 1.0f,1.0f , 1.0f);\n"
      "}\0";
  unsigned int wireframeShader = compile_frag_shader(wireframeShaderSource);
  check_shader(wireframeShader);

  // shader program and linking
  unsigned int shaderProgram = create_program(vertexShader, fragmentShader);

  unsigned int wireframeProgram = create_program(vertexShader, wireframeShader);

  unsigned int yellowShaderProgram = create_program(vertexShader, yellowShader);
  check_program(shaderProgram);
  check_program(wireframeProgram);
  check_program(yellowShaderProgram);

  // Linking vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  //= Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    processInput(window);

    glClearColor(0.157f, 0.157f, 0.157f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    float timeValue = glfwGetTime();
    float redValue = (sin(timeValue) / 2.0f) + 0.5f;

    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

    // rendering commands:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(yellowShaderProgram);
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(wireframeProgram);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Freeing the memory
  glfwTerminate();
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(wireframeShader);
  glDeleteShader(yellowShader);
  glDeleteProgram(yellowShaderProgram);
  glDeleteProgram(shaderProgram);
  glDeleteProgram(wireframeProgram);
  return 0;
}
