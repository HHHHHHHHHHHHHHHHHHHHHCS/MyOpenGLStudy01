#version 460 core

out vec4 fragColor;

//uniform vec3 objectColor;
//uniform vec3 lightColor;

void main()
{
	fragColor = vec4(1,0,0,1);//vec4(lightColor * objectColor, 1.0);
}