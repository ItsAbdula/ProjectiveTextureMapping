#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"

class Light
{
private:
	Transform transform;

public:
	Light(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);

	Transform *get_transform();

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class DirectionalLight : Light
{
public:
	glm::vec3 get_light_direction();
};