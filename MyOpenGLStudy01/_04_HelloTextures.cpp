#include "_04_HelloTextures.h"


//ʹ��������� ��stb_image.h ��� cpp ʹ��
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

	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	// S->X T->Y W->Z �����3D ������W
	//GL_TEXTURE_WRAP ���ظ���ʽ  GL_TEXTURE_???_FILTER �ǷŴ���С���˲���ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���ز���������
	int width, height, nrChannels;
	//��stb����ͼƬ Ȼ�� �Զ���� width height �� ͨ������
	unsigned char* data = stbi_load("Images/01.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		//��һ���� �������� 1D  2D  3D
		//�ڶ����� ��ǰͼƬ��mipmap�㼶,�����Զ�����,Ҳ�����ֶ��޸Ĳ㼶 ����ͼƬ
		//�������� ��ɫ����
		//���ĸ��� ����Ŀ��
		//������� ����ĸ߶�
		//�������� ����Ϊ0 ��ʷ��������
		//���߸��� ͼƬ��ɫ��ʽ
		//�ڰ˸��� ͼƬ������ɫ����������
		//�ھŸ��� ͼƬ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//�Զ�����mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//�ͷ�ͼƬ
	stbi_image_free(data);


	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� UV 0,0�ڶ���
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1 - 1.0f, // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1 - 0.0f, // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1 - 0.0f, // ����
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1 - 1.0f // ����
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
