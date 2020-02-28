#include "_26_GammaCorrection.h"

#include "CommonBaseScript.h"
#include "Shader.h"
#include "Camera.h"
#include "ImageHelper.h"

int _26_GammaCorrection::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}
	float planeVertices[] = {
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

	//textures
	unsigned int linearFloorTexture = ImageHelper::LoadTexture("red.png", ImageHelper::defaultPath, false);
	unsigned int gammaFloorTexture = ImageHelper::LoadTexture("red.png", ImageHelper::defaultPath, true);

	//shader
	Shader floorShader{"26_GammaCorrection"};
	floorShader.Use();
	floorShader.SetInt("floorTexture", 0);

	glm::vec3 lightPositions[] =
	{
		glm::vec3(-3.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	};
	glm::vec3 lightColors[] =
	{
		glm::vec3(0.25),
		glm::vec3(0.50),
		glm::vec3(0.75),
		glm::vec3(1.00)
	};

	//gamma校正将把线性颜色空间转变为非线性空间

	//SRGB 把 颜色 pow(2.2)
	//即在Gamma空间制作的纹理，经过Gamma校正->pow(1/2.2)后存储的纹理。
	//如 大脑以为的0.5颜色 在gamma以pow(1/2.2)->0.72储存
	//现在勾选srgb读取 pow(2.2) 变回大脑以为的0.5 在shader中计算


	//这个可以告诉帧(包括默认帧数) 当前的帧数是SRGB
	//在最后渲染之前 把颜色 pow(1/2.2) 变回线性
	//glEnable(GL_FRAMEBUFFER_SRGB);

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera = Camera();
	camera.AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	glm::mat4 model{1.0f};

	bool gammaCorrection = true;
	bool lastPress = false;
	std::cout << (gammaCorrection ? "gamma" : "linear") << '\n';

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		if (lastPress && !CommonBaseScript::keys[GLFW_KEY_G])
		{
			gammaCorrection = !gammaCorrection;
			std::cout << (gammaCorrection ? "gamma" : "linear") << '\n';
		}
		lastPress = CommonBaseScript::keys[GLFW_KEY_G];

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		floorShader.Use();
		auto MVP = camera.GetProjectionMat4() * camera.GetViewMat4() * model;
		floorShader.SetMat4("Model", model);
		floorShader.SetMat4("NormalMatrix", glm::transpose(glm::inverse(model)));
		floorShader.SetMat4("MVP", MVP);
		floorShader.SetVec3Array("lightPositions", 4, &lightPositions[0][0]);
		floorShader.SetVec3Array("lightColors", 4, &lightColors[0][0]);
		floorShader.SetVec3("cameraPos", camera.position);
		floorShader.SetBool("gammaCorrection", gammaCorrection);

		glActiveTexture(GL_TEXTURE0);
		//传入shader 的颜色都是linear 的
		//如果勾选的是gamma 则输出的颜色是gamma空间的否则是线性的
		glBindTexture(GL_TEXTURE_2D, gammaCorrection ? gammaFloorTexture : linearFloorTexture);
		glActiveTexture(0);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteTextures(1, &gammaFloorTexture);
	glDeleteTextures(1, &gammaFloorTexture);

	return 0;
}
