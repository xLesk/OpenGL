#include "Renderer/VBO.hpp"

VBO::VBO(const void *data, GLsizeiptr size) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO() { glDeleteBuffers(1, &ID); }

void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
void VBO::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
