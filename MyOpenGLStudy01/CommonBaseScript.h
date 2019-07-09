#pragma once
#include "_01_HelloWindow.h"

/*
 * 记录了一些公共的基础功能
 */
class CommonBaseScript
{
public:

	static void InitOpenGL();
	static GLFWwindow* InitWindow();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(GLFWwindow* window);
	static bool CheckCompile(unsigned id);
	static void UsePolygonMode();
};
