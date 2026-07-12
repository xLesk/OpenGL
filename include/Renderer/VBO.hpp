#pragma once
#include <glad/glad.h>

class VBO {
public:
  VBO(const void *data, GLsizeiptr size);
  ~VBO();

  void bind() const;
  void unbind() const;

private:
  GLuint ID{};
};
