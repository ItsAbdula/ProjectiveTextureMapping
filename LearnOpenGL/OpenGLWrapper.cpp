#include "OpenGLWrapper.h"
#include "ShaderType.h"
#include "ResourceLoader.h"

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
	if (ext.compare("model") == 0)
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