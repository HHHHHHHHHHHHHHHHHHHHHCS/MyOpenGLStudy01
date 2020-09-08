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

//法线分部
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

vec3 FresnelSchlick(float cosTheta,vec3 F0)
{
	return F0+(1.-F0)*pow(1.-cosTheta,5.);
}

//菲尼尔计算被反射的光线的百分比
vec3 FresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness)
{
	return F0+(max(vec3(1.-roughness),F0)-F0)*pow(1.-cosTheta,5.);
}

void main()
{
	vec3 N=Normal;
	vec3 V=normalize(camPos-WorldPos);
	vec3 R=reflect(-V,N);
	
	vec3 F0=vec3(.04);
	F0=mix(F0,albedo,metallic);
	
	vec3 Lo=vec3(0.);
	for(int i=0;i<lightCount;++i)
	{
		//计算每盏灯光的辐射度
		vec3 L=normalize(lightPositions[i]-WorldPos);
		vec3 H=normalize(V+L);
		float distance=length(lightPositions[i]-WorldPos);
		float attenuation=1./(distance*distance);
		vec3 radiance=lightColors[i]*attenuation;
		
		//BRDF
		float NDF=DistributionGGX(N,H,roughness);
		float G=GeometrySmith(N,V,L,roughness);
		vec3 F=FresnelSchlick(max(dot(H,V),0.),F0);
		
		vec3 nominator=NDF*G*F;
		float denominator=4*max(dot(N,V),0.)*max(dot(N,L),0.)+.001;
		vec3 specular=nominator/denominator;
		
		vec3 kS=F;
		
		vec3 kD=vec3(1.)-kS;
		
		kD*=1.-metallic;
		
		float NdotL=max(dot(N,L),0.);
		
		Lo+=(kD*albedo/PI+specular)*radiance*NdotL;
	}
	
	vec3 ambient=vec3(.03)*albedo*ao;
	
	vec3 color=ambient+Lo;
	
	color=color/(color+vec3(1.));
	color=pow(color,vec3(1./2.2));
	
	FragColor=vec4(color,1.);
}