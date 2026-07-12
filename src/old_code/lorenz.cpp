#include <glm/glm.hpp>
#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include "Core/camera.hpp"
#include "Core/stb_image.hpp"
#include <iostream>
#include <vector>

int main() {
  const unsigned int SCR_WIDTH(960);
  const unsigned int SCR_HEIGHT(540);
  const unsigned int maxPoints(50000);

  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

  glEnable(GL_DEPTH_TEST);

  struct Points {
    float x, y, z;
  };

  std::vector<Points> points;

  // float x(0.1f);
  // float y(0.0f);
  // float z(0.0f);
  //
  // float sigma(10.0f);
  // float rho(28.0f);
  // float beta(8.0f / 3.0f);
  //
  // float dt = 0.01f;
  //
  // for (unsigned int i(0); i < 50000; ++i) {
  //   float dx = sigma * (y - x);
  //   float dy = x * (rho - z) - y;
  //   float dz = x * y - beta * z;
  //   x += dx * dt;
  //   y += dy * dt;
  //   z += dz * dt;
  //
  //   points.push_back({x * 0.02f, y * 0.02f, z * 0.02f});
  //   //           << std::endl;
  // }

  // Startwerte
  float x = 0.1f, y = 0.0f, z = 0.0f;

  // Parameter
  float a = 0.95f, b = 0.7f, c = 0.6f, d = 3.5f, e = 0.25f, f = 0.1f;
  float dt = 0.01f;
  for (unsigned int i = 0; i < maxPoints; ++i) {
    float dx = (z - b) * x - d * y;
    float dy = d * x + (z - b) * y;
    float dz = c + a * z - (z * z * z) / 3.0f -
               (x * x + y * y) * (1.0f + e * z) + f * z * (x * x * x);

    x += dx * dt;
    y += dy * dt;
    z += dz * dt;

    points.push_back({x * 1.5f, y * 1.5f, (z - 0.8f) * 1.5f});
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
  Shader lorenz("../assets/shaders/threed_mat_shader.vert",
                "../assets/shaders/lorenz_fragshader.frag");
  lorenz.use();

  glm::mat4 trans(glm::mat4(1.0f));

  unsigned int transformLoc = glGetUniformLocation(lorenz.getID(), "transform");
  // std::cout << transformLoc << std::endl;
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

  // frustum
  glm::mat4 persp_proj = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.00f);
  int projLoc = glGetUniformLocation(lorenz.getID(), "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(persp_proj));

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  lorenz.setMat4("model", model);

  float deltaTime(0.0f);
  float lastFrame(0.0f);

  float pointsToDraw(0.0f);
  float growthSpeed(500.0f);

  //= Render loop
  while (!window.shouldClose()) {
    // Input handling
    float currentFrame(static_cast<float>(glfwGetTime()));
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float *deltaPtr(&deltaTime);

    pointsToDraw += growthSpeed * deltaTime;
    if (pointsToDraw > points.size()) {
      pointsToDraw = 0.0f;
    }

    window.processInput(deltaPtr);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lorenz.use();

    glm::mat4 projection =
        glm::perspective(glm::radians(window.camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    lorenz.setMat4("projection", projection);

    glm::mat4 view = window.camera.GetViewMatrix();
    lorenz.setMat4("view", view);
    glBindVertexArray(VAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(pointsToDraw));
    window.swapBuffers();
    window.pollEvents();
  }

  // Freeing the memory
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}
