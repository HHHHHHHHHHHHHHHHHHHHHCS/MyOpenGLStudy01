#include "SkinModel.h"

#include "AssimpGLMHelpers.h"

SkinModel::SkinModel(std::string const& path): Model(path)
{
}

void SkinModel::SetVertexBoneDataToDefault(SkinVertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}


Mesh SkinModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<SkinVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinVertex vertex;

		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

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

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace face = mesh->mFaces[i];
			for(unsigned int j=0;j<face.mNumIndices;j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}


		LoadTextures(textures, mesh, scene);

		return SkinMesh()
	}
}
