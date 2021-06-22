#pragma once

#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>


#include "Mesh.h"
#include "assimp/mesh.h"

#define MAX_BONE_INFLUENCE 4

struct SkinVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;

    //bone indexes which will influence this vertex
    int BoneIDs[MAX_BONE_INFLUENCE];

    //weights from each bone
    float Weights[MAX_BONE_INFLUENCE];
};

class SkinMesh 
{
public:
	~SkinMesh();
	/* 网格数据 */
	std::vector<SkinVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/* 渲染数据 */
	unsigned int VAO, VBO, EBO;
	SkinMesh(std::vector<SkinVertex> vertices, std::vector<unsigned int> indices
		, std::vector<Texture> textures);
	virtual void Draw(Shader shader);
private:
	/* 函数 */
	virtual void SetupMesh();
};
