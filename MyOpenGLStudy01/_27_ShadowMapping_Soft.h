#pragma once

class Shader;

class _27_ShadowMapping_Soft
{
public:
	static unsigned int planeVAO;
	static unsigned int cubeVAO;
	static unsigned int quadVAO;

	static int DoMain();
	static void BindPlaneVAO();
	static void BindCubeVAO();
	static void BindQuadVAO();
	static void RenderScene(const Shader& shader);
	static void RenderCube();
	static void RenderQuad();
};
