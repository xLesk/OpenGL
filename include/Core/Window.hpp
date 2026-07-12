#pragma once
#include "Core/Camera.hpp"
#include "GLFW/glfw3.h"

class Window {
public:
  // constructor, destructor
  Window(int SCR_WIDTH, int SCR_HEIGHT, const char *title);
  ~Window();

  // camera
  Camera camera;

  // window status
  bool shouldClose() const;
  void swapBuffers();
  void pollEvents();

  // window getter
  GLFWwindow *getWindow() const;
  float aspectRatio() const;

  // input
  void processInput(float dt);

private:
  GLFWwindow *window;

  // screen
  unsigned int SCR_WIDTH;
  unsigned int SCR_HEIGHT;

  // mouse
  float lastX;
  float lastY;
  bool firstMouse;

  static void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);

  static void framebufferCallback(GLFWwindow *window, int width, int height);
  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);
};
