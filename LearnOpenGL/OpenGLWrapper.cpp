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

Material::Material(GLuint _prog, GLuint _diffuseMap, GLuint _specularMap)
{
	prog = _prog;

	diffuseMap = _diffuseMap;
	specularMap = _specularMap;
}

GLuint Material::get_program()
{
	return prog;
}
GLuint Material::get_diffuseMap()
{
	return diffuseMap;
}
GLuint Material::get_specularMap()
{
	return specularMap;
}

RenderObject::RenderObject(Mesh * _mesh)
{
	transform = Transform();
	mesh = _mesh;
}

Material *RenderObject::get_material()
{
	return material;
}

GLuint RenderObject::get_vertex_count()
{
	return mesh->get_vertex_count();
}

Transform *RenderObject::get_transform()
{
	return &transform;
}

void RenderObject::set_material(Material *_material)
{
	material = _material;
}

void RenderObject::render(Camera &camera)
{
	auto prog = material->get_program();
	glUseProgram(prog);

	set_uniform_value(prog, "light.position", _lightPos);
	set_uniform_value(prog, "viewPos", camera.Position);

	glm::vec3 a = { 0.2f, 0.2f, 0.2f };
	glm::vec3 d = { 0.5f, 0.5f, 0.5f };
	glm::vec3 f = { 1.0f, 1.0f, 1.0f };
	set_uniform_value(prog, "light.ambient", a);
	set_uniform_value(prog, "light.diffuse", d);
	set_uniform_value(prog, "light.specular", f);

	set_uniform_value(prog, "material.shininess", glm::fvec1{ 64.0f });

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = transform.get_model_matrix();

	set_uniform_value(prog, "projection", projection);
	set_uniform_value(prog, "view", view);
	set_uniform_value(prog, "model", model);

	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->get_diffuseMap());
	}

	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
	}

	draw_mesh(*mesh);

	glUseProgram(0);
}

void RenderObject::projective_render(Camera &camera)
{
	glm::mat4 bias = { 0.5f, 0.0f, 0.0f, 0.5f,
							0.0f, 0.5f, 0.0f, 0.5f,
							0.0f, 0.0f, 0.5f, 0.5f,
							0.0f, 0.0f, 0.0f, 1.0f };

	glm::mat4 projector_view = glm::lookAt(camera.transform.get_translate(), camera.transform.get_translate() + camera.transform.get_front(), camera.transform.get_up());
	glm::mat4 projector_projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.1f, 100.0f);

	auto prog = material->get_program();
	glUseProgram(prog);

	set_uniform_value(prog, "light.position", _lightPos);
	set_uniform_value(prog, "viewPos", camera.Position);

	glm::vec3 a = { 0.2f, 0.2f, 0.2f };
	glm::vec3 d = { 0.5f, 0.5f, 0.5f };
	glm::vec3 f = { 1.0f, 1.0f, 1.0f };
	set_uniform_value(prog, "light.ambient", a);
	set_uniform_value(prog, "light.diffuse", d);
	set_uniform_value(prog, "light.specular", f);

	set_uniform_value(prog, "material.shininess", glm::fvec1{ 64.0f });

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = transform.get_model_matrix();

	set_uniform_value(prog, "projectorBias", bias);
	set_uniform_value(prog, "projectorProjection", projector_projection);
	set_uniform_value(prog, "projectorView", projector_view);

	set_uniform_value(prog, "projection", projection);
	set_uniform_value(prog, "view", view);
	set_uniform_value(prog, "model", model);

	{
		set_uniform_value(prog, "material.diffuse", glm::ivec1{ 0 });
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->get_diffuseMap());
	}

	{
		set_uniform_value(prog, "material.specular", glm::ivec1{ 1 });
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
	}

	{
		set_uniform_value(prog, "projImage", glm::ivec1{ 2 });
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
	}

	draw_mesh(*mesh);

	glUseProgram(0);
}

RenderObject *make_render_object(Mesh *mesh)
{
	RenderObject *ro = new RenderObject(mesh);

	return ro;
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
void set_uniform_value(GLuint &prog, const char *name, glm::vec4 value)
{
	auto uniform = glGetUniformLocation(prog, name);
	glUniform4fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec4 value)
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

GLuint allocate_VBO(const GLuint attribIndex, std::vector<glm::vec2> *VBO)
{
	GLuint VBOIndex = 0;

	glGenBuffers(1, &VBOIndex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ARRAY_BUFFER, VBO->size() * sizeof(glm::vec2), &(VBO->front()), GL_STATIC_DRAW);

	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VBOIndex;
}

GLuint *allocate_VBOs(GLuint VAO, std::vector<std::vector<glm::vec3> *> &vertexInfoVec3, std::vector<std::vector<glm::vec2> *> &vertexInfoVec2)
{
	auto size = vertexInfoVec3.size() + vertexInfoVec2.size();
	GLuint *VBOindicies = new GLuint[size];

	glBindVertexArray(VAO);

	GLuint i = 0;
	for (i = 0; i < vertexInfoVec3.size(); i++)
	{
		VBOindicies[i] = allocate_VBO(i, vertexInfoVec3.at(i));
	}
	for (i = vertexInfoVec3.size(); i < size; i++)
	{
		VBOindicies[i] = allocate_VBO(i, vertexInfoVec2.at(i - vertexInfoVec3.size()));
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
	std::vector<glm::vec2> vertexTexCoord;
	std::vector<glm::vec3> vertexNormals;

	const std::string ext = get_extension(fileName);
	if (ext.compare("obj") == 0)
	{
		openObj(fileName, vertices, vertexTexCoord, vertexNormals);
	}
	else
	{
		std::cout << "Can't Open " + fileName + " Extension." << std::endl;
	}

	std::vector<std::vector<glm::vec3> *> vertexInfoVec3;
	std::vector<std::vector<glm::vec2> *> vertexInfoVec2;

	vertexInfoVec3.push_back(&vertices);
	if (vertexTexCoord.size() > 0)
	{
		vertexInfoVec2.push_back(&vertexTexCoord);
	}
	if (vertexNormals.size() > 0)
	{
		vertexInfoVec3.push_back(&vertexNormals);
	}

	auto VAO = allocate_VAO();
	auto VBOs = allocate_VBOs(VAO, vertexInfoVec3, vertexInfoVec2);

	Mesh *m = new Mesh(vertices.size(), VAO, VBOs);

	return m;
}

GLuint load_texture(const std::string fileName)
{
	return load_image(fileName, ImageType::REPEAT);
}

GLuint load_image(const std::string fileName, const ImageType type)
{
	int width, height, nrChannels;
	GLuint textureID;

	Image *tex = make_Image(fileName, &width, &height, &nrChannels);
	if (tex != NULL && tex->getData() != NULL)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		if (type == ImageType::REPEAT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else if (type == ImageType::CLAMP)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, tex->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	free_image(tex);

	return textureID;
}