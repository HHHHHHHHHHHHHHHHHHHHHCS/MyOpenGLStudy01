#pragma once

class _40_PBR_Lighting
{
public:
	static unsigned int sphereVAO;
	static unsigned int sphereIndexCount;

	static int DoMain();
	static void BindSphereVAO();
	static void RenderSphere();
};
