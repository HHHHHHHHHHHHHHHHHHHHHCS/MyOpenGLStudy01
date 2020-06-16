#pragma once

struct _34_ParallaxOcclusionMapping
{
	static unsigned int quadVAO;

	static int DoMain();
	static void BindQuadVAO();
	static void RenderQuad();
};
