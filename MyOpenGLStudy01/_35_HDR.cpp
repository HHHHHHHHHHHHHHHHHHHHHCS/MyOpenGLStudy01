#include "_35_HDR.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include "ImageHelper.h"

int _35_HDR::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//hdr buffer
	//-----------------
	//hdr colorBuffer
	unsigned int colorBuffer;
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//depth buffer
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	//hdr fbo
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//最后一位0 是指定MRT 用的
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Camera
	//-----------------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//Shader
	// -------------------------
	Shader objShader("6.lighting.vs", "6.lighting.fs");
	Shader hdrShader("6.hdr.vs", "6.hdr.fs");

	glEnable(GL_DEPTH_TEST);

	//Textures
	// -------------
	unsigned int woodTexture = ImageHelper::LoadTexture("wood.png", ImageHelper::defaultPath, true);

	objShader.Use();
	objShader.SetInt("diffuseTexture", 0);
	hdrShader.Use();
	hdrShader.SetInt("hdrBuffer", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	std::vector<glm::vec3> lightPositions;
	//emplace_back 不会执行拷贝   push_back 会执行拷贝
	lightPositions.emplace_back(0.0f, 0.0f, 49.5f); // back light
	lightPositions.emplace_back(-1.4f, -1.9f, 9.0f);
	lightPositions.emplace_back(0.0f, -1.8f, 4.0f);
	lightPositions.emplace_back(0.8f, -1.7f, 6.0f);
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.emplace_back(200.0f, 200.0f, 200.0f);
	lightColors.emplace_back(0.1f, 0.0f, 0.0f);
	lightColors.emplace_back(0.0f, 0.0f, 0.2f);
	lightColors.emplace_back(0.0f, 0.1f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 viewProjection = camera.GetViewProjection();

		objShader.Use();
		objShader.SetMat4("viewProjection", viewProjection);
		//TODO:

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}
