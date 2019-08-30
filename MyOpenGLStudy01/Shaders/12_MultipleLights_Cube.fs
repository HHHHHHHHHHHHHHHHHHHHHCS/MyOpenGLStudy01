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
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float cutOff;
	float outerCutOff;
};

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcBaseLight(vec3 atten,vec3 ambient,vec3 diffuse,vec3 specular);
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

//useData.x attenuation    useData.y diff    useData.z spec
vec3 CalcBaseLight(vec3 userData,vec3 ambientColor,vec3 diffuseColor,vec3 specularColor)
{
	float atten=userData.x;
	float diff=userData.y;
	float spec=userData.z;
	vec3 diffCol=texture(material.diffuse,TexCoords).rgb;
	vec3 ambient=ambientColor*diffCol;
	vec3 diffuse=diffuseColor*diff*diffCol;
	vec3 specular=specularColor*spec*texture(material.specular,TexCoords).rgb;
	return(ambient+diffuse+specular)*atten;
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-light.direction);
	//漫反射颜色
	float diff=max(dot(normal,lightDir),0.);
	//镜面光颜色
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	
	return CalcBaseLight(vec3(1,diff,spec),light.ambient,light.diffuse,light.specular);
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
	
	return CalcBaseLight(vec3(attenuation,diff,spec),light.ambient,light.diffuse,light.specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir=normalize(light.position-fragPos);
	//漫反射颜色
	float diff=max(dot(normal,lightDir),0.);
	//镜面光颜色
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	//衰减
	float theta=dot(lightDir,normalize(-light.direction.xyz));
	float epsilon=light.cutOff-light.outerCutOff;
	float attenuation=clamp((theta-light.outerCutOff)/epsilon,0.,1.);
	
	return CalcBaseLight(vec3(attenuation,diff,spec),light.ambient,light.diffuse,light.specular);
	
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
	result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
	
	fragColor=vec4(result,1.);
}