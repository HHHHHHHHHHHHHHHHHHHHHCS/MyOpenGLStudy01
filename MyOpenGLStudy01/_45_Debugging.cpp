﻿#include "_45_Debugging.h"
#include "CommonBaseScript.h"
#include "ImageHelper.h"

bool _45_Debugging::notInitDebugging = false;
Shader _45_Debugging::debuggingDisplayShader{};
unsigned int _45_Debugging::debuggingQuadVAO = 0;

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << "(" << line << ")" << std::endl;
		return errorCode;
	}
}

//__FILE__, __LINE__ 它们会在编译的时候被替换成编译时对应的文件与行号
#define MyGLCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam)
{
	// 忽略一些不重要的错误/警告代码
	//如NVidia驱动中的131185仅告诉我们缓冲成功创建了
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	//可以加断点看栈堆
	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API: std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void _45_Debugging::DisplayFramebufferTexture(GLuint textureID)
{
	if (!notInitDebugging)
	{
		notInitDebugging = true;

		debuggingDisplayShader = Shader{"45_Debugging"};
		debuggingDisplayShader.Use();
		debuggingDisplayShader.SetInt("fboAttachment", 0);

		float debuggingVertices[] = {
			0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &debuggingQuadVAO);
		glBindVertexArray(debuggingQuadVAO);
		unsigned int debuggingQuadVBO;
		glGenBuffers(1, &debuggingQuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, debuggingQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(debuggingVertices), &debuggingVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	debuggingDisplayShader.Use();
	glBindVertexArray(debuggingQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}


int _45_Debugging::DoMain()
{
	//可以检查shader
	//https://github.com/KhronosGroup/glslang
	//glsllangvalidator shaderFile.vert


	CommonBaseScript::InitOpenGL();
	//在调试上下文中使用OpenGL会明显更缓慢一点，所以当你在优化或者发布程序之前请将这一GLFW调试请求给注释掉。
	//要在glfwCreateWindow之前调用
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	GLFWwindow* window = CommonBaseScript::InitWindow();

	//glewInit()会设置一个GL_INVALID_ENUM的错误标记，所以第一次调用的glGetError永远会猝不及防地给你返回一个错误代码
	//glGetError();

	if (window == nullptr)
	{
		return -1;
	}

	//要检查我们是否成功地初始化了调试上下文，我们可以对OpenGL进行查询
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "DEBUG OPEN\n";
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); //确保同步显示错误
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		//过滤调试输出
		glDebugMessageControl(GL_DEBUG_SOURCE_API,
		                      GL_DEBUG_TYPE_ERROR,
		                      GL_DEBUG_SEVERITY_HIGH,
		                      0, nullptr, GL_TRUE);
	}


	std::cout << "0:" << glGetError() << std::endl; // 返回 0 (无错误)

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	std::cout << "1:" << glGetError() << std::endl; // 返回 0 (无错误)

	glTexImage2D(GL_TEXTURE_3D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	std::cout << "2:" << glGetError() << std::endl; // 返回 1280 (非法枚举)

	unsigned int colorBuffers[5];
	glGenTextures(-5, colorBuffers);
	std::cout << "3:" << glGetError() << std::endl; // 返回 1281 (非法值)

	std::cout << "4:" << glGetError() << std::endl; // 返回 0 (无错误)

	//=====================================================
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);
	MyGLCheckError();


	//=====================================================
	unsigned int debugFBO;
	glGenFramebuffers(1, &debugFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, debugFBO);

	unsigned int debugAttachment0;
	glGenTextures(1, &debugAttachment0);
	glBindTexture(GL_TEXTURE_2D, debugAttachment0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int debugDepth;
	glGenRenderbuffers(1, &debugDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, debugDepth);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, debugAttachment0, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, debugDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);

		glBindFramebuffer(GL_FRAMEBUFFER, debugFBO);
		glClearColor(0.6f, 0.1f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.8f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DisplayFramebufferTexture(debugAttachment0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();


	return 0;
}
