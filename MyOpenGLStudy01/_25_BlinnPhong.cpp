#include "_25_BlinnPhong.h"

#include "CommonBaseScript.h"
#include "Shader.h"
#include "Camera.h"
#include "ImageHelper.h"

int _25_BlinnPhong::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float planeVertices[] =
	{
		// positions            // normals         // texcoords
		10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
		-10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

		10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
		-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
		10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
	};

	//plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glBindVertexArray(0);

	// load textures
	unsigned int floorTexture = ImageHelper::LoadTexture("wood.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glActiveTexture(0);


	Shader floorShader{"25_BlinnPhong_Floor"};
	// shader configuration
	floorShader.Use();
	floorShader.SetInt("floorTexture", 0);

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera = Camera();
	camera.AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	// lighting info
	glm::vec3 lightPos(0.0f, 2.0f, 0.0f);
	floorShader.Use();
	floorShader.SetVec3("lightPos", lightPos);

	bool blinn = true;
	bool lastPress = false;

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);


		if (lastPress && !CommonBaseScript::keys[GLFW_KEY_B])
		{
			blinn = !blinn;
		}
		lastPress = CommonBaseScript::keys[GLFW_KEY_B];


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//floor
		floorShader.Use();
		glm::mat4 model{1.0f};
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		floorShader.SetMat4("NormalMatrix", glm::transpose(glm::inverse(model)));
		floorShader.SetMat4("Model", model);
		floorShader.SetMat4("MVP", camera.GetProjectionMat4() * camera.GetViewMat4() * model);
		floorShader.SetVec3("viewPos", camera.position);
		floorShader.SetBool("blinn", blinn);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteTextures(1, &floorTexture);

	return 0;
}
