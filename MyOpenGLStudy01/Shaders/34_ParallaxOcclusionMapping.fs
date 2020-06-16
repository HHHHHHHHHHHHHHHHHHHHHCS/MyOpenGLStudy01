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
	const float minLayers=8;
	const float maxLayer=32;
	//如果视线和平面的角度越垂直 则 layer越小
	float numLayers=mix(maxLayer,minLayers,abs(dot(vec3(0.,0.,1.),viewDir)));
	//计算每层的深度
	float layerDepth=1./numLayers;
	//每层的移动量
	vec2 P=viewDir.xy/viewDir.z*heightScale;
	vec2 deltaTexCoords=P/numLayers;
	
	//init values
	float currentLayerDepth=0.;
	vec2 currentTexCoords=texCoords;
	float currentDepthMapValue=texture(parallaxMap,currentTexCoords).r;
	
	while(currentLayerDepth<currentDepthMapValue)
	{
		currentTexCoords-=deltaTexCoords;
		currentDepthMapValue=texture(parallaxMap,currentTexCoords).r;
		currentLayerDepth+=layerDepth;
	}
	
	// 碰撞前获取纹理坐标（反向操作）
	vec2 prevTexCoords=currentTexCoords+deltaTexCoords;
	
	// 线性插值的碰撞前后深度
	float afterDepth=currentDepthMapValue-currentLayerDepth;
	float beforeDepth=texture(parallaxMap,prevTexCoords).r-currentLayerDepth+layerDepth;
	
	// 线性插值的碰撞前后深度
	float weight=afterDepth/(afterDepth-beforeDepth);
	vec2 finalTexCoords=prevTexCoords*weight+currentTexCoords*(1.-weight);
	
	return finalTexCoords;
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