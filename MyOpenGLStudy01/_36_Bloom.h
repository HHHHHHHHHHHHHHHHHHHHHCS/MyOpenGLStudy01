﻿#pragma once

class _36_Bloom
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