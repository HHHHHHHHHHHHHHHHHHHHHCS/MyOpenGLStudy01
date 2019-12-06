#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	// 空气	1.00
	// 水	1.33
	// 冰	1.309
	// 玻璃	1.52
	// 钻石	2.42
	float ratio=1./1.52;
	vec3 I=normalize(Position-cameraPos);
	vec3 R=refract(I,normalize(Normal),ratio);
	FragColor=vec4(texture(skybox,R).rgb,1.);
}