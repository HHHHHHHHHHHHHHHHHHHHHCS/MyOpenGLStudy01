#version 460 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	//vec3 position;//使用定向光就不需要了
	vec4 direction;// 正常w表示是否是定向光
	
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
	vec3 lightDir=normalize(-light.direction);//这里取反是为了方便计算
	float diff=max(dot(norm,lightDir),0.);
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	vec3 specular=light.diffuse*spec*texture(material.specular,TexCoords).r;
	
	vec3 result=ambient+diffuse+specular;
	
	float distance=length(light.position-FragPos);
	float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	
	fragColor=vec4(result,1.);
}