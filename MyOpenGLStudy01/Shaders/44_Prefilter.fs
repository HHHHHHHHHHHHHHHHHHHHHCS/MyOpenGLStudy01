#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;

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

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
//模拟随机
float RadicalInverse_VdC(uint bits)
{
	bits=(bits<<16u)|(bits>>16u);
	bits=((bits&0x55555555u)<<1u)|((bits&0xAAAAAAAAu)>>1u);
	bits=((bits&0x33333333u)<<2u)|((bits&0xCCCCCCCCu)>>2u);
	bits=((bits&0x0F0F0F0Fu)<<4u)|((bits&0xF0F0F0F0u)>>4u);
	bits=((bits&0x00FF00FFu)<<8u)|((bits&0xFF00FF00u)>>8u);
	return float(bits)*2.3283064365386963e-10;// / 0x100000000
}

vec2 Hammersley(uint i,uint N)
{
	return vec2(float(i)/float(N),RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi,vec3 N,float roughness)
{
	float a=roughness*roughness;
	
	float phi=2.*PI*Xi.x;
	float cosTheta=sqrt((1.-Xi.y)/(1.+(a*a-1.)*Xi.y));
	//TODO:
	
}