#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	instance = this;
	position = _position;
	worldUp = _up;
	yaw = _yaw;
	pitch = _pitch;
	lastFrame = glfwGetTime();
	UpdateCameraVectors();
}

Camera::Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	instance = this;
	position = glm::vec3(_posX, _posY, _posZ);
	worldUp = glm::vec3(_upX, _upY, _upZ);
	yaw = _yaw;
	pitch = _pitch;
	lastFrame = glfwGetTime();
	UpdateCameraVectors();
}

void Camera::AddMouseEvent(GLFWwindow* window, bool hideCursor)
{
	if (hideCursor)
	{
		//隐藏光标
		CommonBaseScript::HideCursor(window);
	}


	//鼠标移动事件
	glfwSetCursorPosCallback(window, MouseCallback);
	//鼠标滚轮移动事件
	glfwSetScrollCallback(window, ScrollCallBack);
}

void Camera::DoKeyboardMove(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
}

void Camera::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	instance->ProcessMouseMovement(xPos, yPos, true);
}

void Camera::ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
{
	instance->ProcesMouseScroll(yOffset);
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

glm::mat4 Camera::GetModelMat4() const
{
	glm::mat4 val = glm::mat4(1.0f);
	val = glm::translate(val, position);
	return val;
}

glm::mat4 Camera::GetViewMat4() const
{
	return glm::lookAt(position, position + front, up);;
}

glm::mat4 Camera::GetProjectionMat4() const
{
	return glm::perspective(glm::radians(zoom), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 1000.0f);
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
