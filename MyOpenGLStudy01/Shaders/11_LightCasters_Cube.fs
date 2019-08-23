#version 460 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;//使用定向光就不需要了
	vec4 direction;//xyz表示方向 正常w表示是否是定向光
	float cutOff;//聚光灯内圈范围
	float outerCutOff;//聚光灯外圈衰减范围
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	
	// ambient
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	
	// diffuse
	vec3 norm=normalize(Normal);
	float attenuation=1;
	
	vec3 result;
	vec3 lightDir;
	
	if(light.direction.w==0)
	{
		lightDir=-light.direction.xyz;//这里取反是为了方便计算
	}
	else if(light.direction.w==1)
	{
		lightDir=light.position.xyz-FragPos;
		float distance=length(lightDir);
		attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	}
	else
	{
		lightDir=light.position-FragPos;
	}
	
	lightDir=normalize(lightDir);
	
	if(light.direction.w==2)
	{
		float theta=dot(lightDir,normalize(-light.direction.xyz));
		float epsilon=light.cutOff-light.outerCutOff;
		attenuation=clamp((theta-light.outerCutOff)/epsilon,0.,1.);
	}
	
	lightDir=normalize(lightDir);
	float diff=max(dot(norm,lightDir),0.);
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	vec3 specular=light.diffuse*spec*texture(material.specular,TexCoords).r;
	
	//使用环境光 不至于全部黑暗
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	
	result=ambient+diffuse+specular;
	
	fragColor=vec4(result,1.);
}