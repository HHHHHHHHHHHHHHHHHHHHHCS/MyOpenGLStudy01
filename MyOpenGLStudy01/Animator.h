#pragma once
#include <fwd.hpp>
#include <vector>

#include "Animation.h"

class Animator
{
public:
	Animator(Animation* current);

	void UpdateAnimation(float dt);

	void PlayAnimation(Animation* pAnimation);

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetPoseTransform();
	int GetMaxJoints();

private:
	std::vector<glm::mat4> transforms;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
};
