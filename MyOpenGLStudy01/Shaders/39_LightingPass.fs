#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light
{
	vec3 Position;
	vec3 Color;
	
	float Linear;
	float Quadratic;
};

uniform Light light;

void main()
{
	//从gbuffer 读取数据
	vec3 FragPos=texture(gPosition,TexCoords).rgb;
	vec3 Normal=texture(gNormal,TexCoords).rgb;
	vec3 Diffuse=texture(gAlbedo,TexCoords).rgb;
	//ssao 遮挡
	float AmbientOcclusion=texture(ssao,TexCoords).r;
	
	//计算灯光
	vec3 ambient=vec3(.3*Diffuse*AmbientOcclusion);
	vec3 lighting=ambient;
	vec3 viewDir=normalize(-FragPos);//因为在view空间 viewPos 是(0,0,0)
	//diffuse light 也在view空间
	vec3 lightDir=normalize(light.Position-FragPos);
	vec3 diffuse=max(dot(Normal,lightDir),0.)*Diffuse*light.Color;
	//specular
	vec3 halfwayDir=normalize(lightDir+viewDir);
	float spec=pow(max(dot(Normal,halfwayDir),0.),8.);
	vec3 specular=light.Color*spec;
	//attenuation
	float distance=length(light.Position-FragPos);
	float attenuation=1./(1.+light.Linear*distance+light.Quadratic*distance*distance);
	diffuse*=attenuation;
	specular*=attenuation;
	lighting+=diffuse+specular;
	
	FragColor=vec4(lighting,1.);
}