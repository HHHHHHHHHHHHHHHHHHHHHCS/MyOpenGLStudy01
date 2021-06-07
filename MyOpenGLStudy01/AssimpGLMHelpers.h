#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include<assimp/Quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>


class AssimpGLMHelpers
{
public:
	static inline glm::mat4 CovertMatrixToGLMFormat(const aiMatrix4x4& from);
	static inline glm::vec3 GetGLMVec(const aiVector3D& from);
	static inline glm::quat GetGLMQuat(const aiQuaternion& from);
};
