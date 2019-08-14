#include "_09_BasicLighting.h"
#include "CommonBaseScript.h"
#include "Shader.h"
#include "Camera.h"

int _09_BasicLighting::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//因为用的还是 一样的顶点数据 所以只用绑定,不用重新填充数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//开启深度测试 进行遮挡计算
	glEnable(GL_DEPTH_TEST);

	Shader lampShader{"09_Lighting_Lamp"};
	Shader cubeShader{"09_Lighting_Cube"};

	Camera camera = Camera();
	camera.AddMouseEvent(window);

	glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.2f));
	lampShader.Use();
	lampShader.SetMat4("model", lightModel);

	glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
	glm::mat4 cubeModel(1.0f);
	cubeModel = glm::translate(cubeModel, glm::vec3(0, 0, 0));
	cubeShader.Use();
	cubeShader.SetMat4("model", cubeModel);
	cubeShader.SetVec3("lightPos", lightPos);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lampShader.Use();
		lampShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lampShader.SetMat4("view", camera.GetViewMat4());
		lampShader.SetMat4("projection", camera.GetProjectionMat4());
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		cubeShader.Use();
		cubeShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
		cubeShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		cubeShader.SetMat4("view", camera.GetViewMat4());
		cubeShader.SetMat4("projection", camera.GetProjectionMat4());
		cubeShader.SetVec3("viewPos", camera.position);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime()*2.0f);
		lightColor.y = sin(glfwGetTime()*0.7f);
		lightColor.z = sin(glfwGetTime()*1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);//降低影响
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);//更低的影响



		cubeShader.SetVec3("material.ambient", ambientColor);
		cubeShader.SetVec3("material.diffuse", diffuseColor);
		cubeShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeShader.SetFloat("material.shininess", 32.0f);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);


	return 0;
}
