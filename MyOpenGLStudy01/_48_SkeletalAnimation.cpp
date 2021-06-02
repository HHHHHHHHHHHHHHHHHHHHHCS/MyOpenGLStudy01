#include "_48_SkeletalAnimation.h"

#include "CommonBaseScript.h"
#include "Camera.h"
#include "Model.h"


int _48_SkeletalAnimation::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if (window == nullptr)
	{
		return -1;
	}

	// configure global opengl state
// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("anim_model_vs.glsl", "anim_model_fs.glsl");


	// load models
	// -----------
	Model ourModel("Models/vampire/dancing_vampire.dae");
	// Animation danceAnimation(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"), &ourModel);
	// Animator animator(&danceAnimation);


	//Camera
	//-----------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);


	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glfwTerminate();

	return 0;
}
