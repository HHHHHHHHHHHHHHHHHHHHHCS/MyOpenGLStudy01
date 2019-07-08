#pragma once
#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//程序ID
	unsigned int ID;
	//构造 读取并且构造着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//使用程序
	void Use() const;
	//uniform工具函数
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
};
