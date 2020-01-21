#include "CommonBaseScript.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern const unsigned int SCR_WIDTH = 800;
extern const unsigned int SCR_HEIGHT = 600;

bool CommonBaseScript::keys[1024];

void CommonBaseScript::InitOpenGL(bool canReisze)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用OpenGL核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //苹果显示需要
#endif
	glfwWindowHint(GLFW_RESIZABLE, canReisze);
}

GLFWwindow* CommonBaseScript::InitWindow()
{
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);//设置回调函数，改变窗口大小

	//初始化glad ,把OpenGL的函数指针导入给GLAD ,但是要在窗口初始化之后执行
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	return window;
}

//当用户改变窗口的大小的时候，视口也应该被调整的回调
void CommonBaseScript::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //设置窗口的维度
}

//按下ESC 关闭
void CommonBaseScript::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//按键注册事件
void CommonBaseScript::RegisterKeyEvent(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}

//按键执行事件
void CommonBaseScript::KeyCallback(GLFWwindow* window, int key, int scancode ,int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

/*
 * 检测编译用
 */
bool CommonBaseScript::CheckCompile(unsigned int id)
{
	//检测编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	//如果编译失败
	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return success;
}

/*
 * 使用线框模式
 */
void CommonBaseScript::UsePolygonMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*
 * 隐藏光标
 */
void CommonBaseScript::HideCursor(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}