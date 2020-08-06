#include "_40_PBR_Lighting.h"
#include <glad/glad.h>
#include <vector>
#include <vec3.hpp>
#include <vec2.hpp>
#include "CommonBaseScript.h"
#include "Shader.h"
#include "Camera.h"

class Camera;
unsigned int _40_PBR_Lighting::sphereVAO = 0;
unsigned int _40_PBR_Lighting::sphereIndexCount = 0;

int _40_PBR_Lighting::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	BindSphereVAO();

	Shader shader{"40_PBR_Lighting"};


	//lights
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f, 10.0f, 10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
	};

	//Camera
	//-----------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);


	shader.Use();
	shader.SetVec3("albedo", 0.5f, 0.0f, 0.0f);
	shader.SetFloat("ao", 1.0f);
	shader.SetMat4("projection", camera.GetProjectionMat4());

	glEnable(GL_DEPTH_TEST);

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.Use();
		glm::mat4 view = camera.GetViewMat4();
		shader.SetMat4("view", view);
		shader.SetVec3("cameraPos", camera.position);

		glm::mat4 model = glm::mat4(1.0f);

		//渲染灯光
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime()*5.0)*5.0, 0.0, 0.0);
			newPos = lightPositions[i];//移动
			shader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			shader.SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			shader.SetMat4("model", model);
			RenderSphere();
		}

		//渲染PBR
		for (int row = 0; row < nrRows; ++row)
		{
			shader.SetFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				shader.SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					                       (col - (nrColumns / 2)) * spacing,
					                       (row - (nrRows / 2)) * spacing,
					                       0.0f
				                       ));
				shader.SetMat4("model", model);
				RenderSphere();
			}
		}



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}


void _40_PBR_Lighting::BindSphereVAO()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;

	//https://baike.baidu.com/item/%E7%90%83%E5%9D%90%E6%A0%87%E7%B3%BB/8315363?fr=aladdin
	//球面坐标
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.emplace_back(xPos, yPos, zPos);
			uv.emplace_back(xSegment, ySegment);
			normals.emplace_back(xPos, yPos, zPos);
		}
	}

	//4个点画一个quad 错行 合成
	bool oddRow = false; //奇数行
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) //偶数行
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);

		if (!uv.empty())
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (!normals.empty())
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	glGenVertexArrays(1, &sphereVAO);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	const unsigned int stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float)));
}

void _40_PBR_Lighting::RenderSphere()
{
	glBindVertexArray(sphereVAO);

	// https://blog.csdn.net/vblegend_2013/article/details/85345102
	// GL_TRIANGLE_STRIP则稍微有点复杂。
	// 其规律是：
	// 构建当前三角形的顶点的连接顺序依赖于要和前面已经出现过的2个顶点组成三角形的当前顶点的序号的奇偶性（如果从0开始）：
	// 如果当前顶点是奇数：
	// 组成三角形的顶点排列顺序：T = [n - 1 n - 2 n].
	// 如果当前顶点是偶数：
	// 组成三角形的顶点排列顺序：T = [n - 2 n - 21 n].
	glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
}
