#include "Light.h"

Light::Light(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular)
{
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
}

Transform *Light::get_transform()
{
	return &transform;
}

glm::vec3 DirectionalLight::get_light_direction()
{
	return get_transform()->get_front();
}