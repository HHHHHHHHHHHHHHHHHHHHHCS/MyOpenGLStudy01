#pragma once

//GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0）
//而不是单位矩阵（对角元素为1，其它元素为0）
//如果你使用的是0.9.9或0.9.9以上的版本
//你需要将所有的矩阵初始化改为 glm::mat4 mat = glm::mat4(1.0f)
//我们需要的GLM的大多数功能都可以从下面这3个头文件中找到：
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "CommonBaseScript.h"

class _05_HelloTRS
{
public:
	static int DoMain();
};
