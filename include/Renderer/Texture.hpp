#pragma once
#include <string>

class Texture {
public:
  explicit Texture(std::string path, bool flip = true);
  ~Texture();

  void bind(unsigned int unit = 0) const;
  void unbind() const;

  unsigned int getID() const;

private:
  unsigned int ID{};
};
