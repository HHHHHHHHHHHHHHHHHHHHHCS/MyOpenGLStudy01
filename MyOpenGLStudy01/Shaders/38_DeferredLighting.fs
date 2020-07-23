#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
	vec3 Position;
	vec3 Color;
	
	float Linear;
	float Quadratic;
	float Radius;
};

const int NR_LIGHTS=32;

uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
	vec3 FragPos=texture(gPosition,TexCoords).rgb;
	vec3 Normal=texture(gNormal,TexCoords).rgb;
	vec3 Diffuse=texture(gAlbedoSpec,TexCoords).rgb;
	float Specular=texture(gAlbedoSpec,TexCoords).a;
	
	vec3 lighting=Diffuse*.1;
	vec3 viewDir=normalize(viewPos-FragPos);
	for(int i=0;i<NR_LIGHTS;++i)
	{
		float distance=length(lights[i].Position-FragPos);
		//在GPU 可能也是假的分支执行
		if(distance<lights[i].Radius)
		{
			//diffuse
			vec3 lightDir=normalize(lights[i].Position-FragPos);
			vec3 diffuse=max(dot(Normal,lightDir),0.)*Diffuse*lights[i].Color;
			//specular
			vec3 halfWayDir=normalize(lightDir+viewDir);
			float spec=pow(max(dot(Normal,halfWayDir),0.),16.);
			vec3 specular=lights[i].Color*spec*Specular;
			//attenuation
			float attenuation=1./(1.+lights[i].Linear*distance+lights[i].Quadratic*distance*distance);
			diffuse*=attenuation;
			specular*=attenuation;
			lighting+=diffuse+specular;
		}
		
	}
	FragColor=vec4(lighting,1.);
}
