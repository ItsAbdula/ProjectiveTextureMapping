#pragma once

#include <GL/glew.h>

#include <vector>

struct pos3d
{
	float x, y, z;
};

void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources);
GLint compile_shader(const GLint shaderType, const std::string *shaderSource);
GLuint build_program(const std::string name);