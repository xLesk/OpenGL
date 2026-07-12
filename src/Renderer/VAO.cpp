#include "Renderer/VAO.hpp"
#include "Renderer/VBO.hpp"
#include <cstddef>

VAO::VAO() { glGenVertexArrays(1, &ID); }

VAO::~VAO() { glDeleteVertexArrays(1, &ID); }

void VAO::bind() const { glBindVertexArray(ID); }

void VAO::unbind() const { glBindVertexArray(0); }

void VAO::linkAttrib(const VBO &vbo, GLuint layout, GLint components,
                     GLenum type, GLsizei stride, std::size_t offset) const {
  bind();
  vbo.bind();

  glVertexAttribPointer(layout, components, type, GL_FALSE, stride,
                        reinterpret_cast<void *>(offset));

  glEnableVertexAttribArray(layout);
  vbo.unbind();
}
