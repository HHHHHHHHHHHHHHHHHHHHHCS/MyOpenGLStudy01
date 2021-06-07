#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const std::string& path);
	~Model();

	std::vector<Mesh> GetMeshes() const;
	std::vector<Texture*> GetTextures() const;

	void Draw(Shader shader);


private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture*> textures_loaded;
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadVertices(std::vector<Vertex>* vertices, aiMesh* mesh, const aiScene* scene);
	void LoadIndices(std::vector<unsigned>* indices, aiMesh* mesh, const aiScene* scene);
	void LoadTextures(std::vector<Texture>* textures, aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
