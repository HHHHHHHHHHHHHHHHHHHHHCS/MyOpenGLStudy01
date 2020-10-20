#include "_46_Text.h"
#include  <iostream>
#include <GLFW/glfw3.h>
#include "CommonBaseScript.h"
#include <mat4x4.hpp>
#include "Camera.h"
//下面两个一起出现
#include  <ft2build.h>
#include FT_FREETYPE_H


std::map<GLchar, _46_Text::Character> _46_Text::characters{};
GLuint _46_Text::textVAO = 0;


int _46_Text::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//初始化
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE:Could not init FreeType Library" << std::endl;
	}

	//加载字体库
	FT_Face face;
	if (FT_New_Face(ft, "Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE:Failed to laod font" << std::endl;
	}

	//字体多大,将宽度值设为0表示我们要从字体面通过给定的高度中动态计算出字形的宽度
	FT_Set_Pixel_Sizes(face, 0, 48);

	//加载字符'x'
	//通过将FT_LOAD_RENDER设为加载标记之一，我们告诉FreeType去创建一个8位的灰度位图，
	//我们可以通过face->glyph->bitmap来访问这个位图
	//width		face->glyph->bitmap.width	位图宽度（像素）
	//height	face->glyph->bitmap.rows	位图高度（像素）
	//bearingX	face->glyph->bitmap_left	水平距离，即位图相对于原点的水平位置（像素）
	//bearingY	face->glyph->bitmap_top		垂直距离，即位图相对于基准线的垂直位置（像素）
	//advance	face->glyph->advance.x		水平预留值，即原点到下一个字形原点的水平距离（单位：1 / 64像素）
	if (FT_Load_Char(face, 'x',FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE:Failed to load Glyph" << std::endl;
	}

	//OpenGL要求所有的纹理都是4字节对齐的，即纹理的大小永远是4字节的倍数
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置为1字节对齐 即 禁用字节对齐限制
	//GLubyte -> unsigned char
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c,FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE:Failed to load Glyph" << std::endl;
			continue;
		}

		//生成纹理
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
		             face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//设置纹理选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//储存字符供之后使用
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	//清理资源
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	//Text VAO
	BindTextVAO();

	Shader textShader{"46_Text"};

	//因为字体是Alpha 所以要启动Alpha 混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//绘制UI的正交投影矩阵
	glm::mat4 uiProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.8f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void _46_Text::BindTextVAO()
{
	//给UI文字用的Quad  VBO数据后面填充
	GLuint VBO;
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//float4 * 6 => quad  因为数据是后面补充的 所以用GL_DYNAMIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 6, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	// 4 个 float 为一组数据    每次间隔 4*sizeof(float)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void _46_Text::RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	s.Use();
	s.SetVec3("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = characters[*c];

		//左边位置 根据字体的宽度
		GLfloat xpos = x + ch.Bearing.x * scale;
		//高度要计算 上下的偏移量 
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
	}
}
