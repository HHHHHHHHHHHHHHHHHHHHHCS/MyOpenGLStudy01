#include "_22_GeometryShader.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include <string>
#include "stb_image.h"
#include "ImageHelper.h"
#include "Model.h"


int _22_GeometryShader::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float points[] = {
		//position  normal
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glBindVertexArray(0);

	Model model{"Models/nanosuit/nanosuit.obj"};

	Shader pointShader{"22_GeometryShader_Point", true};
	Shader explodeShader{"22_GeometryShader_Explode", true};
	Shader normalShader{"22_GeometryShader_Normal", true};


	Camera camera{};
	camera.AddMouseEvent(window);

	glEnable(GL_DEPTH_TEST);

	//CommonBaseScript::UsePolygonMode();

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// pointShader.Use();
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_POINTS, 0, 4);

		// explodeShader.Use();
		// explodeShader.SetFloat("time", glfwGetTime());
		// explodeShader.SetMat4("view", camera.GetViewMat4());
		// explodeShader.SetMat4("projection", camera.GetProjectionMat4());
		// explodeShader.SetMat4("model", glm::mat4{ 1 });
		// model.Draw(explodeShader);

		explodeShader.Use();
		explodeShader.SetFloat("time", -3.14 / 2);
		explodeShader.SetMat4("view", camera.GetViewMat4());
		explodeShader.SetMat4("projection", camera.GetProjectionMat4());
		explodeShader.SetMat4("model", glm::mat4{1});
		model.Draw(explodeShader);
		normalShader.Use();
		normalShader.SetMat4("view", camera.GetViewMat4());
		normalShader.SetMat4("projection", camera.GetProjectionMat4());
		normalShader.SetMat4("model", glm::mat4{1});
		model.Draw(normalShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}
