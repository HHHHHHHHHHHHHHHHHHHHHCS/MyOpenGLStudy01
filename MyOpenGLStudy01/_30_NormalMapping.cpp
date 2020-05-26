#include "_30_NormalMapping.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include "ImageHelper.h"

unsigned int _30_NormalMapping::quadVAO = 0;

int _30_NormalMapping::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	BindQuadVAO();

	//Camera
	//-----------------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//Shader
	//-----------------
	Shader shader("30_NormalMapping");

	//Textures
	//-----------------
	unsigned int diffuseMap = ImageHelper::LoadTexture("Images/brickwall.jpg");
	unsigned int normalMap = ImageHelper::LoadTexture("Images/brickwall_normal.jpg");

	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	//configure
	//--------------------
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glActiveTexture(0);

	shader.Use();
	shader.SetInt("diffuseMap", 0);
	shader.SetInt("normalMap", 1);
	shader.SetVec3("lightPos", lightPos);


	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderQuad();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}

void _30_NormalMapping::BindQuadVAO()
{
	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);
}

void _30_NormalMapping::RenderQuad()
{
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
