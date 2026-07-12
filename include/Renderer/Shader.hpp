#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "stb_image.hpp"

class Shader {
private:
  unsigned int ID;
  std::string read_shader(const char *shaderPath) const;
  unsigned int compiled_shader(const char *shaderCode, GLenum shaderType) const;
  unsigned int create_and_link_shader(unsigned int vertex,
                                      unsigned int fragment) const;

public:
  // the program ID

  Shader(const char *vertexPath, const char *fragmentPath);
  void use() const;

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;
  unsigned int getID() const;

  ~Shader() { glDeleteProgram(ID); }
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;
};

#endif // !SHADER_H
