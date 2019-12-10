#include "_21_AdvancedGLSL.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include <string>
#include "stb_image.h"
#include "ImageHelper.h"

//可以开始绑定
//glMapBuffer
//指针将会不再可用
//glUnmapBuffer(GL_ARRAY_BUFFER);

//数据拷贝过去
//glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
//glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
//glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));

#include "_20_Cubemaps.h"


int _20_Cubemaps::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}
	//cube
	//逆时针
	float cubeVertices[] = {
		//Pos   Normal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//skybox
	float skyboxVertices[] = {
		// positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	std::vector<std::string> facesPath
	{
		"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg",
		"skybox/back.jpg"

	};

	//cube
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glBindVertexArray(0);

	//skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));


	const unsigned int cubemapTexture = ImageHelper::LoadCubemap(facesPath);
	const unsigned int cubeTexture = ImageHelper::LoadTexture_Filp("02.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	//重新设置回0 免得乱改了
	glActiveTexture(GL_TEXTURE);

	Shader skyboxShader = { "20_Cubemaps_Skybox" };
	Shader reflectionShader{ "20_Cubemaps_Reflection" };
	Shader refractionShader{ "20_Cubemaps_Refraction" };


	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0);

	reflectionShader.Use();
	reflectionShader.SetInt("skybox", 0);

	refractionShader.Use();
	refractionShader.SetInt("skybox", 0);

	Camera camera{};
	camera.AddMouseEvent(window);

	glEnable(GL_DEPTH_TEST);
	//这里的顶点没有按照时针顺摆放  所以不开剔除
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//reflection
		reflectionShader.Use();
		glBindVertexArray(cubeVAO);

		glm::mat4 model;
		reflectionShader.SetMat4("view", camera.GetViewMat4());
		reflectionShader.SetMat4("projection", camera.GetProjectionMat4());
		reflectionShader.SetVec3("cameraPos", camera.position);

		model = glm::translate(glm::mat4{ 1 }, glm::vec3(-1.0f, 0.0f, -1.0));
		reflectionShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//refraction
		refractionShader.Use();
		glBindVertexArray(cubeVAO);

		refractionShader.SetMat4("view", camera.GetViewMat4());
		refractionShader.SetMat4("projection", camera.GetProjectionMat4());
		refractionShader.SetVec3("cameraPos", camera.position);

		model = glm::translate(glm::mat4{ 1 }, glm::vec3(2.0f, 0.0f, 0));
		refractionShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//深度缓冲默认是 1.0  所以 需要 <= 1.0 写入天空盒
		//不能是 LESS(<1.0)
		glDepthFunc(GL_LEQUAL);
		//深度遮罩 不写入深度
		glDepthMask(GL_FALSE);

		skyboxShader.Use();
		//mat3->mat4 去除掉了 位移 
		skyboxShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMat4())));
		skyboxShader.SetMat4("projection", camera.GetProjectionMat4());
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteTextures(1, &cubemapTexture);
	glDeleteTextures(1, &cubeTexture);


	glfwTerminate();

	return 0;
}
