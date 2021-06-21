#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

//别的材质就不写了  这里是做骨骼矩阵demo
struct Material
{
	sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
	FragColor=texture(material.texture_diffuse1,TexCoords);
}