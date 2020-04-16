#include "_28_PointShadow.h"
#include "CommonBaseScript.h"
#include "Shader.h"
#include "ImageHelper.h"
#include "Camera.h"

unsigned int _28_PointShadow::cubeVAO = 0;


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

	//Camera
	//-----------------------------
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


	glm::vec3 lightPos{0.0f, 0.0f, 0.0f};

	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f)
	                                        , static_cast<float>(SHADOW_WIDTH) / SHADOW_HEIGHT
	                                        , near_plane, far_plane);

	pointShadowsShader.Use();
	pointShadowsShader.SetInt("diffuseTexture", 0);
	pointShadowsShader.SetInt("depthMap", 1);
	pointShadowsShader.SetVec3("lightPos", lightPos);
	pointShadowsShader.SetFloat("far_plane", far_plane);

	depthShader.Use();
	depthShader.SetFloat("far_plane", far_plane);
	depthShader.SetVec3("lightPos", lightPos);

	bool showShader = true;
	bool lastPress = false;
	std::cout << (showShader ? "Show Shader" : "Close Shadow") << '\n';

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		if (lastPress && !CommonBaseScript::keys[GLFW_KEY_SPACE])
		{
			showShader = !showShader;
			std::cout << (showShader ? "Show Shader" : "Close Shadow") << '\n';
		}
		lastPress = CommonBaseScript::keys[GLFW_KEY_SPACE];

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

		//1.render scene to depth cubemap
		//-------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); //切换到ShadowMap
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.Use();
		for (unsigned int i = 0; i < 6; ++i)
		{
			depthShader.SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		}
		RenderScene(depthShader);

		//2.render scene as normal
		//----------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //切换为默认的RT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pointShadowsShader.Use();
		glm::mat4 proj = camera.GetProjectionMat4();
		glm::mat4 view = camera.GetViewMat4();
		pointShadowsShader.SetMat4("projection", proj);
		pointShadowsShader.SetMat4("view", view);
		pointShadowsShader.SetVec3("viewPos", camera.position);
		pointShadowsShader.SetInt("shadows", true); //todo:off/on
		RenderScene(depthShader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(pointShadowsShader.ID);
	glDeleteShader(depthShader.ID);
	glDeleteVertexArrays(1, &cubeVAO);

	glfwTerminate();

	return 0;
}

void _28_PointShadow::RenderScene(Shader shader)
{
	//room
	glm::mat4 model = glm::mat4{1.0f};
	model = glm::scale(model, glm::vec3{5.0f});
	shader.SetMat4("model", model);
	glDisable(GL_CULL_FACE); //因为我们在一个大盒子里面所以先禁用
	shader.SetInt("reverse_normals", 1); //因为我们在盒子里面normal需要反着的
	RenderCube();
	shader.SetInt("reverse_normals", 0);
	glEnable(GL_CULL_FACE);

	//cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetMat4("model", model);
	RenderCube();
}

void _28_PointShadow::BindCubeVAO()
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
	};

	glGenVertexArrays(1, &cubeVAO);
	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &cubeVBO); //这时候VBO已经写入VAO了 可以删除VBO了
	//delete[] vertices;//末尾自动删除不用手动调用
}

void _28_PointShadow::RenderCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
