#include "Renderer/Shader.hpp"
#include <fstream>
#include <ostream>
#include <iostream>
#include <sstream>

// Constructor
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1) Reading the vertex and fragment shader code
  std::string vertexCode = read_shader(vertexPath);
  const char *vShaderCode = vertexCode.c_str();
  std::string fragmentCode = read_shader(fragmentPath);
  const char *fShaderCode = fragmentCode.c_str();

  // 2) Compiling the shaders
  unsigned int vertex = compiled_shader(vShaderCode, GL_VERTEX_SHADER);
  unsigned int fragment = compiled_shader(fShaderCode, GL_FRAGMENT_SHADER);

  // 3) Linking
  ID = create_and_link_shader(vertex, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); };

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
};

std::string Shader::read_shader(const char *shaderPath) const {
  std::string shaderCode;
  std::ifstream ShaderFile;

  ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    ShaderFile.open(shaderPath);
    std::stringstream ShaderStream;
    ShaderStream << ShaderFile.rdbuf();

    ShaderFile.close();

    shaderCode = ShaderStream.str();
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    return "";
  }
  return shaderCode;
}

unsigned int Shader::compiled_shader(const char *shaderCode,
                                     GLenum shaderType) const {
  unsigned int shader;
  int success;
  char infoLog[512];

  shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED" << infoLog << std::endl;
  }
  return shader;
}

unsigned int Shader::create_and_link_shader(unsigned int vertex,
                                            unsigned fragment) const {
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog
              << std::endl;
  }
  return program;
}

unsigned int Shader::getID() const { return ID; }
