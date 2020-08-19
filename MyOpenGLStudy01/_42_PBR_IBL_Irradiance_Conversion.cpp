#include "_42_PBR_IBL_Irradiance_Conversion.h"
#include "CommonBaseScript.h"
#include "Shader.h"

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

	//渲染天空盒子用
	unsigned int captureFBO;
	glGenFramebuffers(1, &captureFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

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


	pbrShader.Use();
	pbrShader.SetVec3("albedo", 0.5f, 0.0f, 0.0f);
	pbrShader.SetFloat("ao", 1.0f);

	backgroundShader.Use();
	backgroundShader.SetInt("environmentMap", 0);
}
