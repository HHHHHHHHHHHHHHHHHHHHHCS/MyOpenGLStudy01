#include "_23_Instancing_Practice.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CommonBaseScript.h"
#include "Camera.h"
#include "Model.h"

int _23_Instancing_Practice::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	Camera camera{};
	camera.AddMouseEvent(window);

	Model planet{ "Models/PlanetRock/planet.obj" };
	Model rock{ "Models/PlanetRock/rock.obj" };

	Shader planetRockShader{ "23_Instancing_PlanetRock" };







	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}
