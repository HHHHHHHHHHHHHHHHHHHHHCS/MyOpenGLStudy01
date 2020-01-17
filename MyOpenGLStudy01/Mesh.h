#pragma once
#include <vector>
#include "Shader.h"
#include <assimp/types.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path; // 我们储存纹理的路径用于与其它纹理进行比较
};

class Mesh
{
public:
	~Mesh();
	/* 网格数据 */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/* 渲染数据 */
	unsigned int VAO, VBO, EBO;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices
	     , std::vector<Texture> textures);
	void Draw(Shader shader);
private:
	/* 函数 */
	void SetupMesh();
};
