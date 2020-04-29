#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool showShadows;
uniform int shadowsType;

// cube 的 八个角 和 12 条棱的中点
vec3 gridSamplingDisk[20]=vec3[]
(
	vec3(1,1,1),vec3(1,-1,1),vec3(-1,-1,1),vec3(-1,1,1),
	vec3(1,1,-1),vec3(1,-1,-1),vec3(-1,-1,-1),vec3(-1,1,-1),
	vec3(1,1,0),vec3(1,-1,0),vec3(-1,-1,0),vec3(-1,1,0),
	vec3(1,0,1),vec3(-1,0,1),vec3(1,0,-1),vec3(-1,0,-1),
	vec3(0,1,1),vec3(0,-1,1),vec3(0,-1,-1),vec3(0,1,-1)
);

float GridShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight=fragPos-lightPos;
	float currentDepth=length(fragToLight);
	
	float shadow=0.;
	float bias=.15;
	int samples=20;
	
	//越近采样跨度越小 越精细
	float diskRadius=(1.+(currentDepth/far_plane))/25.;
	
	for(int i=0;i<samples;++i)
	{
		float closestDepth=texture(depthMap,fragToLight+gridSamplingDisk[i]*diskRadius).r;
		closestDepth*=far_plane;
		if(currentDepth-bias>closestDepth)
		shadow+=1.;
	}
	
	shadow/=float(samples);
	
	//if debug use
	//FragColor = vec4(vec3(closestDepth/far_plane), 1.0)
	
	return shadow;
}

float PCFShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight=fragPos-lightPos;
	float currentDepth=length(fragToLight);
	
	float shadow=0.;
	float bias=.15;
	float samples=2.;
	float offset=.04;
	for(float x=-offset;x<=offset;x+=(2*offset)/samples)
	{
		for(float y=-offset;y<=offset;y+=(2*offset)/samples)
		{
			for(float z=-offset;z<=offset;z+=(2*offset)/(samples))
			{
				float closestDepth=texture(depthMap, fragToLight+vec3(x,y,z)).r;
				closestDepth*=far_plane;
				if(currentDepth-bias>closestDepth)
				shadow+=1.;
			}
		}
	}
	
	shadow/=(samples+1)*(samples+1)*(samples+1);
	
	return shadow;
}

float SampleShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight=fragPos-lightPos;
	float closestDepth=texture(depthMap,fragToLight).r;
	closestDepth*=far_plane;
	float currentDepth=length(fragToLight);
	float bias=.05;
	float shadow=currentDepth-bias>closestDepth?1.:0.;
	
	return shadow;
}

void main()
{
	vec3 color=texture(diffuseTexture,fs_in.TexCoords).rgb;
	vec3 normal=normalize(fs_in.Normal);
	vec3 lightColor=vec3(.3);
	// ambient
	vec3 ambient=.3*color;
	// diffuse
	vec3 lightDir=normalize(lightPos-fs_in.FragPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*lightColor;
	//specular
	vec3 viewDir=normalize(viewPos-fs_in.FragPos);
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=0.;
	vec3 halfwayDir=normalize(lightDir+viewDir);
	spec=pow(max(dot(normal,halfwayDir),0.),64.);
	vec3 specular=spec*lightColor;
	//calculate shadow
	float shadow=0;
	if(showShadows)
	{
		if(shadowsType==1)
		{
			shadow=SampleShadowCalculation(fs_in.FragPos);
		}
		else if(shadowsType==2)
		{
			shadow=PCFShadowCalculation(fs_in.FragPos);
		}
		else// if(shadowsType==3)
		{
			shadow=GridShadowCalculation(fs_in.FragPos);
		}
	}
	
	vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
	
	FragColor=vec4(lighting,1.);
}