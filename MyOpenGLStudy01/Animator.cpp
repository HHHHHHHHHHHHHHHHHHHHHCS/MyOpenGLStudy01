#include "Animator.h"

Animator::Animator(Animation* current)
{
	currentAnimation = current;
	currentTime = 0.0f;
	transforms.reserve(100); //这里的作用是开创建capacity
	for (int i = 0; i < 100; i++)
	{
		transforms.emplace_back(glm::mat4(1.0f));
	}
}

void Animator::UpdateAnimation(float dt)
{
	deltaTime = dt;
	if (currentAnimation)
	{
		currentTime += currentAnimation->GetTicksPerSecond() * dt;
		//loop
		currentTime = fmod(currentTime, currentAnimation->GetDuration());
		CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* pAnimation)
{
	currentAnimation = pAnimation;
	currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = currentAnimation->FindBone(nodeName);

	if (bone)
	{
		bone->Update(currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = currentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		transforms[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}


std::vector<glm::mat4> Animator::GetPoseTransform()
{
	return transforms;
}

int Animator::GetMaxJoints()
{
	return transforms.size();
}
