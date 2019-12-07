#pragma once
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileSystem.h"
#include "Logger.h"

class Mesh
{
private:
	GLuint nVertex;
	GLuint VAO;
	GLuint *VBOs;

public:
	Mesh(GLuint _nVertex, GLuint _VAO, GLuint *_VBOs);
	GLuint get_vertex_count();
	GLuint get_VAO();
	GLuint *get_VBOs();
};

GLint compile_shader(const GLint shaderType, const std::string *shaderSource);
void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources);
GLuint build_program(const std::string name);

void set_uniform_value(GLuint &prog, const char *name, glm::vec1 value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec1 value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec2 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec2 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec3 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec3 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec4 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec4 &value);

void set_uniform_value(GLuint &prog, const char *name, glm::mat4 &value);

GLuint allocate_VBO(const GLuint attribIndex, std::vector<glm::vec3> *VBO);
GLuint *allocate_VBOs(GLuint VAO, std::vector<std::vector<glm::vec3> *> &vertexInfo);
GLuint allocate_VAO(std::vector<std::vector<glm::vec3> *> &VBOs);
Mesh *make_mesh(const std::string fileName);
GLuint load_image(const std::string fileName);