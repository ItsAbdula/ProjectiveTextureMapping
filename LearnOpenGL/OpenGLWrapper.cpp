#include "OpenGLWrapper.h"
#include "ShaderType.h"
#include "ResourceLoader.h"

// settings
const unsigned int _SCR_WIDTH = 800;
const unsigned int _SCR_HEIGHT = 600;

// lighting
glm::vec3 _lightPos(1.2f, 20.0f, 2.0f);

Mesh::Mesh(GLuint _nVertex, GLuint _VAO, GLuint *_VBOs)
{
	nVertex = _nVertex;
	VAO = _VAO;
	VBOs = _VBOs;
}

GLuint Mesh::get_vertex_count()
{
	return nVertex;
}
GLuint Mesh::get_VAO()
{
	return VAO;
}
GLuint *Mesh::get_VBOs()
{
	return VBOs;
}

void draw_mesh(Mesh &mesh)
{
	glBindVertexArray(mesh.get_VAO());

	glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());

	glBindVertexArray(0);
}

RenderObject::RenderObject(Mesh * _mesh)
{
	translate = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	mesh = _mesh;

	update_model_matrix();
}

GLuint RenderObject::get_programs()
{
	return prog;
}

GLuint RenderObject::get_vertex_count()
{
	return mesh->get_vertex_count();
}

glm::mat4 RenderObject::get_model_matrix()
{
	return model;
}

void RenderObject::update_model_matrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, translate);

	model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);
}

void RenderObject::set_translate(glm::vec3 _translate)
{
	translate = _translate;

	update_model_matrix();
}

void RenderObject::set_rotate(glm::vec3 _rotate)
{
	rotate = _rotate;

	update_model_matrix();
}

void RenderObject::set_scale(glm::vec3 _scale)
{
	scale = _scale;

	update_model_matrix();
}

void RenderObject::move(glm::vec3 _delta)
{
	translate += _delta;

	update_model_matrix();
}

void RenderObject::move(glm::vec3 _direction, glm::vec1 _velocity)
{
	translate += glm::vec3(_velocity *_direction.x, _velocity *_direction.y, _velocity *_direction.z);

	update_model_matrix();
}

void RenderObject::set_program(GLuint _prog)
{
	prog = _prog;
}

void RenderObject::render(Camera &camera)
{
	glUseProgram(prog);

	auto objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	set_uniform_value(prog, "objectColor", objectColor);

	auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	set_uniform_value(prog, "lightColor", lightColor);
	set_uniform_value(prog, "lightPos", _lightPos);
	set_uniform_value(prog, "viewPos", camera.Position);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 100.0f);
	set_uniform_value(prog, "projection", projection);

	glm::mat4 view = camera.GetViewMatrix();
	set_uniform_value(prog, "view", view);

	set_uniform_value(prog, "model", model);

	draw_mesh(*mesh);

	glUseProgram(0);
}

RenderObject *make_render_object(Mesh *mesh)
{
	RenderObject *ro = new RenderObject{ mesh };

	return ro;
};

GLuint build_program(const std::string name)
{
	std::string shaderSources[5];
	shaderSources[0] = FileSystem::read_file("../Shaders/" + name + "/" + name + ".vert");
	shaderSources[1] = FileSystem::read_file("../Shaders/" + name + "/" + name + ".frag");

	std::vector<GLint> shaderIDs;
	compile_shaders(&shaderIDs, shaderSources);

	auto programID = glCreateProgram();
	for (auto i : shaderIDs)
	{
		glAttachShader(programID, i);
	}
	glLinkProgram(programID);
	for (auto i : shaderIDs)
	{
		glDeleteShader(i);
	}

	return programID;
}

void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources)
{
	shaderIDs->push_back(compile_shader(GL_VERTEX_SHADER, &shaderSources[0]));
	shaderIDs->push_back(compile_shader(GL_FRAGMENT_SHADER, &shaderSources[1]));
}

GLint compile_shader(const GLint shaderType, const std::string *shaderSource)
{
	GLuint shaderID;
	switch (shaderType)
	{
	case (GL_VERTEX_SHADER): shaderID = glCreateShader(GL_VERTEX_SHADER); break;
	case (GL_FRAGMENT_SHADER): shaderID = glCreateShader(GL_FRAGMENT_SHADER); break;
	default:
		GLchar infoLog[512];
		log_error(infoLog, "SHADER::CAN'T_FIND_SHADER_TYPE");

		break;
	}

	const char *c_str = (*shaderSource).c_str();
	glShaderSource(shaderID, 1, &c_str, NULL);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);

		log_error(infoLog, "SHADER::" + parse_shaderType(shaderType) + "::COMPILATION_ERROR");
	}

	return shaderID;
}

GLint link_program(const GLint *shaderIDs)
{
	auto programID = glCreateProgram();
	glAttachShader(programID, shaderIDs[0]);
	glAttachShader(programID, shaderIDs[1]);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		log_error(infoLog, "PROGRAM::LINK_FAILED");
	}

	glDeleteShader(shaderIDs[0]);
	glDeleteShader(shaderIDs[1]);

	return programID;
}

void set_uniform_value(GLuint &prog, const char *name, glm::vec1 value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform1fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec1 value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform1iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec2 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform2fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec2 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform2iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec3 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform3fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec3 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform3iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec4 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform4fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec4 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform4iv(uniform, 1, &value.x);
}

void set_uniform_value(GLuint &prog, const char *name, glm::mat4 &value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
}

GLuint allocate_VBO(const GLuint attribIndex, std::vector<glm::vec3> *VBO)
{
	GLuint VBOIndex = 0;

	glGenBuffers(1, &VBOIndex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ARRAY_BUFFER, VBO->size() * sizeof(glm::vec3), &(VBO->front()), GL_STATIC_DRAW);

	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VBOIndex;
}

GLuint *allocate_VBOs(GLuint VAO, std::vector<std::vector<glm::vec3> *> &vertexInfo)
{
	GLuint *VBOindicies = new GLuint[vertexInfo.size()];

	glBindVertexArray(VAO);

	for (GLuint i = 0; i < vertexInfo.size(); i++)
	{
		VBOindicies[i] = allocate_VBO(i, vertexInfo.at(i));
	}

	glBindVertexArray(0);

	return VBOindicies;
}

GLuint allocate_VAO()
{
	GLuint VAO = 0;

	glGenVertexArrays(1, &VAO);

	return VAO;
}

Mesh *make_mesh(const std::string fileName)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNormals;

	const std::string ext = get_extension(fileName);
	if (ext.compare("obj") == 0)
	{
		openObj(fileName, vertices, vertexNormals);
	}
	else
	{
		std::cout << "Can't Open " + fileName + " Extension." << std::endl;
	}

	std::vector<std::vector<glm::vec3> *> vertexInfo;
	vertexInfo.push_back(&vertices);
	if (vertexNormals.size() > 0)
	{
		vertexInfo.push_back(&vertexNormals);
	}

	auto VAO = allocate_VAO();
	auto VBOs = allocate_VBOs(VAO, vertexInfo);

	Mesh *m = new Mesh(vertices.size(), VAO, VBOs);

	return m;
}

GLuint load_image(const std::string fileName)
{
	int width, height, nrChannels;
	GLuint textureID;

	Image *tex = load_Image(fileName, &width, &height, &nrChannels);
	if (tex != NULL && tex->getData() != NULL)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	free_image(tex);

	return textureID;
}