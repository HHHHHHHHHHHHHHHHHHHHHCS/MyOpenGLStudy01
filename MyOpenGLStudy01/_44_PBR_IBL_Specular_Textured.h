#pragma once

class _44_PBR_IBL_Specular_Textured
{
public:
	static unsigned int quadVAO;
	static unsigned int cubeVAO;
	static unsigned int sphereVAO;
	static unsigned int sphereIndexCount;

	static int DoMain();
	static void BindQuadVAO();
	static void BindCubeVAO();
	static void BindSphereVAO();
	static void RenderQuad();
	static void RenderCube();
	static void RenderSphere();
};
