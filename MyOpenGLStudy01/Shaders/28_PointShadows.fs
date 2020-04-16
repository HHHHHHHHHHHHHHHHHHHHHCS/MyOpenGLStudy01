#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCorrds;
}fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

float ShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight=fragPos-lightPos;
	float closestDepth=texture(depthMap,fragToLight).r;
	closestDepth*=far_plane;
	float currentDepth=length(fragToLight);
	float bias=.05;
	float shadow=currentDepth-bias>closestDepth?1.:0.;
	
	return shadow;
}

void main()
{
	vec3 color=texture(diffuseTexture,fs_in.TexCorrds).rgb;
	vec3 normal=normalize(fs_in.Normal);
	vec3 lightColor=vec3(.3);
	// ambient
	vec3 ambient=.3*color;
	// diffuse
	vec3 lightDir=normalize(lightPos-fs_in.FragPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*lightColor;
	//specular
	vec3 vecDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflectDir-lightDir,normal);
	//TODO:
}