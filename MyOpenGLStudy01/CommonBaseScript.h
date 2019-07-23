#pragma once
#include "_01_HelloWindow.h"

//extern 是声明 没有定义 
extern const unsigned int SCR_WIDTH ;
extern const unsigned int SCR_HEIGHT;

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
