#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	void update_directional_vector();

public:
	Transform();

	glm::vec3 get_translate();
	glm::vec3 get_rotate();
	glm::vec3 get_scale();
	void set_translate(glm::vec3 _translate);
	void set_rotate(glm::vec3 _rotate);
	void set_scale(glm::vec3 _scale);

	glm::vec3 get_front();
	glm::vec3 get_up();
	glm::vec3 get_right();

	glm::mat4 get_model_matrix();

	void move(glm::vec3 _delta);
	void move(glm::vec3 _direction, glm::vec1 _velocity);
};

#endif