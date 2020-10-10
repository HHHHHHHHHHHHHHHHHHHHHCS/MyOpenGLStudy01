#include "_45_Debugging.h"
#include "CommonBaseScript.h"
#include "ImageHelper.h"


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


int _45_Debugging::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
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

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();


	return 0;
}
