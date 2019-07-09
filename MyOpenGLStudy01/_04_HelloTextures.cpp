#include "_04_HelloTextures.h"


//使用了这个宏 把stb_image.h 变成 cpp 使用
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int _04_HelloTextures::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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

	Shader shader{"04_HelloTextures"};

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	return 0;
}
