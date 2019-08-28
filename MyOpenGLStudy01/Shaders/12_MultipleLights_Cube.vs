#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	FragPos=vec3(model*vec4(aPos,1.));
	//在shader中逆矩阵计算比较消耗性能  建议在CPU做
	//normal因为不规则缩放 所以要这么做
	Normal=mat3(transpose(inverse(model)))*aNormal;
	
	//乘法要从右向左边读
	gl_Position=projection*view*vec4(FragPos,1);
	
	TexCoords=aTexCoords;
}