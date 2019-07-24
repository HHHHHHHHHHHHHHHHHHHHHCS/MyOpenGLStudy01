#pragma once
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的 请确保GLAD头文件的引入在GLFW之前
#include<Windows.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <iostream>


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
	static void HideCursor(GLFWwindow* window);
};
