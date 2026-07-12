#pragma once
#include <cstddef>
#include <glad/glad.h>

class VBO;

class VAO {
public:
  VAO();
  ~VAO();

  void bind() const;
  void unbind() const;
  void linkAttrib(const VBO &vbo, GLuint layout, GLint components, GLenum type,
                  GLsizei stride, std::size_t offset) const;

private:
  GLuint ID{};
};
