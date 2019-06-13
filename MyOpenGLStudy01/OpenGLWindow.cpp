#include<Windows.h>
#include <stdio.h>
#include <glad/glad.h>//GLAD����������OpenGL�ĺ���ָ���
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Mac OS X��Ҫ



	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��glad ,����Ҫ�ڴ��ڳ�ʼ��֮��ִ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//�������ر�

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����glClear֮��,������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//��ջ�����ɫ

		glfwSwapBuffers(window); //�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents(); //�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
	}

	glfwTerminate(); //��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ

	return 0;
}

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ������Ļص�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //���ô��ڵ�ά��
}

//����ESC �ر�
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}