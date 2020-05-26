#include "_29_CSM.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include "ImageHelper.h"

unsigned int _29_CSM::planeVAO = 0;
unsigned int _29_CSM::cubeVAO = 0;
unsigned int _29_CSM::quadVAO = 0;

int _29_CSM::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	BindPlaneVAO();
	BindCubeVAO();
	BindQuadVAO();

	//Frame
	//-------------
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT
	             , 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Camera
	//-----------------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	//LightSpaceMatrix
	GLfloat near_plane = 0.1f;
	float csmOrth[]{2, 4, 6, 8};
	float csmDistances[]{10, 20, 30, 40};
	glm::vec3 lightPos = glm::vec3(0.0f, 6, -7);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f)
	                                  , glm::vec3(0.0f, 1.0f, 0.0f));

	float oSize = csmOrth[0];
	glm::mat4 lightProjection0 = glm::ortho(-oSize, oSize, -oSize, oSize, near_plane, csmDistances[0]);
	glm::mat4 lightSpaceMatrix0 = lightProjection0 * lightView;

	oSize = csmOrth[1];
	glm::mat4 lightProjection1 = glm::ortho(-oSize, oSize, -oSize, oSize, near_plane, csmDistances[1]);
	glm::mat4 lightSpaceMatrix1 = lightProjection1 * lightView;

	oSize = csmOrth[2];
	glm::mat4 lightProjection2 = glm::ortho(-oSize, oSize, -oSize, oSize, near_plane, csmDistances[2]);
	glm::mat4 lightSpaceMatrix2 = lightProjection2 * lightView;

	oSize = csmOrth[3];
	glm::mat4 lightProjection3 = glm::ortho(-oSize, oSize, -oSize, oSize, near_plane, csmDistances[3]);
	glm::mat4 lightSpaceMatrix3 = lightProjection3 * lightView;


	//load textures
	//--------------
	unsigned int woodTexture = ImageHelper::LoadTexture("wood.png");

	//Shader
	//------------------------
	Shader simpleDepthShader{"29_CSMDepthShader"};
	Shader debugDepthQuadShader{"27_DebugDepthQuad"};
	Shader objShader{"29_CSMShadow_Soft"};

	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(0);

	debugDepthQuadShader.Use();
	debugDepthQuadShader.SetInt("depthMap", 1);

	objShader.Use();
	objShader.SetInt("diffuseTexture", 0);
	objShader.SetInt("shadowMap", 1);
	objShader.SetVec3("lightPos", glm::normalize(lightPos));
	objShader.SetVec4("lightSqrDist"
	                  , glm::vec4(csmOrth[0] * csmOrth[0], csmOrth[1] * csmOrth[1]
	                              , csmOrth[2] * csmOrth[2], csmOrth[3] * csmOrth[3]));

	glm::mat4 offsetM4 = glm::mat4{1};
	//[-1,1] 缩放到[0,1]
	offsetM4 = glm::scale(offsetM4, glm::vec3(0.5, 0.5, 0.5));
	offsetM4 = glm::translate(offsetM4, glm::vec3(1.0, 1.0, 1.0));

	objShader.SetMat4("lightSpaceMatrix[0]", offsetM4 * lightSpaceMatrix0);
	objShader.SetMat4("lightSpaceMatrix[1]", offsetM4 * lightSpaceMatrix1);
	objShader.SetMat4("lightSpaceMatrix[2]", offsetM4 * lightSpaceMatrix2);
	objShader.SetMat4("lightSpaceMatrix[3]", offsetM4 * lightSpaceMatrix3);

	int halfShadowWidth = SHADOW_WIDTH / 2.0;
	int halfShadowHeight = SHADOW_HEIGHT / 2.0;

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);
		simpleDepthShader.Use();

		glEnable(GL_SCISSOR_TEST); // 启用剪裁框

		glScissor(4, 4, halfShadowWidth - 8, halfShadowHeight - 8); //裁剪框
		glViewport(0, 0, halfShadowWidth, halfShadowHeight);
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix0);
		RenderScene(simpleDepthShader);

		glScissor(halfShadowWidth + 4, 4, halfShadowWidth - 8, halfShadowHeight - 8); //裁剪框
		glViewport(halfShadowWidth, 0, halfShadowWidth, halfShadowHeight);
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix1);
		RenderScene(simpleDepthShader);

		glScissor(4, halfShadowHeight + 4, halfShadowWidth - 8, halfShadowHeight - 8); //裁剪框
		glViewport(0, halfShadowHeight, halfShadowWidth, halfShadowHeight);
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix2);
		RenderScene(simpleDepthShader);

		glScissor(halfShadowWidth + 4, halfShadowHeight + 4, halfShadowWidth - 8, halfShadowHeight - 8); //裁剪框
		glViewport(halfShadowWidth, halfShadowHeight, halfShadowWidth, halfShadowHeight);
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix3);
		RenderScene(simpleDepthShader);

		glDisable(GL_SCISSOR_TEST); // 禁用剪裁测试

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		objShader.Use();
		objShader.SetMat4("viewProjection"
		                  , camera.GetProjectionMat4() * camera.GetViewMat4());
		objShader.SetVec3("viewPos", camera.position);
		RenderScene(objShader);

		// debugDepthQuadShader.Use();
		// glBindVertexArray(quadVAO);
		// //GL_TRIANGLE_STRIP是将顶点传递给opengl渲染管道线（pipeline）
		// //进行进一步处理的方式（创建几何图形）
		// glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(simpleDepthShader.ID);
	glDeleteShader(debugDepthQuadShader.ID);
	glDeleteShader(objShader.ID);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &planeVAO);


	glfwTerminate();

	return 0;
}

void _29_CSM::BindPlaneVAO()
{
	//plane
	//---------------------
	float planeVertices[] =
	{
		// positions				// normals			// texcoords
		100.0f, -0.5f, 100.0f, 0.0f, 1.0f, 0.0f, 100.0f, 0.0f,
		-100.0f, -0.5f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f,

		100.0f, -0.5f, 100.0f, 0.0f, 1.0f, 0.0f, 100.0f, 0.0f,
		-100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f,
		100.0f, -0.5f, -100.0f, 0.0f, 1.0f, 0.0f, 100.0f, 100.0f
	};
	glGenVertexArrays(1, &planeVAO);
	unsigned int planeVBO;
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &planeVBO); //这时候VBO已经写入VAO了 可以删除VBO了
}

void _29_CSM::BindCubeVAO()
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
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &cubeVBO); //这时候VBO已经写入VAO了 可以删除VBO了
	//delete[] vertices;//自动销毁的不用手动调用
}

void _29_CSM::BindQuadVAO()
{
	float quadVertices[] =
	{
		// positions        // texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &quadVAO);
	unsigned int quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &quadVBO); //这时候VBO已经写入VAO了 可以删除VBO了
}

void _29_CSM::RenderScene(const Shader& shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// cubes
	for (int i = -10; i < 100; ++i)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, i));
		model = glm::rotate(model, glm::radians(i * 15.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3((i % 4) * 0.25));
		shader.SetMat4("model", model);
		RenderCube();
	}
}

void _29_CSM::RenderCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
