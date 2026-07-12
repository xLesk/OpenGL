#include "Core/Window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

// constructor, destructor
Window::Window(int SCR_WIDTH, int SCR_HEIGHT, const char *title)
    : SCR_HEIGHT(SCR_HEIGHT), SCR_WIDTH(SCR_WIDTH), lastX(SCR_WIDTH / 2.0f),
      lastY(SCR_HEIGHT / 2.0f), firstMouse(true),
      camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
  assert(glfwInit());

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);

  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    std::exit(EXIT_FAILURE);
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glEnable(GL_DEPTH_TEST);

  // "this" an GLFW hängen
  glfwSetWindowUserPointer(window, this);

  // Callbacks registrieren
  glfwSetFramebufferSizeCallback(window, framebufferCallback);

  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  // Maus einfangen
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

// public:
void Window::processInput(float *deltaPtr) {
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(FORWARD, *deltaPtr);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, *deltaPtr);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, *deltaPtr);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, *deltaPtr);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

void Window::pollEvents() { glfwPollEvents(); }

GLFWwindow *Window::getWindow() const { return window; }

// private
void Window::framebufferCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void Window::mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (self->firstMouse) {
    self->lastX = xpos;
    self->lastY = ypos;
    self->firstMouse = false;
  }

  float xoffset = xpos - self->lastX;
  float yoffset = self->lastY - ypos;

  self->lastX = xpos;
  self->lastY = ypos;

  self->camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::scrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

  self->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
