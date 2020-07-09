#version 460 core
layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 BrightColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform vec3 lightColor;

void main()
{
	FragColor=vec4(lightColor,1.);
	
	float brightness=dot(FragColor.rgb,vec3(.2126,.7152,.0722));
	if(brightness>1.)
	BrightColor=vec4(FragColor.rgb,1.);
	else
	BrightColor=vec4(0.,0.,0.,1.);
	
}