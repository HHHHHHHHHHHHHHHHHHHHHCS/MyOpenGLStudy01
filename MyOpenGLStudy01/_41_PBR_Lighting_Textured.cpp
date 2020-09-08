#include "_41_PBR_Lighting_Textured.h"
#include "CommonBaseScript.h"
#include <vector>
#include <vec3.hpp>
#include <vec2.hpp>
#include "Shader.h"
#include "Camera.h"
#include "ImageHelper.h"

unsigned int _41_PBR_Lighting_Textured::sphereVAO = 0;
unsigned int _41_PBR_Lighting_Textured::sphereIndexCount = 0;

int _41_PBR_Lighting_Textured::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	BindSphereVAO();

	Shader shader{"41_PBR_Lighting_Textured"};

	// camera
	// --------------------------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);


	glEnable(GL_DEPTH_TEST);

	//lights
	//-----------------
	glm::vec3 lightPositions[] =
	{
		glm::vec3(0.0f, 0.0f, 10.0f),
	};
	glm::vec3 lightColors[] =
	{
		glm::vec3(150.0f, 150.0f, 150.0f),
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	// load PBR material textures
	// --------------------------
	unsigned int albedo = ImageHelper::LoadTexture("pbr/rusted_iron/albedo.png");
	unsigned int normal = ImageHelper::LoadTexture("pbr/rusted_iron/normal.png");
	unsigned int metallic = ImageHelper::LoadTexture("pbr/rusted_iron/metallic.png");
	unsigned int roughness = ImageHelper::LoadTexture("pbr/rusted_iron/roughness.png");
	unsigned int ao = ImageHelper::LoadTexture("pbr/rusted_iron/ao.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);


	shader.Use();
	shader.SetInt("albedoMap", 0);
	shader.SetInt("normalMap", 1);
	shader.SetInt("metallicMap", 2);
	shader.SetInt("roughnessMap", 3);
	shader.SetInt("aoMap", 4);
	shader.SetMat4("projection", camera.GetProjectionMat4());
	shader.SetInt("lightCount", sizeof(lightPositions) / sizeof(lightPositions[0]));

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4{1.0f};

		shader.Use();
		shader.SetMat4("view", camera.GetViewMat4());
		shader.SetVec3("cameraPos", camera.position);

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(glm::sin(glfwGetTime() * 5.0) * 5.0, 0, 0);
			newPos = lightPositions[i];
			shader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			shader.SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4{1.0f};
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			shader.SetMat4("model", model);
			RenderSphere();
		}

		for (int row = 0; row < nrRows; ++row)
		{
			shader.SetFloat("metallicIntensity", static_cast<float>(row + 1) / nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				shader.SetFloat("roughnessIntensity", static_cast<float>(col + 1) / nrColumns);
				model = glm::mat4{1.0f};
				model = glm::translate(model, glm::vec3(
					                       static_cast<float>(col - (nrColumns / 2)) * spacing,
					                       static_cast<float>(row - (nrRows / 2)) * spacing,
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

void _41_PBR_Lighting_Textured::BindSphereVAO()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;

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

	bool oddRow = false;
	for (int y = 0; y <= Y_SEGMENTS; ++y)
	{
		if (!oddRow)
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.emplace_back(y * (X_SEGMENTS + 1) + x);
				indices.emplace_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.emplace_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.emplace_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	sphereIndexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.emplace_back(positions[i].x);
		data.emplace_back(positions[i].y);
		data.emplace_back(positions[i].z);

		if (!uv.empty())
		{
			data.emplace_back(uv[i].x);
			data.emplace_back(uv[i].y);
		}

		if (!normals.empty())
		{
			data.emplace_back(normals[i].x);
			data.emplace_back(normals[i].y);
			data.emplace_back(normals[i].z);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0],GL_STATIC_DRAW);

	const unsigned int stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float)));
}

void _41_PBR_Lighting_Textured::RenderSphere()
{
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
}
