#version 460 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform int lightCount;

uniform vec3 cameraPos;

const float PI=3.14159265359;

vec3 GetNormalFromMap()
{
	vec3 tangentNormal=texture(normalMap,TexCoords).xyz*2.-1.;
	
	//正常的渲染是2*2的去渲染  可以求出相邻两个坐标的差值
	vec3 Q1=dFdx(WorldPos);
	vec3 Q2=dFdy(worldPos);
	vec2 st1=dFdx(TexCoords);
	vec2 st2=dFdy(TexCoords);
	
	//简单的技巧，使切线法线到世界空间保持简化PBR代码。
	//不然要传入 tangent
	vec3 N=normalize(Normal);
	vec3 T=normalize(Q1*st2.y-Q2*st1.y);
	vec3 B=-normalize(cameraPos(N,T));
	
	mat3 TBN=mat3(T,B,N);
}

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
//TODO: