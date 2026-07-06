#include "Core/VAO_VBO_EBO.hpp"
#include <glad/glad.h>

void gen_id_and_bind_VAO(unsigned int &VAO) {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
}

void gen_id_and_bind_VBO(unsigned int &VBO, const float *vertices,
                         size_t size) {
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void gen_id_and_bind_EBO(unsigned int &EBO, const unsigned int *indices,
                         size_t size) {
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
