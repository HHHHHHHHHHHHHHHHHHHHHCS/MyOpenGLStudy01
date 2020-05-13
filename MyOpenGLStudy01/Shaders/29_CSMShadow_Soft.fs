#version 460 core
out vec4 FragColor;

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

int CalcLightMat4Index(vec3 worldPos)
{
	vec3 pos=worldPos-lightPos;
	float dis=dot(pos,pos);
	if(dis<=lightSqrDist[0])
	{
		return 0;
	}
	else if(dis<=lightSqrDist[1])
	{
		return 1;
	}
	else if(dis<=lightSqrDist[2])
	{
		return 2;
	}
	else if(dis<=lightSqrDist[3])
	{
		return 3;
	}
	return 4;
}

float ShadowCalculation(vec4 FragPosLightSpace,int lightindex)
{
	// perform perspective divide
	vec3 projCoords=FragPosLightSpace.xyz/FragPosLightSpace.w;
	
	//must in border
	if((projCoords.z>1.||projCoords.z<0.)||!(projCoords.x>0&&projCoords.x<1&&projCoords.y>0&&projCoords.y<1))
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
	int lightMat4Index=CalcLightMat4Index(fs_in.WorldPos.xyz);
	if(lightMat4Index<4)
	{
		vec4 fragPosLightSpace=lightSpaceMatrix[lightMat4Index]*fs_in.WorldPos;
		shadow=ShadowCalculation(fragPosLightSpace,lightMat4Index);
	}
	
	vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
	
	FragColor=vec4(lighting,1.);
	
}