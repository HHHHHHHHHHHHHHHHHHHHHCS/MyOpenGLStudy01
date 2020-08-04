#include "_40_PBR_Lighting.h"
#include <glad/glad.h>
#include <vector>
#include <vec3.hpp>
#include <vec2.hpp>

unsigned int _40_PBR_Lighting::sphereVAO = 0;
unsigned int _40_PBR_Lighting::sphereIndexCount = 0;

int _40_PBR_Lighting::DoMain()
{
	return 0;
}


void _40_PBR_Lighting::BindSphereVAO()
{
	glGenVertexArrays(1, &sphereVAO);

	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;

	//https://baike.baidu.com/item/%E7%90%83%E5%9D%90%E6%A0%87%E7%B3%BB/8315363?fr=aladdin
	//球面坐标
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.emplace_back(xPos, yPos, zPos);
			uv.emplace_back(xSegment, ySegment);
			normals.emplace_back(xPos, yPos, zPos);
		}
	}

	//4个点画一个quad 错行 合成
	bool oddRow = false;//奇数行
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow)//偶数行
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

}
