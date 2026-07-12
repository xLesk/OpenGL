#include <GLFW/glfw3.h>
#include <iostream>

void mouse(GLFWwindow *, double x, double y) { std::cout << "mouse\n"; }

int main() {
  glfwInit();

  glfwSetErrorCallback([](int error, const char *desc) {
    std::cout << error << " : " << desc << '\n';
  });

  std::cout << glfwPlatformSupported(GLFW_PLATFORM_WAYLAND) << '\n';
  std::cout << glfwPlatformSupported(GLFW_PLATFORM_X11) << '\n';
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

  GLFWwindow *window = glfwCreateWindow(800, 600, "test", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glfwSetCursorPosCallback(window, mouse);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      std::cout << "W\n";

    glfwSwapBuffers(window);
  }

  glfwTerminate();
}
