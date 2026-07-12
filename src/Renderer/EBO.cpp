#include "Renderer/EBO.hpp"

EBO::EBO(const unsigned int *indices, std::size_t size) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::~EBO() { glDeleteBuffers(1, &ID); }

void EBO::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }

void EBO::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
