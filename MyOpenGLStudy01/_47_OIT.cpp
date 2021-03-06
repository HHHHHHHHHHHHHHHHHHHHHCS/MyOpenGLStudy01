﻿#include "_47_OIT.h"

#include "Camera.h"
#include "CommonBaseScript.h"


int _47_OIT::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//Shader
	//----------
	Shader solidShader("47_Solid");
	Shader transparentShader("47_Transparent");
	Shader compositeShader("47_Composite");
	Shader screenShader("47_Screen");

	float quadVertices[] = {
		// positions		// uv
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};


	//Quad VAO
	//----------
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glBindVertexArray(0);

	//Setup attachments
	//----------
	unsigned int opaqueFBO, transparentFBO;
	glGenFramebuffers(1, &opaqueFBO);
	glGenFramebuffers(1, &transparentFBO);

	unsigned int opaqueTexture;
	glGenTextures(1, &opaqueTexture);
	glBindTexture(GL_TEXTURE_2D, opaqueTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Opaque framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int accumTexture;
	glGenTextures(1, &accumTexture);
	glBindTexture(GL_TEXTURE_2D, accumTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int revealTexture;
	glGenTextures(1, &revealTexture);
	glBindTexture(GL_TEXTURE_2D, revealTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0); //use opaque FBO depth

	const GLenum transparentDrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, transparentDrawBuffers);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!" << std::endl;
	}

	//Camera
	//-----------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	// set up transformation matrices
	// ------------------------------------------------------------------
	glm::mat4 whiteModelMat = CalculateModelMatrix(glm::vec3(0.0f, 0.0f, -1.0f));

	glm::mat4 yellowModelMat = CalculateModelMatrix(glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 redModelMat = CalculateModelMatrix(glm::vec3(0.0f, -0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	                                             glm::vec3(1.0f, 0.1f, 1.0f));
	glm::mat4 greenModelMat = CalculateModelMatrix(glm::vec3(-0.25f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 45.0f),
	                                               glm::vec3(1.0f, 0.1f, 1.0f));
	glm::mat4 blueModelMat = CalculateModelMatrix(glm::vec3(0.25f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, -45.0f),
	                                              glm::vec3(1.0f, 0.1f, 1.0f));

	// set up intermediate variables
	// ------------------------------------------------------------------
	glm::vec4 zeroFillerVec(0.0f);
	glm::vec4 oneFillerVec(1.0f);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		glm::mat4 viewProjection = camera.GetViewProjection();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use solid shader
		//-------------------
		solidShader.Use();

		//draw white 
		solidShader.SetMat4("MVP", viewProjection * whiteModelMat);
		solidShader.SetVec3("Color", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//draw white 
		solidShader.SetMat4("MVP", viewProjection * yellowModelMat);
		solidShader.SetVec3("Color", glm::vec3(1.0f, 1.0f, 0.0f));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//--------------------
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		//glBlendFunci  i 可以指定attach
		glBlendFunci(0, GL_ONE, GL_ONE);
		glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);

		glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
		//glClearBuffer  可以指定清除哪种缓存 和attach
		glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);

		transparentShader.Use();

		//draw red 
		transparentShader.SetMat4("MVP", viewProjection * redModelMat);
		transparentShader.SetVec4("Color", glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//draw green
		transparentShader.SetMat4("MVP", viewProjection * greenModelMat);
		transparentShader.SetVec4("Color", glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//draw blue
		transparentShader.SetMat4("MVP", viewProjection * blueModelMat);
		transparentShader.SetVec4("Color", glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//draw composite image
		//----------------------------

		glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);

		compositeShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, accumTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, revealTexture);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//draw to backbuffer(final pass)
		//----------------------------
		glDisable(GL_DEPTH);
		glDepthMask(GL_TRUE); //enable depth writes 
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		screenShader.Use();
		screenShader.SetInt("screen", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, opaqueTexture);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteTextures(1, &opaqueTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteTextures(1, &accumTexture);
	glDeleteTextures(1, &revealTexture);
	glDeleteFramebuffers(1, &opaqueFBO);
	glDeleteFramebuffers(1, &transparentFBO);

	glfwTerminate();

	return 0;
}

glm::mat4 _47_OIT::CalculateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position);
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, scale);

	return trans;
}
