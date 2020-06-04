#include "_31_MeshNormal.h"
#include "CommonBaseScript.h"
#include "Camera.h"

int _31_MeshNormal::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//Camera
	//-----------------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//Shader
	//-----------------
	Shader shader("30_NormalMapping");

	//Model
	//-----------------


	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	//configure
	//--------------------
	glEnable(GL_DEPTH_TEST);


	shader.Use();
	shader.SetVec3("lightPos", lightPos);

	bool haveNormal = true;
	shader.SetBool("haveNormal", haveNormal);
	std::cout << (haveNormal ? "Have Normal" : "No Normal") << '\n';

	bool inVSTBN = true;
	shader.SetBool("inVSTBN", inVSTBN);
	std::cout << (inVSTBN ? "in vs TBN" : "in fs TBN") << '\n';

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		if (CommonBaseScript::clickKeys[GLFW_KEY_N])
		{
			haveNormal = !haveNormal;
			shader.SetBool("haveNormal", haveNormal);
			std::cout << (haveNormal ? "Have Normal" : "No Normal") << '\n';
		}

		if (CommonBaseScript::clickKeys[GLFW_KEY_B])
		{
			inVSTBN = !inVSTBN;
			shader.SetBool("inVSTBN", inVSTBN);
			std::cout << (inVSTBN ? "in vs TBN" : "in fs TBN") << '\n';
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetMat4("model", model);
		shader.SetMat4("viewProjection", camera.GetViewProjection());
		shader.SetVec3("viewPos", camera.position);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}
