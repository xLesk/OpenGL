//=
#include "Core/Window.hpp"
#include "Core/Camera.hpp"
#include "Renderer/Shader.hpp"
// #include "Renderer/Texture.hpp"
#include "Renderer/VAO.hpp"
#include "Renderer/VBO.hpp"
// #include "Renderer/EBO.hpp"
// #include "Graphics/Geometry.hpp"
//=

int main() {
  const unsigned int SCR_WIDTH(960), SCR_HEIGHT(540), maxPoints(50000);
  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
  glEnable(GL_DEPTH_TEST);

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
    //           << std::endl;
  }

  // // Startwerte
  // float x = 0.1f, y = 0.0f, z = 0.0f;
  //
  // // Parameter
  // float a = 0.95f, b = 0.7f, c = 0.6f, d = 3.5f, e = 0.25f, f = 0.1f;
  // float dt = 0.01f;
  // for (unsigned int i = 0; i < maxPoints; ++i) {
  //   float dx = (z - b) * x - d * y;
  //   float dy = d * x + (z - b) * y;
  //   float dz = c + a * z - (z * z * z) / 3.0f -
  //              (x * x + y * y) * (1.0f + e * z) + f * z * (x * x * x);
  //
  //   x += dx * dt;
  //   y += dy * dt;
  //   z += dz * dt;
  //
  //   points.push_back({x * 1.5f, y * 1.5f, (z - 0.8f) * 1.5f});
  // }

  // shader
  Shader lorenz("../assets/shaders/threed_mat_shader.vert",
                "../assets/shaders/lorenz_fragshader.frag");

  // VAO, VBO, EBO
  VAO vao;
  VBO vbo(points.data(), points.size() * sizeof(Points));

  vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Points), 0);

  float deltaTime(0.0f);
  float lastFrame(0.0f);

  float pointsToDraw(0.0f);
  float growthSpeed(500.0f);

  //= Render loop
  while (!window.shouldClose()) {
    // deltatime and input
    float currentFrame(static_cast<float>(glfwGetTime()));
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    window.processInput(deltaTime);
    pointsToDraw += growthSpeed * deltaTime;
    pointsToDraw = std::min(pointsToDraw, static_cast<float>(points.size()));

    // render
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vao.bind();
    lorenz.use();
    lorenz.setCamera(window.camera, window.aspectRatio());

    glm::mat4 model = glm::mat4(1.0f);
    lorenz.setMat4("model", model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(pointsToDraw));
    window.swapBuffers();
    window.pollEvents();
  }
  return 0;
}
