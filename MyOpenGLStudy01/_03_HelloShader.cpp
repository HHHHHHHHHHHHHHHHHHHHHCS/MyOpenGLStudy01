#include "_03_HelloShader.h"

int _03_HelloShader::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	SupportVertexAttr();


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource{
		R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
		layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
		
		out vec4 vertexColor;

		void main()
		{
			gl_Position = vec4(aPos.xyz, 1.0);
			vertexColor= vec4(aColor.xyz,1.0);
		}
		)"
	};
	glad_glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	CommonBaseScript::CheckCompile(vertexShader);


	const char* fragmentShaderSource{
		R"(
		#version 460 core
		out vec4 FragColor;

		in vec4 vertexColor;
		//uniform是全局的 数据会一直保存 并且在program 任意阶段访问
		//如果你声明了一个uniform却在GLSL代码中没用过，编译器会静默移除这个变量，导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误
		uniform vec4 ourColor;

		void main()
		{
		    FragColor = vertexColor * ourColor;
		} 
		)"
	};
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	CommonBaseScript::CheckCompile(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CommonBaseScript::CheckCompile(shaderProgram);

	glDeleteShader(vertexShader); //删除已经没有用的顶点着色器对象
	glDeleteShader(fragmentShader); //删除已经没有用的片元着色器对象


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

		glUseProgram(shaderProgram); //使用这个程序对象

		float timeValue = glfwGetTime();
		float value = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //glGetUniformLocation == -1就是没有找到
		glUniform4f(vertexColorLocation, value, value, value, 1.0f);
		//uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的
		//glUniform3f glUniform4i glUniformi 

		glBindVertexArray(VAO); //虽然我们只有一个VAO,不用每次都绑定,但是这样看更加直观
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //正确释放/删除之前的分配的所有资源


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

/*
 *支持的最大顶点数据 数量
 *要在OPENGL 和 WINDOW init 之后
 */
int _03_HelloShader::SupportVertexAttr()
{
	int nrAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;
	return nrAttributes;
}
