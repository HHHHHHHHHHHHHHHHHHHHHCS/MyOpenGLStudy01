#pragma once
#include <map>
#include <vector>
#include <assimp/material.h>
#include <assimp/scene.h>


#include "SkinMesh.h"

struct BoneInfo
{
public:
	int id;

	glm::mat4 offset;
};

class SkinModel
{
public:
	SkinModel(const std::string& path);
	~SkinModel();

	std::vector<SkinMesh> GetMeshes() const;
	std::vector<Texture*> GetTextures() const;

	void Draw(Shader shader);

private:
	std::vector<SkinMesh> meshes;
	std::string directory;
	std::vector<Texture*> textures_loaded;

	std::map<std::string, BoneInfo> offsetMatMap;
	int boneCount = 0;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	SkinMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadVertices(std::vector<SkinVertex>* vertices, aiMesh* mesh, const aiScene* scene);
	void LoadIndices(std::vector<unsigned>* indices, aiMesh* mesh, const aiScene* scene);
	void LoadTextures(std::vector<Texture>* textures, aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void SetVertexBoneDataToDefault(SkinVertex& vertex);
	void SertVertexBoneData(SkinVertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<SkinVertex>& vertices, aiMesh* mesh, const aiScene* scene);
};
