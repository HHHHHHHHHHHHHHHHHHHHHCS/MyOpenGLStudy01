#include "_01_HelloWindow.h"

int _01_HelloWindow::DoMain()
{
	InitOpenGL();
	GLFWwindow* window = InitWindow();
	if(window==nullptr)
	{
		return -1;
	}
	UpdateDraw(window);

	return 0;
}

void _01_HelloWindow::InitOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ��OpenGL����ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //ƻ����Ҫ
#endif
}

GLFWwindow* _01_HelloWindow::InitWindow()
{
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��glad ,����Ҫ�ڴ��ڳ�ʼ��֮��ִ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	return window;
}

void _01_HelloWindow::UpdateDraw(GLFWwindow* window)
{
	//glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window); //�����رռ��

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //����glClear֮��,������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT); //��ջ�����ɫ

		glfwSwapBuffers(window); //�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents(); //�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
	}

	glfwTerminate(); //��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
}

//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ������Ļص�
void _01_HelloWindow::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //���ô��ڵ�ά��
}

//����ESC �ر�
void _01_HelloWindow::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
