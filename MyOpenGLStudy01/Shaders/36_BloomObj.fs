#version 460 core
layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 BrightColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

struct Light
{
	vec3 Position;
	vec3 Color;
};

uniform Light lights[4];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main()
{
	vec3 color=texture(diffuseTexture,fs_in.TexCoords).rgb;
	vec3 normal=normalize(fs_in.Normal);
	
	//ambient
	vec3 ambient=0.*color;
	vec3 viewDir=normalize(viewPos-fs_in.FragPos);
	vec3 lighting = vec3(0);
	for(int i=0;i<4;i++)
	{
		//diffuse
		vec3 lightDir=normalize(lights[i].Position-fs_in.FragPos);
		float diff=max(dot(lightDir,normal),0.);
		vec3 result=lights[i].Color*diff*color;
		//attenuation
		float distance=length(fs_in.FragPos-lights[i].Position);
		result*=1./(distance*distance);
		lighting+=result;
	}
	
	vec3 result=ambient+lighting;
	float brightness=dot(result,vec3(.2126,.7152,.0722));
	if(brightness>1.)
	BrightColor=vec4(result,1.);
	else
	BrightColor=vec4(0.,0.,0.,1.);
	FragColor=vec4(result,1.);
	
}