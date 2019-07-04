﻿#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>


class _02_HelloTriangle
{
public:
	static int DoMain();
	static bool CheckCompile(unsigned int id);
	static void InitOpenGL();
	static GLFWwindow* InitWindow();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(GLFWwindow* window);
};
