#version 460 core

out vec4 FragColor;

in vec3 TexCoords;//代表3D纹理坐标的方向向量
uniform samplerCube skybox;

void main()
{
	FragColor=texture(skybox,TexCoords);
}