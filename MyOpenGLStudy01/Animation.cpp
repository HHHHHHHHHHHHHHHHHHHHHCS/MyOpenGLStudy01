#include "Animation.h"

#include "AssimpGLMHelpers.h"

Animation::Animation(const std::string& animationPath, SkinModel* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	//默认第0个动画
	auto* animation = scene->mAnimations[0];
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	// aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	// globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(rootNode, scene->mRootNode);
	SetupBones(animation, *model);
}

Animation::~Animation()
{
}

Bone* Animation::FindBone(const std::string& name)
{
	//&。函数体内可以使用 Lambda 所在范围内所有可见的局部变量（包括 Lambda 所在类的 this），并且是引用传递方式
	//（相当于是编译器自动为我们按引用传递了所有局部变量）。
	//vector<Bone>::iterator 这里是指针
	const auto iter = std::find_if(bones.begin(), bones.end(),
	                               [&](const Bone& bone)
	                               {
		                               return bone.GetBoneName() == name;
	                               });
	if (iter == bones.end())
	{
		return nullptr;
	}
	else
	{
		return &*iter;
	}
}


void Animation::SetupBones(const aiAnimation* animation, SkinModel& model)
{
	//channels  所有动的骨骼的通道的总数量
	int size = animation->mNumChannels;

	auto& _boneInfoMap = model.GetOffsetMatMap();
	int& boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto* channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (_boneInfoMap.find(boneName) == _boneInfoMap.end())
		{
			_boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		bones.emplace_back(channel->mNodeName.data,
		                   _boneInfoMap[channel->mNodeName.data].id, channel);
	}

	boneInfoMap = _boneInfoMap;
}


void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	//TODO:
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::CovertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.emplace_back(newData);
	}
}
