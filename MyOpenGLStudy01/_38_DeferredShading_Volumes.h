#pragma once

class _38_DeferredShading_Volumes
{
public:
	static unsigned int cubeVAO;
	static unsigned int quadVAO;

	static int DoMain();
	static void BindCubeVAO();
	static void RenderCube();
	static void BindQuadVAO();
	static void RenderQuad();
};
