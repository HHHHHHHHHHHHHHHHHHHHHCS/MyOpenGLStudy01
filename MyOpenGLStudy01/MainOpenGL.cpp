#include <crtdbg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "_01_HelloWindow.h"
#include "_02_HelloTriangle.h"
#include "_02_HelloTriangle_Practice.h"
#include "_03_HelloShader.h"
#include "_03_HelloShader_File.h"
#include "_03_HelloShard_Practice.h"
#include "_04_HelloTextures.h"
#include "_04_HelloTextures_Practice.h"
#include "_05_HelloTRS.h"
#include "_05_HelloTRS_Practice.h"
#include "_06_CoordinateSystems.h"
#include "_06_CoordinateSystems_Practice.h"
#include "_07_Camera.h"
#include "_08_Lighting.h"
#include "_09_BasicLighting.h"
#include "_10_LightingMaps.h"
#include "_11_LightCasters.h"
#include "_11_LightCasters_Practice.h"
#include "_12_MultipleLights.h"
#include "_13_Mesh.h"
#include "_14_DepthTesting.h"
#include "_15_StencilTesting.h"
#include "_16_Blending_Discard.h"
#include "_17_Blending_Sort.h"
#include "_18_FaceCulling.h"
#include "_19_Framebuffers.h"
#include "_20_Cubemaps.h"
#include "_21_AdvancedGLSL.h"
#include "_22_GeometryShader.h"
#include "_23_Instancing.h"
#include "_23_Instancing_Practice.h"
#include "_24_MSAA.h"
#include "_24_MSAA_MultiFramebuffer.h"
#include "_25_BlinnPhong.h"
#include "_26_GammaCorrection.h"
#include "_27_ShadowMapping_Debug.h"
#include "_27_ShadowMapping_Base.h"
#include "_27_ShadowMapping_Soft.h"
#include "_28_PointShadow.h"
#include "_29_CSM.h"
#include "_30_NormalMapping.h"
#include "_31_ModelNormal.h"
#include "_32_ParallaxMapping.h"
#include "_33_SteepParallaxMapping.h"
#include "_34_ParallaxOcclusionMapping.h"

int main(int argc, char* argv[])
{
	//_CrtSetBreakAlloc(164);
	int ret = _13_Mesh::DoMain();
	_CrtDumpMemoryLeaks();
	return 0;
}
