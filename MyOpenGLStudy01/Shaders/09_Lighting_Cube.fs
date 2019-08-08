#version 460 core

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	// ambient
	float ambientStrength=.1;
	vec3 ambient=ambientStrength*lightColor;
	
	// diffuse
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(lightPos-FragPos);
	float diff=max(dot(norm,lightDir),0.);
	
	vec3 diffuse=diff*lightColor;
	
	vec3 result=(ambient+diffuse)*objectColor;
	
	fragColor=vec4(result,1.);
}