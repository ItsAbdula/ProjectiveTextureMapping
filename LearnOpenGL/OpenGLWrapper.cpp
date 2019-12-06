#include "OpenGLWrapper.h"
#include "ShaderType.h"

#include "FileIO.h"
#include "Logger.h"

GLuint build_program(const std::string name)
{
	FileIO fileIO;
	std::string shaderSources[5];
	shaderSources[0] = fileIO.read_file("../Shaders/" + name + "/" + name + ".vert");
	shaderSources[1] = fileIO.read_file("../Shaders/" + name + "/" + name + ".frag");

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

GLint link_program(const GLint  *shaderIDs)
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

GLuint allocate_VAO(std::vector<std::vector<glm::vec3> *> &VBOs)
{
	const GLuint nVBO = VBOs.size();

	GLuint VAO;
	GLuint *VBO = new GLuint[nVBO];
	GLuint i = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	for (i = 0; i < nVBO; i++)
	{
		glGenBuffers(1, &VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, VBOs.at(i)->size() * sizeof(glm::vec3), &(VBOs.at(i)->front()), GL_STATIC_DRAW);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}