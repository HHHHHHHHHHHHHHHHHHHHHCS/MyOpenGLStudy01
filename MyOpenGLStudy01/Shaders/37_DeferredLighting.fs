#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
	vec3 Position;
	vec3 Color;
	
	float Linear;
	float Quadratic;
};

const int NR_LIGHTS=32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
	vec3 FragPos=texture(gPosition,TexCoords).rgb;
	vec3 Normal=texture(gNormal,TexCoords).rgb;
	vec3 Diffuse=texture(gAlbedoSpec,TexCoords).rgb;
	float Specular=texture(gAlbedoSpec,TexCoords).a;

	vec3 lighting = Diffuse * 0.1;
	//TODO:
}