#version 460 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform float lightCount;

uniform vec3 camPos;

const float PI=3.14159265359;

float DistributionGGX(vec3 N,vec3 H,float roughness)
{
	float a=roughness*roughness;
	float a2=a*a;
	float NdotH=max(dot(N,H),0.);
	float NdotH2=NdotH*NdotH;
	
	float nom=a2;
	float denom=(NdotH2*(a2-1.)+1.);
	denom=PI*denom*denom;
	
	return nom/denom;
}

float GeometrySchlickGGX(float NdotV,float roughness)
{
	float r=(roughness+1.);
	float k=(r*r)/8.;
	
	float nom=NdotV;
	float denom=NdotV*(1.-k)+k;
	
	return nom/denom;
}

float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	float NdotV=max(dot(N,V),0.);
	float NdotL=max(dot(N,L),0.);
	float ggx2=GeometrySchlickGGX(NdotV,roughness);
	float ggx1=GeometrySchlickGGX(NdotL,roughness);
	
	return ggx1*ggx2;
}