#pragma once
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>


class _02_HelloTriangle_Practice
{
public:
	static int DoMain();
	static bool CheckCompile(unsigned int id);
	static void InitOpenGL();
	static GLFWwindow* InitWindow();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(GLFWwindow* window);
};
