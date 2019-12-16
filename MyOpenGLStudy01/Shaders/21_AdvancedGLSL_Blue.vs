#version 460 core
layout(location=0)in vec3 aPos;

//三个布局块  Shared  std140  packed
//GL4.2+ 可以加binding绑定点储存的位置 可以不再调用glGetUniformBlockIndex和glUniformBlockBinding
layout(std140/*, binding = 0*/)uniform VPMatrices
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position=projection*view*model*vec4(aPos,1.);
}