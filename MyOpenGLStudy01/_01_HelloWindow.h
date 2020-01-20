#pragma once
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的 请确保GLAD头文件的引入在GLFW之前
#include<Windows.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <iostream>

class _01_HelloWindow
{
public:
	static int DoMain();
	static void InitOpenGL(bool canResize = false);
	static GLFWwindow* InitWindow();
	static void UpdateDraw(GLFWwindow*);
	static void FrameBufferSizeCallback(GLFWwindow*, int, int);
	static void ProcessInput(GLFWwindow*);
};
