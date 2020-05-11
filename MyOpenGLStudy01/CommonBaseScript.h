#pragma once

#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的 请确保GLAD头文件的引入在GLFW之前
#include <GLFW/glfw3.h>

#include<Windows.h>
#include <iostream>
#include <stdio.h>

//extern 是声明 没有定义 
extern const unsigned int SCR_WIDTH ;
extern const unsigned int SCR_HEIGHT;

/*
 * 记录了一些公共的基础功能
 */
class CommonBaseScript
{
public:
	static bool clickKeys[];
	static bool keys[];
	static void InitOpenGL(bool canResize = false);
	static GLFWwindow* InitWindow(int width = SCR_WIDTH,int height = SCR_HEIGHT);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(GLFWwindow* window);
	static void ProcessKeyClick();
	static void RegisterKeyEvent(GLFWwindow* window);
	static bool CheckCompile(unsigned id);
	static void UsePolygonMode();
	static void HideCursor(GLFWwindow* window);
	static void ShowCursor(GLFWwindow* window);
private:
	static bool lastKeys[];
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};
