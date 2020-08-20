#pragma once

class _42_PBR_IBL_Irradiance_Conversion
{
public:
	static unsigned int cubeVAO;

	static int DoMain();
	static void BindCubeVAO();
	static void RenderCube();
};
