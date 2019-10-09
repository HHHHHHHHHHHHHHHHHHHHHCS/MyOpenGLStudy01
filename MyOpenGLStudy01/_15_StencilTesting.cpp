#include "_15_StencilTesting.h"
#include "CommonBaseScript.h"
#include "ImageHelper.h"
#include "Camera.h"


int _15_StencilTesting::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	float cubeVertices[] = {
		// 位置          // UV
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	float planeVertices[] = {
		// 位置          // UV (注意我们的图片模式是否是REPEAT模式 这时候UV才能高于1)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	//Cube
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glBindVertexArray(0);
	//Plane
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glBindVertexArray(0);

	//Load Textures
	unsigned int cubeTexture = ImageHelper::LoadTexture("marble.jpg");
	unsigned int floorTexture = ImageHelper::LoadTexture("metal.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	// 深度测试
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// 模版测试
	// -----------------------------
	glEnable(GL_STENCIL_TEST);
	//第一个参数比较的运算符   第二个参数参考值   
	//第三个参数 设置一个掩码，它将会与参考值和储存的模板值在测试比较它们之前进行与(AND)运算。初始情况下所有位都为1。
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//三个参数对应的状态 fail equal  pass
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Camera camera = Camera();
	camera.AddMouseEvent(window);

	Shader objShader{"14_DepthTesting_Object"};
	Shader outlineShader{"15_DepthTesting_Outline"};

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		objShader.Use();
		glm::mat4 model{1};

		objShader.SetMat4("view", camera.GetViewMat4());
		objShader.SetMat4("projection", camera.GetProjectionMat4());

		// 正常绘制地板，但不要将地板写入模板缓冲区，我们只关心容器。我们将其掩码设置为0x00以不写入模具缓冲区。
		glStencilMask(0x00);
		//floor
		glBindVertexArray(planeVAO);
		objShader.SetInt("texture1", 1);
		model = glm::mat4{1.0f};
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 1.渲染过程，正常绘制对象，写入模具缓冲区
		// --------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		objShader.SetInt("texture1", 0);
		glBindVertexArray(cubeVAO);
		model = glm::mat4{1.0f};
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, -1.0f));
		objShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4{1.0f};
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
		objShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// 第二。渲染过程：现在绘制稍微缩放的对象版本，这一次禁用模具写入。
		//因为模具缓冲区现在填充了几个1。缓冲区的1部分不会被绘制，所以只绘制
		//对象的大小差异，使其看起来像边框。
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		outlineShader.Use();

		glm::vec3 scale = glm::vec3(1.1f, 1.1f, 1.1f);
		outlineShader.SetMat4("view", camera.GetViewMat4());
		outlineShader.SetMat4("projection", camera.GetProjectionMat4());

		glBindVertexArray(cubeVAO);
		model = glm::mat4{1.0f};
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, -1.0f));
		model = glm::scale(model, scale);
		outlineShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4{1.0f};
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
		model = glm::scale(model, scale);
		outlineShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVAO);


	return 0;
}
