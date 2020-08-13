#pragma once

class _41_PBR_Lighting_Textured
{
public:
	static unsigned int sphereVAO;
	static unsigned int sphereIndexCount;

	static int DoMain();
	static void BindSphereVAO();
	static void RenderSphere();
};
