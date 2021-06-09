#include "SkinModel.h"


#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "AssimpGLMHelpers.h"
#include "ImageHelper.h"


SkinModel::SkinModel(const std::string& path)
{
	LoadModel(path);
}

SkinModel::~SkinModel()
{
}

std::vector<SkinMesh> SkinModel::GetMeshes() const
{
	return meshes;
}

std::vector<Texture*> SkinModel::GetTextures() const
{
	return textures_loaded;
}


void SkinModel::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void SkinModel::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	//第二个参数是后期处理      Triangulate是全部三角化      FlipUVs翻转UV 的 Y坐标   CalcTangentSpace 读取的时候自动计算法线切线
	//aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
	//aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
	//aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
	                                         aiProcess_CalcTangentSpace);

	//如果没有场景  或者 flag==错误的flag  或者  没有根目录
	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void SkinModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	//处理节点所有的网格(如果有的话)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(ProcessMesh(mesh, scene));
		for (Texture& texture : meshes.back().textures)
		{
			textures_loaded.emplace_back(&texture);
		}
	}

	//接下来对他的子节点继续重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}


SkinMesh SkinModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<SkinVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	LoadVertices(&vertices, mesh, scene);
	LoadIndices(&indices, mesh, scene);
	LoadTextures(&textures, mesh, scene);

	return SkinMesh(vertices, indices, textures);
}

void SkinModel::LoadVertices(std::vector<SkinVertex>* vertices, aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinVertex vertex;

		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

		SetVertexBoneDataToDefault(vertex);

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices->push_back(vertex);
	}
}

void SkinModel::LoadIndices(std::vector<unsigned int>* indices, aiMesh* mesh, const aiScene* scene)
{
	//处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices->push_back(face.mIndices[j]);
		}
	}
}


void SkinModel::LoadTextures(std::vector<Texture>* textures, aiMesh* mesh, const aiScene* scene)
{
	//处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		// mMaterialIndex mesh 在 场景材质列表 中的索引 
		// 信息存在mtl中
		// 很奇怪 height 是 normal    ambient  是height
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures->insert(textures->end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures->insert(textures->end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures->insert(textures->end(), normalMaps.begin(), normalMaps.end());
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures->insert(textures->end(), heightMaps.begin(), heightMaps.end());
	}
}

std::vector<Texture> SkinModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (auto* img : textures_loaded)
		{
			//用于比较两个字符串并根据比较结果返回整数。
			//基本形式为strcmp(str1,str2)，若str1=str2，则返回零；
			//若str1<str2，则返回负数；若str1>str2，则返回正数

			if (std::strcmp(img->path.data, str.C_Str()) == 0)
			{
				textures.push_back(*img);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = ImageHelper::LoadTexture(str.C_Str(), directory + "/");
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
		}
	}
	return textures;
}

void SkinModel::SetVertexBoneDataToDefault(SkinVertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}
