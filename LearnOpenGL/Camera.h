#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	Transform transform;

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		transform.set_translate(position);

		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;

		transform.set_rotate(glm::vec3(PITCH, YAW, 0));
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		transform.set_translate(glm::vec3(posX, posY, posZ));

		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;

		transform.set_rotate(glm::vec3(PITCH, YAW, 0));
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(transform.get_translate(), transform.get_translate() + transform.get_front(), transform.get_up());
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			transform.set_translate(transform.get_translate() += transform.get_front() * velocity);
		if (direction == BACKWARD)
			transform.set_translate(transform.get_translate() -= transform.get_front() * velocity);
		if (direction == LEFT)
			transform.set_translate(transform.get_translate() -= transform.get_right() * velocity);
		if (direction == RIGHT)
			transform.set_translate(transform.get_translate() += transform.get_right() * velocity);
	}

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		transform.set_rotate(glm::vec3(Pitch, Yaw, 0));
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}
};
#endif