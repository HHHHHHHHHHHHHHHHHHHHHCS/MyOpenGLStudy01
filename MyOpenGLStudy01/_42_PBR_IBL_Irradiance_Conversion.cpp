#include "_42_PBR_IBL_Irradiance_Conversion.h"
#include "CommonBaseScript.h"
#include "Shader.h"
#include "ImageHelper.h"
#include <ext/matrix_clip_space.inl>
#include <detail/func_trigonometric.inl>
#include <ext/matrix_transform.inl>
#include "Camera.h"

unsigned int _42_PBR_IBL_Irradiance_Conversion::cubeVAO = 0;

int _42_PBR_IBL_Irradiance_Conversion::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); //天空盒深度是1  小于等于1 的被渲染

	Shader pbrShader{""};
	Shader equirectangulareToCubemapShader{""};
	Shader backgroundShader{""};


	//hdr cubemap
	unsigned int hdrTexture = ImageHelper::LoadHDR_Filp("hdr/newport_loft.hdr");
	unsigned int envCubemap = ImageHelper::BindHDRCubemap();


	//模拟朝向天空盒 
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};


	//渲染天空盒子用
	unsigned int captureFBO;
	glGenFramebuffers(1, &captureFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


	//渲染天空盒
	//-------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	equirectangulareToCubemapShader.Use();
	equirectangulareToCubemapShader.SetInt("equirectangularMap", 0);
	equirectangulareToCubemapShader.SetMat4("projection", captureProjection);


	glViewport(0, 0, 512, 512); //不要忘记设置viewport
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangulareToCubemapShader.SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// lights
	// ----------------
	glm::vec3 lightPositions[] = {
		glm::vec3{-10.0f, 10.0f, 10.0f},
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;


	Camera camera{};

	glm::mat4 projection = camera.GetProjectionMat4();

	pbrShader.Use();
	pbrShader.SetVec3("albedo", 0.5f, 0.0f, 0.0f);
	pbrShader.SetFloat("ao", 1.0f);
	pbrShader.SetMat4("projection", projection);

	backgroundShader.Use();
	backgroundShader.SetInt("environmentMap", 0);
	backgroundShader.SetMat4("projection", projection);

	//变成原来viewport
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	while(!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void _42_PBR_IBL_Irradiance_Conversion::BindCubeVAO()
{
	
}

void _42_PBR_IBL_Irradiance_Conversion::RenderCube()
{
	
}

