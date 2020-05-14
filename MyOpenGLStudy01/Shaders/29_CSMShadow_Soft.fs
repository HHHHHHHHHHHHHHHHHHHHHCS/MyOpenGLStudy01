#version 460 core
out vec4 FragColor;

#define sat(x)clamp(ceil(x),0,1)

#define Fast 0

in VS_OUT
{
	vec3 Normal;
	vec2 TexCoords;
	vec4 WorldPos;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

uniform vec4 lightSqrDist;
uniform vec3 lightPos;
uniform mat4[4]lightSpaceMatrix;

vec4 CalcLightMat4Index_Complex(vec4 worldPos)
{
	vec4 fragPosLightSpace;
	vec3 projCoords;
	
	for(int i=0;i<4;++i)
	{
		fragPosLightSpace=lightSpaceMatrix[i]*worldPos;
		projCoords=fragPosLightSpace.xyz/fragPosLightSpace.w;
		if(projCoords.z<=0.99&&projCoords.z>=0.01&&projCoords.x>=0.01&&projCoords.x<=0.99&&projCoords.y>=0.01&&projCoords.y<=0.99)
		{
			return vec4(projCoords,i);
		}
	}
	
	return vec4(-1,-1,-1,-1);
}

int CalcLightMat4Index_Fast(vec3 worldPos)
{
	vec3 pos=worldPos-lightPos;
	float dis=dot(pos,pos);
	vec4 indexs=vec4(
		sat(lightSqrDist[0]-dis),
		sat(lightSqrDist[1]-dis),
		sat(lightSqrDist[2]-dis),
		sat(lightSqrDist[3]-dis)
	);
	
	indexs.yzw=indexs.yzw-indexs.xyz;
	
	return int(4.-dot(vec4(4.,3.,2.,1.),indexs));
}

float ShadowCalculation(vec4 FragPosLightSpace,int lightindex)
{
	#if Fast
	vec3 projCoords=FragPosLightSpace.xyz/FragPosLightSpace.w;
	#else
	vec3 projCoords=FragPosLightSpace.xyz;
	#endif
	
	//must in border
	if(!(projCoords.z<=1&&projCoords.z>=0&&projCoords.x>=0&&projCoords.x<=1&&projCoords.y>=0&&projCoords.y<=1))
	{
		return 0.;
	}
	
	projCoords.xy=projCoords.xy*.5;
	projCoords.x+=.5*(lightindex%2);
	projCoords.y+=.5*int(lightindex/2);
	
	//float closestDepth=texture(shadowMap,projCoords.xy).r;
	
	float currentDepth=projCoords.z;
	//calculate bias
	vec3 normal=normalize(fs_in.Normal);
	vec3 lightDir=lightPos;//normalize(lightPos);
	//dot(normal,lightDir)解决坡度过大出现的 阴影失真(Shadow Acne)
	float bias=max(.05*(1.-dot(normal,lightDir)),.005);
	//check in shadow
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//PCF
	float shadow=0.;
	vec2 texelSize=1./textureSize(shadowMap,0);
	for(int x=-1;x<=1;++x)
	{
		for(int y=-1;y<=1;++y)
		{
			float pcfDepth=texture(shadowMap,projCoords.xy+vec2(x,y)*texelSize).r;
			shadow+=currentDepth-bias>pcfDepth?1.:0.;
		}
	}
	
	shadow/=9.;
	
	return shadow;
}

void main()
{
	vec3 color=texture(diffuseTexture,fs_in.TexCoords).rgb;
	vec3 normal=normalize(fs_in.Normal);
	vec3 lightColor=vec3(.3);
	//ambient
	vec3 ambient=.3*color;
	//diffuse
	vec3 lightDir=lightPos;//normalize(lightPos);
	float diffuse=max(dot(lightDir,normal),0.);
	//spec
	float spec=0.;
	vec3 viewDir=normalize(viewPos-fs_in.WorldPos.xyz);
	vec3 halfWayDir=normalize(lightDir+viewDir);
	spec=pow(max(dot(normal,halfWayDir),0.),64.);
	vec3 specular=spec*lightColor;
	//shadow
	float shadow=0.;
	
	#if Fast
	int lightMat4Index=CalcLightMat4Index_Fast(fs_in.WorldPos.xyz);
	if(lightMat4Index<4)
	{
		vec4 fragPosLightSpace=lightSpaceMatrix[lightMat4Index]*fs_in.WorldPos;
		shadow=ShadowCalculation(fragPosLightSpace,lightMat4Index);
	}
	#else
	vec4 uvwIndex=CalcLightMat4Index_Complex(fs_in.WorldPos);
	shadow=ShadowCalculation(uvwIndex,int(uvwIndex.w));
	#endif
	
	vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
	
	FragColor=vec4(lighting,1.);
}