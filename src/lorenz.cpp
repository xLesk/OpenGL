#include "Core/include.hpp"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

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

  struct Points {
    float x, y, z;
  };

  std::vector<Points> points;

  float x(0.1f);
  float y(0.0f);
  float z(0.0f);

  float sigma(10.0f);
  float rho(28.0f);
  float beta(8.0f / 3.0f);

  float dt = 0.01f;

  for (unsigned int i(0); i < 50000; ++i) {
    float dx = sigma * (y - x);
    float dy = x * (rho - z) - y;
    float dz = x * y - beta * z;
    x += dx * dt;
    y += dy * dt;
    z += dz * dt;

    points.push_back({x * 0.02f, y * 0.02f, z * 0.02f});
  }

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Points), points.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Points), (void *)0);
  glEnableVertexAttribArray(0);
  Shader lorenz("../assets/shaders/lorenz_vertshader.vert",
                "../assets/shaders/lorenz_fragshader.frag");
  // std::vector<float> vertices = {
  //     // positions          // colors           // texture coords
  //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
  //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
  //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
  //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  // };

  // Textures
  //-----------
  // Loading the texture
  // unsigned int texture1, texture2{};
  // glGenTextures(1, &texture1);
  // glBindTexture(GL_TEXTURE_2D, texture1);
  //
  // // set the texture wrapping/filtering options (on currently bound texture)
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // // load and generate the texture
  // int width, height, nrChannels;
  // stbi_set_flip_vertically_on_load(true);
  // unsigned char *data_container =
  // stbi_load("../assets/textures/container.jpg",
  //                                           &width, &height, &nrChannels, 0);
  // if (data_container) {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
  //                GL_UNSIGNED_BYTE, data_container);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  // } else {
  //   std::cout << "Failed to load texture" << std::endl;
  // }
  //
  // stbi_image_free(data_container);
  //
  // glGenTextures(1, &texture2);
  // glBindTexture(GL_TEXTURE_2D, texture2);
  //
  // unsigned char *data_awesome_face = stbi_load(
  //     "../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  // if (data_awesome_face) {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
  //                GL_UNSIGNED_BYTE, data_awesome_face);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  // } else {
  //   std::cout << "Failed to load texture" << std::endl;
  // }
  //
  // stbi_image_free(data_awesome_face);
  //
  // std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};
  //
  // unsigned int VBO, VAO, EBO;
  // gen_id_and_bind_VAO(VAO);
  // gen_id_and_bind_VBO(VBO, vertices.data(), vertices.size() * sizeof(float));
  // gen_id_and_bind_EBO(EBO, indices.data(),
  //                     indices.size() * sizeof(unsigned int));
  //
  // // position attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0);
  // // color attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  // // texture coord attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  //
  // Shader texture_mat_shader("../assets/shaders/mat.vert",
  // "../assets/shaders/sampler.frag");

  // float millis = glfwGetTime();
  //
  // texture_mat_shader.use();
  // texture_mat_shader.setInt("texture2", 1);
  //
  // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  // glm::mat4 trans(glm::mat4(1.0f));
  // trans = glm::rotate(trans, glm::radians(90.0f),
  // glm::vec3(0.0, 0.0, 1.0f)); trans =
  // glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
  //
  // unsigned int transformLoc =
  //     glGetUniformLocation(texture_mat_shader.getID(),
  //     "transform");
  // std::cout << transformLoc << std::endl;
  // glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
  // glm::value_ptr(trans));
  //= Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    processInput(window);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // // Binding texture
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    //
    // glUniform1i(glGetUniformLocation(texture_mat_shader.getID(),
    // "texture1"),
    //             0);
    //
    // texture_mat_shader.use();
    // glm::mat4 trans1 = glm::mat4(1.0f);
    // trans1 = glm::translate(trans1, glm::vec3(0.5f,
    // -0.5f, 0.0f)); trans1 =
    //     glm::rotate(trans1, (float)glfwGetTime(),
    //     glm::vec3(0.0f, 0.0f, 1.0f));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
    // glm::value_ptr(trans1));
    //
    // glm::mat4 trans2 = glm::mat4(1.0f);
    // trans2 = glm::translate(trans2,
    // glm::vec3(-0.5f, 0.5f, 0.0)); trans2 =
    //     glm::scale(trans2, glm::vec3(sin(millis),
    //     sin(millis), sin(millis)));
    // glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, 6,
    // GL_UNSIGNED_INT, 0);
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
    // glm::value_ptr(trans2));
    // glDrawElements(GL_TRIANGLES, 6,
    // GL_UNSIGNED_INT, 0);
    lorenz.use();

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Freeing the memory
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}
