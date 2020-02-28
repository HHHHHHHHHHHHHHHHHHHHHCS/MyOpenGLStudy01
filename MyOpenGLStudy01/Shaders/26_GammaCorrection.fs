#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
}fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 cameraPos;
uniform bool gammaCorrection;

vec3 BlinnPhong(vec3 normal,vec3 fragPos,vec3 lightPos,vec3 lightColor)
{
	//diffuse
	vec3 lightDir=normalize(lightPos-fragPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*lightColor;
	//specular
	vec3 viewDir=normalize(cameraPos-fragPos);
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=0.;
	vec3 halfwayDir=normalize(lightDir+viewDir);
	spec=pow(max(dot(normal,halfwayDir),0.),64.);
	vec3 specular=spec*lightColor;
	//simple attenuation
	float max_distance=1.5;
	float distance=length(lightPos-fragPos);
	//线性变回gammma  需要 pow(1/2.2)   所以亮度需要提前 pow(1/distance,2.2)
	//这类似于  1/(distance*distance)
	//但是多盏灯的时候 linear 会比 gamma亮
	//因为lighting(亮度)那边是累加的
	float attenuation=1./(gammaCorrection?distance*distance:distance);
	
	return(diffuse+specular)*attenuation;
}

void main()
{
	vec3 color=texture(floorTexture,fs_in.Texcoords).rgb;
	vec3 lighting=vec3(0.);
	for(int i=0;i<4;++i)
	{
		lighting+=BlinnPhong(normalize(fs_in.Normal),fs_in.FragPos,lightPositions[i],lightColors[i]);
	}
	color*=lighting;
	if(gammaCorrection)
	{
		//因为传进来的颜色是线性 这边重新变回gamma
		color=pow(color,vec3(1./2.2));
	}
	FragColor=vec4(color,1.);
}