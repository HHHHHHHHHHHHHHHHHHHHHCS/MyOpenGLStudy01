#pragma once


#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <glad/glad.h>
#include "Shader.h"
#include "CommonBaseScript.h"

class _07_Camera
{
public:
	static bool isFirst;
	static float deltaTime;
	static float lastFrame;
	static glm::vec3 cameraPos;
	static glm::vec3 cameraFront;
	static glm::vec3 cameraUp;
	static float lastX;
	static float lastY;
	static float yaw;
	static float pitch;
	static float fov;
	static int DoMain();
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset);
};
