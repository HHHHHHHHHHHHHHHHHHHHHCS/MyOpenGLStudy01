#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <glad/glad.h>
#include "Shader.h"
#include "CommonBaseScript.h"

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float ZNEAR = 0.1f;
const float ZFAR = 1000.0f;

class Camera
{
public:
	static Camera* instance;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;


	bool isFirst = true;
	float deltaTime;
	float lastFrame;
	double lastXPos;
	double lastYPos;

	Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f)
	       , float _yaw = YAW, float _pitch = PITCH);
	Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch);

	static void AddMouseEvent(GLFWwindow* window, bool hideCursor = true);
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset);

	void DoKeyboardMove(GLFWwindow* window);
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);
	glm::mat4 GetViewMat4() const;
	glm::mat4 GetProjectionMat4() const;
	glm::mat4 GetViewProjection() const;

private:
	void UpdateCameraVectors();
};
