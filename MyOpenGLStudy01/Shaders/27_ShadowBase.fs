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
	// transform to [0,1] range
	projCoords=projCoords*.5+.5;
	float closestDepth=texture(shadowMap,projCoords.xy).r+.01;//0.01用做阴影bias
	float currentDepth=projCoords.z;
	float shadow=currentDepth>closestDepth?1.:0.;
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