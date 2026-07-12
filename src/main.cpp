//=
#include "Core/Window.hpp"
#include "Core/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VAO.hpp"
#include "Renderer/VBO.hpp"
#include "Renderer/EBO.hpp"
#include "Graphics/Geometry.hpp"
//=

int main() {
  // window
  const unsigned int SCR_WIDTH(960), SCR_HEIGHT(540);
  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
  glEnable(GL_DEPTH_TEST);

  // VAO, VBO, EBO
  VAO vao;
  VBO vbo(Geometry::cubeVertices.data(),
          Geometry::cubeVertices.size() * sizeof(float));

  vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);

  vao.linkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));

  EBO ebo(Geometry::indices.data(), sizeof(Geometry::indices));

  // shader
  Shader shader("../assets/shaders/threed_mat_shader.vert",
                "../assets/shaders/sampler.frag");

  // texture
  Texture container("../assets/textures/container.jpg");
  shader.setInt("texture1", 0);

  Texture awesomeface("../assets/textures/awesomeface.png");
  shader.setInt("texture2", 1);

  // transformatoin Matrix
  glm::mat4 trans(glm::mat4(1.0f));
  unsigned int transformLoc = glGetUniformLocation(shader.getID(), "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

  // Model Matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  int modelLoc = glGetUniformLocation(shader.getID(), "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // frustum
  glm::mat4 persp_proj = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.00f);
  int projLoc = glGetUniformLocation(shader.getID(), "projection");
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
    container.bind(0);
    awesomeface.bind(1);

    glUniform1i(glGetUniformLocation(shader.getID(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.getID(), "texture2"), 1);

    shader.use();

    glm::mat4 projection =
        glm::perspective(glm::radians(window.camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    shader.setMat4("projection", projection);

    glm::mat4 view = window.camera.GetViewMatrix();
    shader.setMat4("view", view);

    vao.bind();

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
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    vao.bind();

    window.swapBuffers();
    window.pollEvents();
  }

  // Freeing the memory
  glfwTerminate();

  return 0;
}
