﻿#pragma once
#include <glad/glad.h>
#include <vec2.hpp>
#include <map>

class _46_Text
{
public:
	struct Character
	{
		GLuint TextureID; //字形纹理的ID
		glm::ivec2 Size; //字形大小
		glm::ivec2 Bearing; //从基准线到字形左部/顶部的偏移值
		GLuint Advance; //原点距下一个字形原点的距离
	};

	static std::map<GLchar, Character> characters;

	static int DoMain();
};
