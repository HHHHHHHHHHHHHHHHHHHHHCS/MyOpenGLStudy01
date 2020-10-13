#pragma once
#include <glad/glad.h>
#include "Shader.h"

class _45_Debugging
{
public:
	static bool notInitDebugging;
	static Shader debuggingDisplayShader;
	static unsigned int debuggingQuadVAO;
	static void DisplayFramebufferTexture(GLuint textureID);
	static int DoMain();
};
