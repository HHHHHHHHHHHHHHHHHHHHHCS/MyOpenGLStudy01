#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WolrdPos;
in vec3 Normal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLut;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI=3.14159265359;

vec3 GetNormalFromMap()
{
	vec3 tangentNormal=texture(normalMap,TexCoords).xyz*2.-1.;
	
	vec3 Q1=dFdx(WolrdPos);
	vec3 Q2=dFdy(WolrdPos);
	vec2 st1=dFdx(TexCoords);
	vec2 st2=dFdy(TexCoords);
	
	vec3 N=normalize(Normal);
	vec3 T=normalize(Q1*st2.y-Q2*st1.y);
	vec3 B=-normalize(cross(N,T));
	mat3 TBN=mat3(T,B,N);
	
	return normalize(TBN*tangentNormal);
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

float GeometrySchlickGGX(float NdotV,float roughness)
{
	float r=(roughness+1.);
	float k=(r*r)/8.;
	
	float nom=NdotV;
	float denom=NdotV*(1.-k)+k;
	
	return nom/denom;
}

float GeomtrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	float NdotV=max(dot(N,V),0.);
	float NdotL=max(dot(N,L),0.);
	float ggx2=GeometrySchlickGGX(NdotV,roughness);
	float ggx1=GeometrySchlickGGX(NdotL,roughness);
	
	return ggx1*ggx2;
}

vec3 FresnelSchlick(float cosTheta,vec3 F0)
{
	return F0+(1.-F0)*pow(1.-cosTheta,5.);
}

vec3 FresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness)
{
	return F0+max(vec3(1.-roughness),F0)-F0)*pow(1.-cosTheta,5.);
}

void main()
{
	vec3 albedo=pow(texture(albedoMap,TexCoords).rgb,vec3(2.2));
	float metallic=texture(metallicMap,TexCoords).r;
	float roughness=texture(roughnessMap,TexCoords).r;
	float ao=texture(aoMap,TexCoords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(camPos-WolrdPos);
	vec3 R = reflect(-V,N);

	vec3 F0 = vec3(0.04);
	//TODO:
}