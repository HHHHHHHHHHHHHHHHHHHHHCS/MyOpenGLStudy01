#version 460 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D parallaxMap;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir)
{
	float height=texture(parallaxMap,texCoords).r;
	return texCoords-viewDir.xy*(height*heightScale);
}

void main()
{
	vec3 viewDir=normalize(fs_in.TangentViewPos-fs_in.TangentFragPos);

	vec2 texCoords=fs_in.TexCoords;
	
	texCoords=ParallaxMapping(texCoords,viewDir);

	//超出UV
	if(texCoords.x>1.||texCoords.y>1.||texCoords.x<0.||texCoords.y<0.)
	discard;
	
	//get normal in [0,1]
	vec3 normal=texture(normalMap,texCoords).rgb;
	//transform normal to [-1,1]
	normal=normalize(normal*2.-1.);//this is in tangent space
	
	vec3 color=texture(diffuseMap,texCoords).rgb;
	vec3 ambient=.1*color;
	
	vec3 lightDir=normalize(fs_in.TangentLightPos-fs_in.TangentFragPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*color;
	
	vec3 halfwayDir=normalize(lightDir+viewDir);
	float spec=pow(max(dot(normal,halfwayDir),0.),32.);
	vec3 specular=vec3(.2)*spec;
	
	FragColor=vec4(ambient+diffuse+specular,1.);
	
}