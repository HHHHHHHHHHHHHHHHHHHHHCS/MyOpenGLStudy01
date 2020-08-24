#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{
	vec3 envColor=texture(environmentMap,WorldPos).rgb;
	
	//HDR Tonemap and gamma correct
	envColor=envColor/(envColor+vec3(1.));
	envColor=pow(envColor,vec3(1./2.2));
	
	FragColor=vec4(envColor,1.);
}