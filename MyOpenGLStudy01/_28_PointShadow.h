#pragma once
#include "Shader.h"

class _28_PointShadow
{
public:
	static unsigned int cubeVAO;

	static int DoMain();
	static void RenderScene(Shader shader);
	static void BindCubeVAO();
	static void RenderCube();
};
