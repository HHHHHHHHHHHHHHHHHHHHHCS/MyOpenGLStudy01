#version 460 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	mat3 FSTBN;
}fs_in;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shininess;
};

uniform Material material;

uniform bool haveNormal;
uniform bool inVSTBN;

void main()
{
	vec3 normal;
	if(haveNormal)
	{
		//get normal in [0,1]
		normal=texture(material.texture_normal1,fs_in.TexCoords).rgb;
		//transform normal to [-1,1]
		normal=normalize(normal*2.-1.);//this is in tangent space
		if(!inVSTBN)
		{
			normal=fs_in.FSTBN*normal;
		}
	}
	else
	{
		normal=fs_in.FSTBN[0];
	}
	
	vec3 color=texture(material.texture_diffuse1,fs_in.TexCoords).rgb;
	vec3 ambient=.1*color;
	
	vec3 lightDir=normalize(fs_in.TangentLightPos-fs_in.TangentFragPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*color;
	
	vec3 viewDir=normalize(fs_in.TangentViewPos-fs_in.TangentFragPos);
	//vec3 reflectDir=reflect(-lightDir,normal);
	vec3 halfwayDir=normalize(lightDir+viewDir);
	float spec=pow(max(dot(normal,halfwayDir),0.),material.shininess);
	vec3 specular=spec*texture(material.texture_specular1,fs_in.TexCoords).rgb;
	
	FragColor=vec4(ambient+diffuse+specular,1.);
	
}