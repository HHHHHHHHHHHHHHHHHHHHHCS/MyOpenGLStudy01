#include "Animation.h"

Animation::Animation(const std::string& animationPath, SkinModel* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto* animation = scene->mAnimations[0];
}
