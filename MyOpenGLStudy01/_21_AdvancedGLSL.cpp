#include "_21_AdvancedGLSL.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include <string>
#include "stb_image.h"
#include "ImageHelper.h"


int _21_AdvancedGLSL::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	//cube positions
	float cubePositionsVertices[] = {
		// positions
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
	};

	//cube texture Coords
	float cubeUVVertices[] = {
		// texture Coords
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	//数据指针拷贝
	// static const float data[] =
	// {
	// 0.25, -0.25, 0.5, 1.0,
	// -0.25, -0.25, 0.5, 1.0,
	// 0.25, 0.25, 0.5, 1.0
	// };
	// // 获取缓冲区的映射指针ptr
	// void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// // 拷贝我们的数据到指针所指向的位置
	// memcpy(ptr, data, sizeof(data));
	// // 使用完之后释放映射的指针
	// glUnmapBuffer(GL_ARRAY_BUFFER);

	//数据缓冲拷贝
	// float vertexData[] = { ... };
	// // 绑定一个顶点数据 但是只能有一个
	// glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	// // 我们不能同时将两个缓冲绑定到同一个缓冲目标上。
	// // 因此OpenGL另外两个缓冲目标，叫做GL_COPY_READ_BUFFER和GL_COPY_WRITE_BUFFER。
	// glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
	// glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
	// glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));


	//cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//申请开辟空间  但是先不填充数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositionsVertices) + sizeof(cubeUVVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubePositionsVertices), &cubePositionsVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositionsVertices), sizeof(cubeUVVertices), &cubeUVVertices);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(sizeof(cubePositionsVertices)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Load Textures
	unsigned int cubeTexture = ImageHelper::LoadTexture("marble.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);


	Camera camera{};
	camera.AddMouseEvent(window);

	// 深度测试
	glEnable(GL_DEPTH_TEST);
	// gl_PointSize用
	glEnable(GL_PROGRAM_POINT_SIZE);


	Shader redShader{ "21_AdvancedGLSL_Red" };
	Shader greenShader{ "21_AdvancedGLSL_Green" };
	Shader blueShader{"21_AdvancedGLSL_Blue"};
	Shader whiteShader{ "21_AdvancedGLSL_White" };

	unsigned int uboVPBlock;
	glGenBuffers(1, &uboVPBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboVPBlock);
	//内存分配里面有计算方法
	//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/08%20Advanced%20GLSL/
	//先不填充数据  后面填充
	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW);//分配128字节的内存 128+128
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	unsigned int VPIndex = glGetUniformBlockIndex(blueShader.ID, "VPMatrices");
	//VP最后的参数是申请的索引 0
	glUniformBlockBinding(blueShader.ID, VPIndex, 0);

	//第二个参数是申请的索引
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboVPBlock);
	//或者 0 起始位置  128 长度 
	//通过使用glBindBufferRange函数，你可以让多个不同的Uniform块绑定到同一个Uniform缓冲对象上。
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboVPBlock, 0, 128);


	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindBuffer(GL_UNIFORM_BUFFER, uboVPBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera.GetViewMat4());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camera.GetProjectionMat4());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(cubeVAO);
		glm::mat4 model{ 1 };

		redShader.Use();
		model = glm::mat4{ 1 };
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
		redShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		greenShader.Use();
		model = glm::mat4{ 1 };
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
		redShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		blueShader.Use();
		model = glm::mat4{1};
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		blueShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		whiteShader.Use();
		model = glm::mat4{ 1 };
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f));
		whiteShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();

	return 0;
}
