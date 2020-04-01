#include "_23_Instancing_Practice.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CommonBaseScript.h"
#include "Camera.h"
#include "Model.h"

int _23_Instancing_Practice::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	Camera camera{};
	camera.AddMouseEvent(window);

	Model planet{"Models/PlanetRock/planet.obj"};
	Model asteroid{"Models/PlanetRock/rock.obj"};

	Shader planetShader{"23_Instancing_Planet"};
	Shader asteroidShader{"23_Instancing_Asteroid"};

	unsigned int amount = 100000;
	glm::mat4* modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // 初始化随机种子
	float radius = 150.0f;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;

		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		model = glm::translate(model, glm::vec3(x, y, z));


		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;


		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngle = rand() % 360;
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	std::vector<Mesh> meshes = asteroid.GetMeshes();

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		unsigned int VAO = meshes[i].VAO;
		glBindVertexArray(VAO);

		//set attribute pointers for matrix4x4 (4 * vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4)
		                      , reinterpret_cast<void*>(0));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4)
		                      , reinterpret_cast<void*>(sizeof(glm::vec4)));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
		                      reinterpret_cast<void*>(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
		                      reinterpret_cast<void*>(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	//因为planet的贴图已经是0了 这里为了避免冲突
	//所以重新绑定给一个 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, asteroid.GetTextures()[0]->id);
	asteroidShader.Use();
	asteroidShader.SetInt("texture_diffuse1", 1);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		camera.DoKeyboardMove(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMat4();
		glm::mat4 projection = camera.GetProjectionMat4();

		planetShader.Use();
		planetShader.SetMat4("view", view);
		planetShader.SetMat4("projection", projection);
		glm::mat4 model = glm::mat4{1.0f};
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		planetShader.SetMat4("model", model);
		planet.Draw(planetShader);



		asteroidShader.Use();
		asteroidShader.SetMat4("view", view);
		asteroidShader.SetMat4("projection", projection);
		for (unsigned int i = 0; i < asteroid.GetMeshes().size(); i++)
		{
			glBindVertexArray(asteroid.GetMeshes()[i].VAO);
			//glDrawElementsInstanced 走indices
			// reinterpret_cast<void*>(0) 是indices 起始偏移
			glDrawElementsInstanced(GL_TRIANGLES, asteroid.GetMeshes()[i].indices.size(), GL_UNSIGNED_INT,
			                        reinterpret_cast<void*>(0), amount);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	delete[] modelMatrices;

	return 0;
}
