//=
#include <glm/glm.hpp>
#include <iostream>
#include "Core/Window.hpp"
#include "Core/VAO_VBO_EBO.hpp"
#include "Core/Shader.hpp"
#include "Core/camera.hpp"
#include "Graphics/Geometry.hpp"
#include "Core/stb_image.hpp"
//=

int main() {
  const unsigned int SCR_WIDTH(960);
  const unsigned int SCR_HEIGHT(540);

  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

  glEnable(GL_DEPTH_TEST);

  //  Textures
  // -----------
  //  Loading the texture
  unsigned int texture1, texture2{};
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // load and generate the texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data_container = stbi_load("../assets/textures/container.jpg",
                                            &width, &height, &nrChannels, 0);
  if (data_container) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data_container);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data_container);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  unsigned char *data_awesome_face = stbi_load(
      "../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data_awesome_face) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data_awesome_face);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data_awesome_face);

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int VBO, VAO;
  gen_id_and_bind_VAO(VAO);
  gen_id_and_bind_VBO(VBO, Geometry::cubeVertices.data(),
                      Geometry::cubeVertices.size() * sizeof(float));

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // texture coords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader texture_mat_shader("../assets/shaders/threed_mat_shader.vert",
                            "../assets/shaders/sampler.frag");

  texture_mat_shader.use();
  texture_mat_shader.setInt("texture2", 1);

  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans(glm::mat4(1.0f));
  trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
  trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

  unsigned int transformLoc =
      glGetUniformLocation(texture_mat_shader.getID(), "transform");
  // std::cout << transformLoc << std::endl;
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

  // Model Matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  int modelLoc = glGetUniformLocation(texture_mat_shader.getID(), "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // frustum
  glm::mat4 persp_proj = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.00f);
  int projLoc = glGetUniformLocation(texture_mat_shader.getID(), "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(persp_proj));

  // delta time
  float deltaTime(0.0f);
  float lastFrame(0.0f);

  //= Render loop
  while (!window.shouldClose()) {
    float currentFrame(static_cast<float>(glfwGetTime()));
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float *deltaPtr(&deltaTime);

    window.processInput(deltaPtr);
    //
    // render
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(texture_mat_shader.getID(), "texture1"),
                0);

    texture_mat_shader.use();

    glm::mat4 projection =
        glm::perspective(glm::radians(window.camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    texture_mat_shader.setMat4("projection", projection);

    glm::mat4 view = window.camera.GetViewMatrix();
    texture_mat_shader.setMat4("view", view);

    glBindVertexArray(VAO);

    for (std::size_t i = 0; i < Geometry::cubePos.size(); ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, Geometry::cubePos[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      if (i % 3 == 0) {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));
      }
      texture_mat_shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(VAO);

    window.swapBuffers();
    window.pollEvents();
  }

  // Freeing the memory
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();

  return 0;
}
