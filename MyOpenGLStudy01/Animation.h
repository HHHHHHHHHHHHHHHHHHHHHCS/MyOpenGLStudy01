#pragma once
#include <mat4x4.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include "Bone.h"
#include "SkinModel.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const std::string& animationPath, SkinModel* model);

	Bone* FindBone(const std::string& name)
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

	inline float GetTicksPerSecond() const
	{
		return ticksPerSecond;
	}

	inline float GetDuration() const
	{
		return duration;
	}

	inline const AssimpNodeData& GetRootNode() const
	{
		return rootNode;
	}

	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() const
	{
		return boneInfoMap;
	}

private:
	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;

	void SetupBones(const aiAnimation* animation, SkinModel& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
};
