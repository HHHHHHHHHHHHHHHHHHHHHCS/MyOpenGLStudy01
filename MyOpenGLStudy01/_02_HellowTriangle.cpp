#include "_02_HelloTriangle.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

int _02_HelloTriangle::DoMain()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//GL_STATIC_DRAW : 数据不会或几乎不会改变使用.
	//GL_DYNAMIC_DRAW : 数据会被改变很多时候用,把数据写到高速写入的内存部分.
	//GL_STREAM_DRAW : 数据每次绘制时都会改变使用,把数据写到高速内存部分.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertexShaderSource{
		R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)"
	};

	glad_glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检测编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//如果编译失败
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return 0;
}
