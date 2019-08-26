#version 460 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS]

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-light.direction);
	//漫反射颜色
	float diff=max(dot(normal,lightDir),0.);
	//镜面光颜色
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	//合并结果
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;
	return(ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir=normalize(light.position-fragPos);
	//漫反射颜色
	float diff=max(dot(normal,lightDir),0.);
	//镜面光颜色
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	//衰减
	float distance=length(light.position-fragPos);
	float attenuation=1./(light.constant+light.linear*distance+light.quadratic*distance*distance);
	//合并结果
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	return(ambient+diffuse+specular);
}

void main()
{
	//属性
	vec3 norm=normalize(Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	
	//先计算定向光
	vec3 result=CalcDirLight(dirLight,norm,viewDir);
	//计算点光
	for(int i=0;i<NR_POINT_LIGHTS;i++)
	{
		result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
	}
	//计算聚光灯
	result+=CalcSpotLight(SpotLight,norm,FragPos,viewDir);
	
	fragColor=vec4(result,1.);
}