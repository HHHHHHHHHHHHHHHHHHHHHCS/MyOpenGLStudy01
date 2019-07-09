#include "_02_HelloTriangle_Practice.h"

int _02_HelloTriangle_Practice::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource{
		R"(
		#version 460 core
		layout(location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)"
	};
	glad_glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	CommonBaseScript::CheckCompile(vertexShader);


	const char* fragmentShaderSource0{
		R"(
		#version 460 core
		out vec4 FragColor;

		void main()
		{
		    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		} 
		)"
	};
	unsigned int fragmentShader0;
	fragmentShader0 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader0, 1, &fragmentShaderSource0, nullptr);
	glCompileShader(fragmentShader0);
	CommonBaseScript::CheckCompile(fragmentShader0);

	unsigned int shaderProgram0;
	shaderProgram0 = glCreateProgram();
	glAttachShader(shaderProgram0, vertexShader);
	glAttachShader(shaderProgram0, fragmentShader0);
	glLinkProgram(shaderProgram0);
	CommonBaseScript::CheckCompile(shaderProgram0);

	const char* fragmentShaderSource1{
	R"(
		#version 460 core
		out vec4 FragColor;

		void main()
		{
		    FragColor = vec4(1.0f, 0f,0f, 1.0f);
		} 
		)"
	};
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, nullptr);
	glCompileShader(fragmentShader1);
	CommonBaseScript::CheckCompile(fragmentShader1);

	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	CommonBaseScript::CheckCompile(shaderProgram1);

	glDeleteShader(vertexShader); 
	glDeleteShader(fragmentShader0);
	glDeleteShader(fragmentShader1);

	float vertices0[] = {
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	float vertices1[] = {
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	//VBO 是一组顶点数据
	//VAO 是VBO的数组
	unsigned int VAOs[2],VBOs[2];
	glGenVertexArrays(2, VAOs);// 这里有两组数据组就是 2
	glGenBuffers(2, VBOs);// 这里有两个顶点数据就是 2

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //安全解除GL_ARRAY_BUFFER ,防止不小心修改
	glBindVertexArray(0); //安全解除顶点数据 ,防止不小心修改


	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window); //按键关闭检测

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //调用glClear之后,整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		glClear(GL_COLOR_BUFFER_BIT); //清空缓冲颜色

		glUseProgram(shaderProgram0); //使用这个程序对象

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6); //画顶点 ,0起始顶点索引 ,需要画的顶点长度

		glUseProgram(shaderProgram1); //使用这个程序对象

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6); //画顶点 ,0起始顶点索引 ,需要画的顶点长度

		glfwSwapBuffers(window); //函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
	}

	glfwTerminate(); //正确释放/删除之前的分配的所有资源


	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	return 0;
}
