#include "Model.h"

Model::Model(char *path)
{
	LoadModel(path);
}

void Model::Draw(Shader shader)
{
	for(unsigned int i=0;i<meshes.size();i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	//第二个参数是后期处理      Triangulate是全部三角化      FlipUVs翻转UV 的 Y坐标
	//aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
	//aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
	//aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//如果没有场景  或者 flag==错误的flag  或者  没有根目录
	if(!scene||(scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE)||!scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//处理节点所有的网格(如果有的话)
	for(unsigned int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene);
	}

	//接下来对他的子节点继续重复这一过程
	for(unsigned int i=0;i<node->mNumChildren;i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
	//TODO:
}

