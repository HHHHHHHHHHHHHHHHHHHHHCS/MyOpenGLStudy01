#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

//material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

//light
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 cameraPos;

const float PI=3.14159265359;

//GGX
//-----------------
float DistributionGGX(vec3 N,vec3 H,float roughness)
{
	float a=roughness*roughness;
	float a2=a*a;
	float NdotH=max(dot(N,H),0.);
	float NdotH2=NdotH*NdotH;
	
	float nom=a2;
	float denom=(NdotH2*(a2-1.)+1.);
	denom=PI*denom*denom;
	
	return nom/max(denom,.001);//防止除以0
}