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
	
	~Animation();

	Bone* FindBone(const std::string& name);

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
