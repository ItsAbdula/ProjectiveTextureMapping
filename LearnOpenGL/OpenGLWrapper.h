#pragma once
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
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
void draw_mesh(Mesh &mesh);

class Material
{
private:
	GLuint prog;

	GLuint diffuseMap;
	GLuint specularMap;

public:
	Material(GLuint _prog, GLuint _diffuseMap, GLuint _specularMap);

	GLuint get_program();

	GLuint get_diffuseMap();
	GLuint get_specularMap();
};

class RenderObject
{
private:
	GLuint id;

	Mesh *mesh;
	Material *material;

	glm::mat4 model;

	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;

	void update_model_matrix();

public:
	RenderObject(Mesh * _mesh);

	Material *get_material();
	GLuint get_vertex_count();
	glm::mat4 get_model_matrix();

	void set_translate(glm::vec3 _translate);
	void set_rotate(glm::vec3 _rotate);
	void set_scale(glm::vec3 _scale);

	void move(glm::vec3 _delta);
	void move(glm::vec3 _direction, glm::vec1 _velocity);

	void set_material(Material *_material);

	void render(Camera &camera);
	void projective_render(Camera &camera);
};

RenderObject *make_render_object(Mesh *mesh);

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
GLuint allocate_VAO();
Mesh *make_mesh(const std::string fileName);
GLuint load_image(const std::string fileName);