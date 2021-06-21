#version 460 core

layout(location=0)in vec3 pos;
layout(location=1)in vec3 norm;
layout(location=2)in vec2 tex;
layout(location=3)in ivec4 boneIds;
layout(location=4)in vec4 weights;

uniform mat4 _ViewProjection;
uniform mat4 _Model;

const int MAX_BONES=100;
const int MAX_BONE_INFLUENCE=4;
uniform mat4 _FinalBonesTransformations[MAX_BONES];

out vec2 TexCoords;

void main()
{
	vec4 totalPosition=vec4(0.f);
	for(int i=0;i<MAX_BONE_INFLUENCE;i++)
	{
		int boneId=boneIds[i];
		if(boneId==-1)
		{
			continue;
		}
		
		//TODO:这里理论上应该是jot max
		if(boneId>=MAX_BONES)
		{
			totalPosition=vec4(pos,1.f);
			break;
		}
		
		mat4 mat=_FinalBonesTransformations[boneId];
		vec4 localPosition=mat*vec4(pos,1.f);
		totalPosition+=localPosition*weights[i];
		// 这里没有算normal
		// vec3 localNormal=mat3(mat)*norm;
	}
	
	gl_Position=_ViewProjection*_Model*totalPosition;
	TexCoords=tex;
	
}
