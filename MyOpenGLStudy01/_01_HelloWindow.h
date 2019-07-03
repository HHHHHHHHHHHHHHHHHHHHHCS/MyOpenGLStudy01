﻿#pragma once

#include<Windows.h>
#include <stdio.h>
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的
#include <GLFW/glfw3.h>
#include <iostream>

class _01_HelloWindow
{
public:
	static int DoMain();
	static void InitOpenGL();
	static GLFWwindow* InitWindow();
	static void UpdateDraw(GLFWwindow*);
	static void FrameBufferSizeCallback(GLFWwindow*, int, int);
	static void ProcessInput(GLFWwindow*);
};
