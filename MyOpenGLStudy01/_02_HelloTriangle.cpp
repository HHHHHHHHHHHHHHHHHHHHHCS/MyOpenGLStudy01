#include "_02_HelloTriangle.h"



int _02_HelloTriangle::DoMain()
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
	CheckCompile(vertexShader);


	const char* fragmentShaderSource{
		R"(
		#version 460 core
		out vec4 FragColor;

		void main()
		{
		    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		} 
		)"
	};
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	CheckCompile(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CheckCompile(shaderProgram);

	glDeleteShader(vertexShader); //删除已经没有用的顶点着色器对象
	glDeleteShader(fragmentShader); //删除已经没有用的片元着色器对象


	//VBO 是一组顶点数据
	//VAO 是VBO的数组
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // 创建VAO 1是VAO长度
	glBindVertexArray(VAO); //绑定VAO

	//不用EBO 画六个顶点用
	/*
	float vertices[] = {
		// 第一个三角形
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, 0.5f, 0.0f,  // 左上角
		// 第二个三角形
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	*/

	float vertices[] = {
		0.5f, 0.5f, 0.0f, // 右上角
		0.5f, -0.5f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f // 左上角
	};


	unsigned int VBO;
	glGenBuffers(1, &VBO); // 1是缓冲ID , 即有几个数量
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STATIC_DRAW : 数据不会或几乎不会改变使用.
	//GL_DYNAMIC_DRAW : 数据会被改变很多时候用,把数据写到高速写入的内存部分.
	//GL_STREAM_DRAW : 数据每次绘制时都会改变使用,把数据写到高速内存部分.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告诉OPENGL怎么解析这个数据
	//第一个参数 layout(location = 0) location 顶点位置属性  0. 因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	//第二个参数 顶点有几个数量 float3 -> 3个组成
	//第三个参数 数据类型 vec* 是 GL_FLOAT 组成的
	//第四个参数 希望数据是否被标准化 如果是 无符号则映射到 0~1    有符号则映射到 -1~1  False为不标准化
	//第五个参数 到下个顶点的步长  也可以设置为0 让OpenGL 自己决定
	//第六个参数 数据缓冲 起始的位置  我们这里是0开始
	//reinterpret_cast	它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0); //启用顶点属性  默认是禁用的

	//EBO
	unsigned int indices[] = {
		// 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3 // 第二个三角形
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //安全解除GL_ELEMENT_ARRAY_BUFFER ,防止不小心修改
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

		glBindVertexArray(VAO); //虽然我们只有一个VAO,不用每次都绑定,但是这样看更加直观 EBO也要用到
		//画顶点 ,0起始顶点索引 ,需要画的顶点长度
		//glDrawArrays(GL_TRIANGLES, 0, 6); 

		//EBO用
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//EBO画的时候用 绘制的模式  长度  数据类型  偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


		glfwSwapBuffers(window); //函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
	}

	glfwTerminate(); //正确释放/删除之前的分配的所有资源


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

/*
 * 检测编译用
 */
bool _02_HelloTriangle::CheckCompile(unsigned int id)
{
	//检测编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	//如果编译失败
	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return success;
}
