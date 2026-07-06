#include "Core/include.hpp"

int main() {
  assert(glfwInit());
  glfwhints();

  // Creating the window and checking if it works
  GLFWwindow *window = glfwCreateWindow(900, 900, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  create_window(window);

  // Alles zum Kreis
  const int segments = 100;
  float radius = 0.5f;

  std::vector<float> vertices;

  // Mittelpunkt
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);

  // Kreisrand
  for (int i = 0; i <= segments; i++) {
    float angle = 2.0f * M_PI * i / segments;

    float x = radius * cos(angle);
    float y = radius * sin(angle);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f);
  }

  float const speed(0.05f);
  float xOffset(0.0);
  float yOffset(0.0);
  float *xPtr(&xOffset);
  float *yPtr(&yOffset);
  float const *speedPtr(&speed);

  // Textures

  std::vector<float> texCoords;
  // Lower Left Corner
  texCoords.push_back(0.0f);
  texCoords.push_back(0.0f);
  // Lower Right Corner
  texCoords.push_back(1.0f);
  texCoords.push_back(0.0f);
  // top-Center Corner
  texCoords.push_back(0.5f);
  texCoords.push_back(1.0f);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);
  // VAO
  unsigned int VAO;
  gen_id_and_bind_VAO(VAO);

  // vertex buffer object
  unsigned int VBO;
  gen_id_and_bind_VBO(VBO, vertices.data(), sizeof(float) * vertices.size());

  // element buffer object

  Shader rainbow_triangle("../assets/shaders/vertexShader.vert",
                          "../assets/shaders/fragmentShader.frag");
  Shader wireframe_triangle("../assets/shaders/vertexShader.vert",
                            "../assets/shaders/wireframeShader.frag");

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //= Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    processMovement(window, xPtr, yPtr, speedPtr);
    float timeValue = glfwGetTime();
    float dt = sin(timeValue);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    rainbow_triangle.use();
    rainbow_triangle.setFloat("millis", dt);
    rainbow_triangle.setFloat("xOffset", xOffset);
    rainbow_triangle.setFloat("yOffset", yOffset);

    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // wireframe_triangle.use();
    // wireframe_triangle.setFloat("millis", dt);
    // glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Freeing the memory
  glfwTerminate();
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}
