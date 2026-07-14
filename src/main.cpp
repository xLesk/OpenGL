//=
#include "Core/Window.hpp"
#include "Core/Camera.hpp"
#include "Renderer/Shader.hpp"
// #include "Renderer/Texture.hpp"
#include "Renderer/VAO.hpp"
#include "Renderer/VBO.hpp"
#include "Renderer/EBO.hpp"
#include "Graphics/Geometry.hpp"
#include "Graphics/Colors.hpp"
//=

int main() {
  // window
  const unsigned int SCR_WIDTH(960), SCR_HEIGHT(540);
  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
  glEnable(GL_DEPTH_TEST);

  // shader
  Shader objectCubeShader("../assets/shaders/lighting/object.vert",
                          "../assets/shaders/lighting/object.frag");
  Shader lightCubeShader("../assets/shaders/lighting/lighting.vert",
                         "../assets/shaders/lighting/lighting.frag");
  // // texture
  // Texture container("../assets/textures/container.jpg");
  // Texture awesomeface("../assets/textures/awesomeface.png");

  // VAO, VBO, EBO
  VAO cubeVAO;
  VAO lightVAO;
  VBO cubeVBO(Geometry::cubeVertices.data(),
              Geometry::cubeVertices.size() * sizeof(float));
  EBO ebo(Geometry::indices.data(), sizeof(Geometry::indices));

  cubeVAO.linkAttrib(cubeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  cubeVAO.linkAttrib(cubeVBO, 1, 3, GL_FLOAT, 6 * sizeof(float),
                     3 * sizeof(float));
  lightVAO.linkAttrib(cubeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);

  objectCubeShader.use();
  lightCubeShader.use();

  // lighting constants
  const glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
  const float ambientStrength(0.1f);
  const float specularStrength(0.5f);

  // delta time
  float dt(0.0f);
  float lastFrame(0.0f);

  //= Render loop
  while (!window.shouldClose()) {
    // deltatime and input
    float currentFrame(static_cast<float>(glfwGetTime()));
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    window.processInput(dt);

    // render
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // object to be lightend?
    cubeVAO.bind();
    objectCubeShader.use();
    objectCubeShader.setCamera(window.camera, window.aspectRatio());

    glm::mat4 objectCubeModel = glm::mat4(1.0f);
    glm::mat3 normalModel = glm::transpose(glm::inverse(objectCubeModel));
    objectCubeShader.setFloat("ambientStrength", ambientStrength);
    objectCubeShader.setFloat("specularStrength", specularStrength);
    objectCubeShader.setMat4("model", normalModel);
    objectCubeShader.setVec3("lightPos", lightPos);
    objectCubeShader.setVec3("objectColor", Colors::coral);
    objectCubeShader.setVec3("lightColor", Colors::lightColor);
    objectCubeShader.setVec3("viewPos", window.camera.Position);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // light source
    lightVAO.bind();
    lightCubeShader.use();
    lightCubeShader.setCamera(window.camera, window.aspectRatio());

    glm::mat4 lightCubeModel = glm::mat4(1.0f);
    lightCubeModel = glm::mat4(1.0f);
    lightCubeModel = glm::translate(lightCubeModel, lightPos);
    lightCubeModel = glm::scale(lightCubeModel, glm::vec3(0.2f));
    lightCubeShader.setMat4("model", lightCubeModel);
    lightCubeShader.setVec3("lightColor", Colors::lightColor);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Drawing

    window.swapBuffers();
    window.pollEvents();
  }
  return 0;
}
