#include "Transform.h"

Transform::Transform()
{
	translate = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	update_directional_vector();
}

glm::vec3 Transform::get_translate()
{
	return translate;
}
glm::vec3 Transform::get_rotate()
{
	return rotate;
}
glm::vec3 Transform::get_scale()
{
	return scale;
}
void Transform::set_translate(glm::vec3 _translate)
{
	translate = _translate;

	update_directional_vector();
}
void Transform::set_rotate(glm::vec3 _rotate)
{
	rotate = _rotate;

	update_directional_vector();
}
void Transform::set_scale(glm::vec3 _scale)
{
	scale = _scale;

	update_directional_vector();
}

glm::mat4 Transform::get_model_matrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, translate);

	model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	return model;
}

void Transform::update_directional_vector()
{
	glm::vec3 _front;
	_front.x = cos(glm::radians(rotate.y)) * cos(glm::radians(rotate.x));
	_front.y = sin(glm::radians(rotate.x));
	_front.z = sin(glm::radians(rotate.y)) * cos(glm::radians(rotate.x));

	front = glm::normalize(_front);
	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Transform::get_front()
{
	return front;
}

glm::vec3 Transform::get_up()
{
	return up;
}

glm::vec3 Transform::get_right()
{
	return right;
}

void Transform::move(glm::vec3 _delta)
{
	translate += _delta;

	update_directional_vector();
}

void Transform::move(glm::vec3 _direction, glm::vec1 _velocity)
{
	translate += glm::vec3(_velocity *_direction.x, _velocity *_direction.y, _velocity *_direction.z);

	update_directional_vector();
}