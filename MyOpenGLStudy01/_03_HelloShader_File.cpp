#include "_03_HelloShader_File.h"
int _03_HelloShader_File::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	Shader ourShader("Shaders/03_HelloShader_File.vs", "Shaders/03_HelloShader_File.fs");


	//VBO 是一组顶点数据
	//VAO 是VBO的数组
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // 创建VAO 1是VAO长度
	glBindVertexArray(VAO); //绑定VAO


	float vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // 顶部
	};


	unsigned int VBO;
	glGenBuffers(1, &VBO); // 1是缓冲ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// 第一个对应Location
	// 第四个对应一组数据的长度
	// 第五个对应在一组内 这个数据的起始位置
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);//启用顶点属性 1


	glBindBuffer(GL_ARRAY_BUFFER, 0); //安全解除GL_ARRAY_BUFFER ,防止不小心修改
	glBindVertexArray(0); //安全解除顶点数据 ,防止不小心修改

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式


	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window); //按键关闭检测

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //调用glClear之后,整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		glClear(GL_COLOR_BUFFER_BIT); //清空缓冲颜色

		ourShader.Use();
		ourShader.SetFloat("time", glfwGetTime());

		glBindVertexArray(VAO); //虽然我们只有一个VAO,不用每次都绑定,但是这样看更加直观
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //正确释放/删除之前的分配的所有资源


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

