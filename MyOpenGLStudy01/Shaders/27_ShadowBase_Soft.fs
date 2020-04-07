#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 FragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords=FragPosLightSpace.xyz/FragPosLightSpace.w;
	//if outside far_plane that shadow is 0.0
	if(projCoords.z>1.)
	{
		return 0.;
	}
	// transform to [0,1] range
	projCoords=projCoords*.5+.5;
	float closestDepth=texture(shadowMap,projCoords.xy).r;
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
	vec3 viewDir=normalize(viewPos-fs_in.FragPos);
	vec3 halfWayDir=normalize(lightDir+viewDir);
	spec=pow(max(dot(normal,halfWayDir),0.),64.);
	vec3 specular=spec*lightColor;
	//shadow
	float shadow=ShadowCalculation(fs_in.FragPosLightSpace);
	vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
	
	FragColor=vec4(lighting,1.);
}