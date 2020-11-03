#include "Shader.h"
#include <gtc/type_ptr.inl>


bool Shader::CheckCompileErrors(unsigned int id, bool isShader, const std::string& path)
{
	//如果有编译错误则打印
	char infoLog[512];
	GLint success;

	if (isShader)
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		glGetShaderInfoLog(id, 512, nullptr, infoLog);

	}
	else
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
	}

	if (!success)
	{
		std::cout << path << "->ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return success;
}

Shader::Shader()
{
}

Shader::Shader(const GLchar* fileName, bool haveGS)
{
	std::string path{"Shaders/"};
	path.append(fileName);

	std::string vsPath = path + ".vs";
	std::string fsPath = path + ".fs";
	std::string geometryPath;
	if (haveGS)
	{
		geometryPath = path + ".gs";
	}

	// char szStr[sizeof(vsPath)] = {0};
	// strncpy_s(szStr, vsPath.c_str(), sizeof(szStr) - 1); // 强烈建议拷贝出来


	Init(vsPath.c_str(), fsPath.c_str(), haveGS ? geometryPath.c_str() : nullptr);
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	Init(vertexPath, fragmentPath, geometryPath);
}

void Shader::Init(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	//1.从文件路径中获取顶点/片段着色器
	std::string vertexCode, fragmentCode, geometryCode;
	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	//保证ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ." << std::endl;
	}
	//把string->const char*
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//2.编译着色器
	unsigned int vertex, fragment;

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, true, vertexPath);

	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, true, fragmentPath);

	unsigned int geometry;
	//几何着色器
	if (geometryPath != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gShaderCode = geometryCode.c_str();
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);

		CheckCompileErrors(geometry, true, geometryPath);
	}

	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);

	glLinkProgram(ID);
	CheckCompileErrors(ID, false, "Shader");


	//因为顶点和片元着色器已经加载到程序中了,可以删除了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	//glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(value));
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, &value[0][0]);
}

void Shader::SetVec2(const std::string& name, float v0, float v1) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v0, v1);
}

void Shader::SetVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float v0, float v1, float v2) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float v0, float v1, float v2, float v3) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float* value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::SetVec3Array(const std::string& name, int count, float* address) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, address);
}
