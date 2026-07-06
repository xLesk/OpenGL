#pragma once
#include <cstddef>
void gen_id_and_bind_VAO(unsigned int &VAO);
void gen_id_and_bind_VBO(unsigned int &VBO, float const *vertices, size_t size);
void gen_id_and_bind_EBO(unsigned int &EBO, unsigned int const *indices,
                         size_t size);
