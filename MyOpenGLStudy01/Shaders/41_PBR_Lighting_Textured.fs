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

uniform float metallicIntensity;
uniform float roughnessIntensity;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform int lightCount;

uniform vec3 cameraPos;

const float PI=3.14159265359;

vec3 GetNormalFromMap()
{
	vec3 tangentNormal=texture(normalMap,TexCoords).xyz*2.-1.;
	
	//正常的渲染是2*2的去渲染  可拿到相邻的局部变量的值  可以求出相邻坐标的局部变量的变化率
	vec3 Q1=dFdx(WorldPos);
	vec3 Q2=dFdy(WorldPos);
	vec2 st1=dFdx(TexCoords);
	vec2 st2=dFdy(TexCoords);
	
	//简单的技巧，使切线法线到世界空间保持简化PBR代码。
	//不然要传入 tangent
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

void main()
{
	vec3 albedo=pow(texture(albedoMap,TexCoords).rgb,vec3(2.2));
	float metallic=texture(metallicMap,TexCoords).r*metallicIntensity;
	float roughness=texture(roughnessMap,TexCoords).r*roughnessIntensity;
	float ao=texture(aoMap,TexCoords).r;
	
	vec3 N=GetNormalFromMap();
	vec3 V=normalize(cameraPos-WorldPos);
	
	//菲尼尔颜色
	vec3 F0=vec3(.04);
	F0=mix(F0,albedo,metallic);
	
	vec3 Lo=vec3(0.);
	
	for(int i=0;i<lightCount;++i)
	{
		vec3 L=normalize(lightPositions[i]-WorldPos);
		vec3 H=normalize(V+L);
		float distance=length(lightPositions[i]-WorldPos);
		float attenuation=1./(distance*distance);
		vec3 radiance=lightColors[i]*attenuation;
		
		//Cook-Torrance BRDF
		float NDF=DistributionGGX(N,H,roughness);
		float G=GeometrySmith(N,V,L,roughness);
		vec3 F=FresnelSchlick(max(dot(H,V),0.),F0);
		
		vec3 nominator=NDF*G*F;
		float denominator=4*max(dot(N,V),0.)*max(dot(N,L),0.)+.001;//避免除以0
		vec3 specular=nominator/denominator;
		
		//diffuse
		vec3 ks=F;
		//漫反射颜色 + 菲尼尔颜色 = 1
		vec3 kd=vec3(1.)-ks;
		//金属度越低  漫反射越强
		kd*=1.-metallic;
		
		float NdotL=max(dot(N,L),0.);
		
		Lo+=(kd*albedo/PI+specular)*radiance*NdotL;
	}
	
	vec3 ambient=vec3(.03)*albedo*ao;
	
	vec3 color=ambient+Lo;
	
	//HDR tonemapping
	color=color/(color+vec3(1.));
	//gamma 校正
	color=pow(color,vec3(1./2.2));
	
	FragColor=vec4(color,1.);
	
}