﻿#include "_48_SkeletalAnimation.h"


#include "Animation.h"
#include "Animator.h"
#include "CommonBaseScript.h"
#include "Camera.h"
#include "SkinModel.h"


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
	Shader animShader("48_SkeletalAnimation");


	// load models
	// -----------
	SkinModel animModel("Models/vampire/dancing_vampire.dae");
	Animation danceAnimation("Models/vampire/dancing_vampire.dae", &animModel);
	Animator animator(&danceAnimation);


	//Camera
	//-----------
	Camera camera{};
	Camera::AddMouseEvent(window);
	CommonBaseScript::RegisterKeyEvent(window);

	float deltaTime = 0;
	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		CommonBaseScript::ProcessInput(window);
		CommonBaseScript::ProcessKeyClick();
		camera.DoKeyboardMove(window);

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		animator.UpdateAnimation(deltaTime);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		animShader.Use();

		animShader.SetMat4("_ViewProjection", camera.GetViewProjection());

		auto transforms = animator.GetPoseTransform();
		for (int i = 0; i < transforms.size(); i++)
		{
			animShader.SetMat4("_FinalBonesTransformations[" + std::to_string(i) + "]", transforms[i]);
			// if (i == 0)
			// {
			// 	auto mm = transforms[i];
			// 	printf("%f,%f,%f,%f|%f,%f,%f,%f|%f,%f,%f,%f|%f,%f,%f,%f\n"
			// 	       , mm[0].x, mm[1].x, mm[2].x, mm[3].x
			// 	       , mm[0].y, mm[1].y, mm[2].y, mm[3].y
			// 	       , mm[0].z, mm[1].z, mm[2].z, mm[3].z
			// 	       , mm[0].w, mm[1].w, mm[2].w, mm[3].w);
			// }
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		animShader.SetMat4("_Model", model);
		animModel.Draw(animShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}
