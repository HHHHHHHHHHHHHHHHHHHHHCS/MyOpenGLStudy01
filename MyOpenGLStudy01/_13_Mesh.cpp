#include "_13_Mesh.h"

#include "CommonBaseScript.h"
#include "Shader.h"
#include "Camera.h"
#include "ImageHelper.h"
#include "Model.h"

int _13_Mesh::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//因为用的还是 一样的顶点数据 所以只用绑定,不用重新填充数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//开启深度测试 进行遮挡计算
	glEnable(GL_DEPTH_TEST);

	Model model{"Models/nanosuit/nanosuit.obj"};
	Shader lampShader{"09_Lighting_Lamp"};
	Shader nanosuitShader{"13_Mesh_Nanosuit"};


	Camera camera = Camera();
	camera.AddMouseEvent(window);

	nanosuitShader.Use();
	nanosuitShader.SetMat4("model", glm::translate(glm::mat4{1}, glm::vec3{0, 0, 0}));
	nanosuitShader.SetMat4("projection", camera.GetProjectionMat4());

	nanosuitShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	nanosuitShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	nanosuitShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	nanosuitShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	for (int i = 0; i < std::size(pointLightPositions); i++)
	{
		std::string lightPath = "pointLights[" + std::to_string(i) + "]";
		nanosuitShader.SetVec3(lightPath + ".position", pointLightPositions[i]);
		nanosuitShader.SetVec3(lightPath + ".ambient", 0.05f, 0.05f, 0.05f);
		nanosuitShader.SetVec3(lightPath + ".diffuse", 0.8f, 0.8f, 0.8f);
		nanosuitShader.SetVec3(lightPath + ".specular", 1.0f, 1.0f, 1.0f);
		nanosuitShader.SetFloat(lightPath + ".constant", 1.0f);
		nanosuitShader.SetFloat(lightPath + ".linear", 0.09);
		nanosuitShader.SetFloat(lightPath + ".quadratic", 0.032);
	}


	nanosuitShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	nanosuitShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	nanosuitShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	nanosuitShader.SetFloat("spotLight.constant", 1.0f);
	nanosuitShader.SetFloat("spotLight.linear", 0.09);
	nanosuitShader.SetFloat("spotLight.quadratic", 0.032);
	nanosuitShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	nanosuitShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lampShader.Use();

		lampShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lampShader.SetMat4("view", camera.GetViewMat4());
		lampShader.SetMat4("projection", camera.GetProjectionMat4());

		glBindVertexArray(lightVAO);
		for (int i = 0; i < std::size(pointLightPositions); i++)
		{
			lampShader.SetMat4("model", glm::translate(glm::mat4{1}, pointLightPositions[i]));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		nanosuitShader.Use();
		nanosuitShader.SetMat4("view", camera.GetViewMat4());
		model.Draw(nanosuitShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);


	return 0;
}
