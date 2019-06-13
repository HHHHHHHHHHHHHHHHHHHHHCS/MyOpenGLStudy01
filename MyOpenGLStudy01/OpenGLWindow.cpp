#include<Windows.h>
#include <stdio.h>
#include <glad/glad.h>//GLAD是用来管理OpenGL的函数指针的
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Mac OS X需要



	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化glad ,但是要在窗口初始化之后执行
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//按键检测关闭

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//调用glClear之后,整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空缓冲颜色

		glfwSwapBuffers(window); //函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
	}

	glfwTerminate(); //正确释放/删除之前的分配的所有资源

	return 0;
}

//当用户改变窗口的大小的时候，视口也应该被调整的回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //设置窗口的维度
}

//按下ESC 关闭
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}