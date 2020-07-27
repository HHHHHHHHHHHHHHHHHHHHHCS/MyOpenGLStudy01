#pragma once

class _39_SSAO
{
public:
	static unsigned int cubeVAO;
	static unsigned int quadVAO;


	static int DoMain();
	static float Lerp(float a, float b, float f);

	static void BindCubeVAO();
	static void RenderCube();
	static void BindQuadVAO();
	static void RenderQuad();
};
