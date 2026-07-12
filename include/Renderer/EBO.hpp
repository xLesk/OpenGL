#pragma once

#include <glad/glad.h>
#include <cstddef>

class EBO {
public:
  EBO(const unsigned int *indices, std::size_t size);
  ~EBO();

  void bind() const;
  void unbind() const;

private:
  GLuint ID{};
};
