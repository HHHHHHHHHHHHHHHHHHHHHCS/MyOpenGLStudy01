#pragma once


#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "CommonBaseScript.h"

class _07_Camera
{
public:
	static glm::vec3 cameraPos;
	static glm::vec3 cameraFront;
	static glm::vec3 cameraUp;
	static int DoMain();
};
