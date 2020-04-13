#include "_28_PointShadow.h"
#include "CommonBaseScript.h"
#include "Shader.h"
#include "ImageHelper.h"
#include "Camera.h"

int _28_PointShadow::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//Configure Depth Map FBO
	//-----------------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT
		             , SHADOW_WIDTH, SHADOW_HEIGHT, 0,GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Shader
	//-----------------------------
	Shader pointShadowsShader{"", ""};
	Shader depthShader{"", "", ""};

	//Textures
	//-----------------------------
	unsigned int woodTexture = ImageHelper::LoadTexture("wood.png");

	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//ShaderConfigure
	//-----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //启动面剔除 默认是背面被剔除

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	glActiveTexture(0);


	pointShadowsShader.Use();
	pointShadowsShader.SetInt("diffuseTexture", 0);
	pointShadowsShader.SetInt("depthMap", 1);

	glm::vec3 lightPos{0.0f, 0.0f, 0.0f};

	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f)
	                                        , static_cast<float>(SHADOW_WIDTH) / SHADOW_HEIGHT
	                                        , near_plane, far_plane);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;

		//clear canvs
		//--------------------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//0.create depth cubemap transformation matrices
		//----------------------------------
		//每个都按顺序注视着立方体贴图的的一个方向：右、左、上、下、近、远：
		//--这里的up虽然很奇怪 但是无所谓
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f)
			              , glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f)
			              , glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f)
			              , glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f)
			              , glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f)
			              , glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj
			* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f)
			              , glm::vec3(0.0f, -1.0f, 0.0f)));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
