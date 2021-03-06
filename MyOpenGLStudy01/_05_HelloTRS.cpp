﻿#include "_05_HelloTRS.h"

#include "stb_image.h"

int _05_HelloTRS::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//这个是翻转读取的图片
	//因为OPENGL的UV是反着的 要么1-UV.Y  要么翻转图片
	//stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	// S->X T->Y W->Z 如果是3D 的则有W
	//GL_TEXTURE_WRAP 是重复方式  GL_TEXTURE_???_FILTER 是放大缩小的滤波方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int width, height, nrChannels;
	//用stb载入图片 然后 自动填充 width height 和 通道数量
	unsigned char* data = stbi_load("Images/01.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		//第一个是 纹理类型 1D  2D  3D
		//第二个是 当前图片的mipmap层级,可以自动生成,也可以手动修改层级 设置图片
		//第三个是 颜色类型
		//第四个是 纹理的宽度
		//第五个是 纹理的高度
		//第六个是 总是为0 历史遗留问题
		//第七个是 图片颜色格式
		//第八个是 图片单个颜色的数据类型
		//第九个是 图片的数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//自动生成mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//释放图片
	stbi_image_free(data);


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	data = stbi_load("Images/02.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//释放图片
	stbi_image_free(data);


	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 UV 0,0在顶部
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1 - 1.0f, // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1 - 0.0f, // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1 - 0.0f, // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1 - 1.0f // 左上
	};


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader{"05_HelloTRS"};

	shader.Use();


	//0.9.9版本以上需要手动初始化矩阵  不然会为0
	//初始化为1的单位矩阵
	glm::mat4 trans = glm::mat4(1.0f);

	glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);

	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

	vec = trans * vec;

	std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;

	//trans = glm::translate(trans, glm::vec3(-1.0f, -1.0f, 0.0f));

	//角度转化为弧度
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	//矩阵ID  矩阵数量  主序  参数类型改变
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


	//如果就一个贴图则不用设置  要在Shader 激活 之后
	glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0); // 手动设置
	shader.SetInt("secondaryTex", 1); // 或者使用着色器类设置

	glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元 0~15张图给我们使用
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 trans(1);
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}
