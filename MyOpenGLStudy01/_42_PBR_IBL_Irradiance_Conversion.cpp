#include "_42_PBR_IBL_Irradiance_Conversion.h"
#include "CommonBaseScript.h"

int _42_PBR_IBL_Irradiance_Conversion::DoMain()
{
	CommonBaseScript::InitOpenGL();
	GLFWwindow* window = CommonBaseScript::InitWindow();

	if(window == nullptr)
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);//天空盒深度是1  小于等于1 的被渲染

	//TODO:
}
