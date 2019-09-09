#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(char *path);
	void Draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	void ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTexture type, std::string typeName);
};
