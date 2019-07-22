#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "CommonBaseScript.h"

class _06_CoordinateSystems_Practice
{
public:
	static float x;
	static float y;
	static float dov;
	static float aspect;
	static int DoMain();
};
