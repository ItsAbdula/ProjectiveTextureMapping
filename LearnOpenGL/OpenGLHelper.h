#pragma once
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources);
GLint compile_shader(const GLint shaderType, const std::string *shaderSource);
GLuint build_program(const std::string name);
GLuint allocate_VAO(std::vector<std::vector<glm::vec3> *> &attribs);