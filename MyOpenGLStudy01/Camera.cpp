#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = _position;
	worldUp = _up;
	yaw = _yaw;
	pitch = _pitch;
	UpdateCameraVectors();
}

Camera::Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = glm::vec3(_posX, _posY, _posZ);
	worldUp = glm::vec3(_upX, _upY, _upZ);
	yaw = _yaw;
	pitch = _pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
		position += front * velocity;
	else if (direction == CameraMovement::BACKWARD)
		position -= front * velocity;
	else if (direction == CameraMovement::LEFT)
		position -= right * velocity;
	else if (direction == CameraMovement::RIGHT)
		position += right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		pitch = min(45.0f, max(1.0f, pitch));
	}
	UpdateCameraVectors();
}

void Camera::ProcesMouseScroll(float yOffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
	{
		zoom -= yOffset;
	}
	//zoom = min(45.0f, max(1.0f, zoom));
}

void Camera::UpdateCameraVectors()
{
	//这里用欧拉角并不完美 可以用四元数
	//代表的摄像机的前轴 乘cos(glm::radians(pitch)) 为了统一normalize
	glm::vec3 _front;
	_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	_front.y = sin(glm::radians(pitch));
	_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
