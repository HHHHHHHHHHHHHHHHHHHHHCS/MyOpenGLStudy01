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
	float sinTheta=sqrt(1.-cosTheta*cosTheta);
	
	vec3 H;
	H.x=cos(phi)*sinTheta;
	H.y=sin(phi)*sinTheta;
	H.z=cosTheta;
	
	vec3 up=abs(N.z)<.999?vec3(0.,0.,1.):vec3(1.,0.,0.);
	vec3 tangent=normalize(cross(up,N));
	vec3 bitangent=cross(N,tangent);
	
	vec3 sampleVec=tangent*H.x+bitangent*H.y+N*H.z;
	return normalize(sampleVec);
}

void main()
{
	vec3 N=normalize(WorldPos);
	
	vec3 R=N;
	vec3 V=R;
	
	const uint SAMPLE_COUNT=1024u;
	vec3 prefilterColor=vec3(0.);
	float totalWeight=0.;
	
	for(uint i=0u;i<SAMPLE_COUNT;++i)
	{
		vec2 Xi=Hammersley(i,SAMPLE_COUNT);
		vec3 H=ImportanceSampleGGX(Xi,N,roughness);
		vec3 L=normalize(2.*dot(V,H)*H-V);
		
		float NdotL=max(dot(N,L),0.);
		if(NdotL>0.)
		{
			//一种解决方案是进一步增加样本数量，但在某些情况下还是不够。
			//另一种方案如 Chetan Jags 所述，我们可以在预过滤卷积时，不直接采样环境贴图，而是基于积分的 PDF 和粗糙度采样环境贴图的 mipmap ，以减少伪像：
			float D=DistributionGGX(N,H,roughness);
			float NdotH=max(dot(N,H),0.);
			float NdotV=max(dot(H,V),0.);
			float pdf=D*NdotH/(4.*HdotV)+.0001;
			
			float resolution=512.;
			float saTexel=4.*PI/(6.*resolution*resolution);
			float saSample=1./(float(SAMPLE_COUNT)*pdf+.0001);
			
			float mipLevel=roughness==0.?0.:.5*log2(saSample/saTexel);
			
			prefilterColor+=textureLod(environmentMap,L,mipLevel).rgb*NdotL;
			totalWeight+=NdotL;
		}
	}
	
	prefilterColor=prefilterColor/totalWeight;
	
	FragColor=vec4(prefilterColor,1.);
}