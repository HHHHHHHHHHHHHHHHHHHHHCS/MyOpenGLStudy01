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

//Beckmann Distribution   高光用的GGX
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
//几何函数 Schlick   漫反射GGX
//---------------
float GeometrySchlickGGX(float NdotV,float roughness)
{
	float r=(roughness+1.);
	float k=(r*r)/8.;
	
	float nom=NdotV;
	float denom=NdotV*(1.-k)+k;
	
	return nom/denom;
}

//细微遮挡模拟
//--------------------
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness)
{
	float NdotV=max(dot(N,V),0.);
	float NdotL=max(dot(N,L),0.);
	float ggx2=GeometrySchlickGGX(NdotV,roughness);
	float ggx1=GeometrySchlickGGX(NdotL,roughness);
	
	return ggx1*ggx2;
}
//Fresnel
//-------------------
vec3 FresnelSchlick(float cosTheta,vec3 F0)
{
	return F0+(1.-F0)*pow(1.-cosTheta,5.);
}
//-------------------
void main()
{
	vec3 N=normalize(Normal);
	vec3 V=normalize(cameraPos-WorldPos);
	
	vec3 F0=vec3(.04);
	F0=mix(F0,albedo,metallic);
	
	vec3 Lo=vec3(0.);
	for(int i=0;i<4;++i)
	{
		//Light
		//------------------------
		//计算每一个灯光的衰减
		vec3 L=normalize(lightPositions[i]-WorldPos);
		vec3 H=normalize(V+L);
		float distance=length(lightPositions[i]-WorldPos);
		float attenuation=1./(distance*distance);
		vec3 radiance=lightColors[i]*attenuation;
		//End Light
		
		//高光
		//--------------------------------
		// Cook-Torrance BRDF
		float NDF=DistributionGGX(N,H,roughness);
		float G=GeometrySmith(N,V,L,roughness);
		vec3 F=FresnelSchlick(clamp(dot(H,V),0.,1.),F0);
		
		vec3 nominator=NDF*G*F;
		float denominator=4*max(dot(N,V),0.)*max(dot(N,L),0.);
		vec3 specular=nominator/max(denominator,.001);//防除以0
		//End 高光

		//漫反射
		//----------------------------
		//kS = F
		vec3 kS=F;
		//能量守恒
		//漫反射+高光反射 = 1
		vec3 kD=vec3(1.)-kS;
		
		//将kD乘以反金属度，得出非金属值
		//非金属值用于 漫反射照明，或线性混合
		//如果是金属  则没有漫反射
		kD*=1.-metallic;
		//End 漫反射
		
		//最终光照
		//-----------------------
		float NdotL=max(dot(N,L),0.);
		
		//这里除掉了PI
		//KD 这里已经计算了菲尼尔
		//NdotL 这里用这个公式有点简单
		//(漫反射 + 高光反射) * 灯光衰减 * 直接的NL
		Lo+=(kD*albedo/PI+specular)*radiance*NdotL;
		//End 最终光照
	}
	
	//环境光照明  后面可以替换IBL
	vec3 ambient=vec3(.03)*albedo*ao;
	
	vec3 color=ambient+Lo;
	
	//HDR映射
	color=color/(color+vec3(1.));
	
	//gamma 校正
	color=pow(color,vec3(1./2.2));
	
	FragColor=vec4(color,1.);
}