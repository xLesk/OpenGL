//=
#include "Core/Window.hpp"
#include "Core/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VAO.hpp"
#include "Renderer/VBO.hpp"
#include "Renderer/EBO.hpp"
#include "Graphics/Geometry.hpp"
#include "Graphics/Colors.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/matrix.hpp>
//=

#define TEX 0

int main() {
  // window
  const unsigned int SCR_WIDTH(960), SCR_HEIGHT(540);
  Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
  glEnable(GL_DEPTH_TEST);

  // shader
  Shader objectCubeShader("../assets/shaders/lighting/object.vert",
                          "../assets/shaders/lighting/object.frag");

  // gouraud
  // Shader objectCubeShader("../assets/shaders/lighting/gouraud.vert",
  //                                 "../assets/shaders/lighting/gouraud.frag");
  Shader lightCubeShader("../assets/shaders/lighting/lighting.vert",
                         "../assets/shaders/lighting/lighting.frag");
  // // texture
  // Texture container("../assets/textures/container.jpg");
  // Texture awesomeface("../assets/textures/awesomeface.png");
  Texture container2("../assets/textures/container2.png");

  // VAO, VBO, EBO
  VAO cubeVAO;
  VAO lightVAO;

#if TEX
  VBO cubeVBO(Geometry::cubeVertices.data(),
              Geometry::cubeVertices.size() * sizeof(float));
  cubeVAO.linkAttrib(cubeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  cubeVAO.linkAttrib(cubeVBO, 1, 3, GL_FLOAT, 6 * sizeof(float),
                     3 * sizeof(float));
  lightVAO.linkAttrib(cubeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  EBO ebo(Geometry::indices.data(), sizeof(Geometry::indices));
#else
  VBO cubeTexVBO(Geometry::cubeVerticesTex.data(),
                 Geometry::cubeVerticesTex.size() * sizeof(float));
  cubeVAO.linkAttrib(cubeTexVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  cubeVAO.linkAttrib(cubeTexVBO, 1, 3, GL_FLOAT, 8 * sizeof(float),
                     3 * sizeof(float));
  cubeVAO.linkAttrib(cubeTexVBO, 2, 2, GL_FLOAT, 8 * sizeof(float),
                     6 * sizeof(float));
  lightVAO.linkAttrib(cubeTexVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  EBO eboTex(Geometry::indices.data(), sizeof(Geometry::indices));
#endif // TEX

  // lighting constants
  //// object
  const glm::vec3 object_diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
  const glm::vec3 object_specular = glm::vec3(0.5f, 0.5f, 0.5f);
  const float shininess(64.0f);

  //// light
  const glm::vec3 lightColor = Colors::white;
  const glm::vec3 light_specular = glm::vec3(1.0f);
  const glm::vec3 light_diffuse = glm::vec3(0.5f);
  const glm::vec3 light_ambient = light_diffuse * glm::vec3(0.4f);

  // delta time
  float dt(0.0f);
  float lastFrame(0.0f);

  // Model Matrix
  glm::mat4 objectCubeModel = glm::mat4(1.0f);
  // normal matrix
  glm::mat3 normalModel =
      glm::transpose(glm::inverse(glm::mat3(objectCubeModel)));

  // Material Tex
  objectCubeShader.setInt("material.diffuse", 0);

  objectCubeShader.use();

  //// material
  objectCubeShader.setVec3("material.diffuse", object_diffuse);
  objectCubeShader.setVec3("material.specular", object_specular);
  objectCubeShader.setFloat("material.shininess", shininess);

  //// light
  objectCubeShader.setVec3("light.ambient", light_ambient);
  objectCubeShader.setVec3("light.diffuse", light_diffuse);
  objectCubeShader.setVec3("light.specular", light_specular);

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

    // Position of light cube
    glm::vec3 lightPos =
        glm::vec3(static_cast<float>(3 * sin(0.5f * currentFrame)),
                  static_cast<float>(sin(currentFrame)),
                  static_cast<float>(3 * cos(0.5f * currentFrame)));

    // object to be lightend?
    cubeVAO.bind();
    objectCubeShader.use();
    objectCubeShader.setCamera(window.camera, window.aspectRatio());

    objectCubeShader.setMat4("model", objectCubeModel);
    objectCubeShader.setMat3("normalModel", normalModel);

    // phong
    objectCubeShader.setVec3("light.position", lightPos);

    objectCubeShader.setVec3("objectColor", Colors::white);
    objectCubeShader.setVec3("viewPos", window.camera.Position);

    container2.bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // light source
    lightVAO.bind();
    lightCubeShader.use();
    lightCubeShader.setCamera(window.camera, window.aspectRatio());

    glm::mat4 lightCubeModel = glm::mat4(1.0f);
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
