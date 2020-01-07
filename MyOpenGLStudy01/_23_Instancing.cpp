#include "_23_Instancing.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CommonBaseScript.h"
#include "Camera.h"
#include "Model.h"

int _23_Instancing::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float quadVertices[] = {
		// 位置          // 颜色
		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		0.05f, 0.05f, 0.0f, 1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glBindVertexArray(0);

	Camera camera{};
	camera.AddMouseEvent(window);

	Shader instancingArrayShader{"23_Instancing_Array"};

	glEnable(GL_DEPTH_TEST);

	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = float(x) / 10.0f + offset;
			translation.y = float(y) / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	/*
	//数组会有上限 OPENGL3.0+ 至少是1024个 glGetIntegerv可以查询
	int maxVert, maxGeom, maxFrag;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVert);
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &maxGeom);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFrag);

	std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS:" << maxVert << std::endl;
	std::cout << "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS:" << maxGeom << std::endl;
	std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:" << maxFrag << std::endl;

	instancingArrayShader.Use();
	for (unsigned int i = 0; i < 100; i++)
	{
		std::stringstream ss;
		std::string index;
		ss << i;
		index = ss.str();
		instancingArrayShader.SetVec2(("offsets[" + index + "]").c_str(), translations[i]);
	}
	*/

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(quadVAO);
		instancingArrayShader.Use();
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glfwTerminate();

	return 0;
}
