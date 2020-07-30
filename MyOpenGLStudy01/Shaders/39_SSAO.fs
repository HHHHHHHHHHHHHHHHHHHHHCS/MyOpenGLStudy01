#version 460 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

//参数 (可以通过 uniform 传入  这里偷懒)
int kernelSize=64;
float radius=.5;
float bias=.025;

//屏幕尺寸/噪音大小的块   用于UV GL_REPEAT (可以外部传入 这里还是偷懒)
const vec2 noiseScale=vec2(1280./4.,720./4.);

uniform mat4 projeciton;

void main()
{
	//准备ssao 参数
	vec3 fragPos=texture(gPosition,TexCoords).xyz;
	vec3 normal=normalize(texture(gNormal,TexCoords).rgb);
	//4*4的噪音块缩放 到屏幕  uv repeat
	vec3 randomVec=normalize(texture(texNoise,TexCoords*noiseScale).xyz);
	
	//创建TBN  切线到屏幕空间
	vec3 tangent=normalize(randomVec-normal*dot(randomVec,normal));
	vec3 bitangent=cross(normal,tangent);
	mat3 TBN=mat3(tangent,bitangent,normal);
	
	//迭代样本核并计算遮挡因子
	float occlusion=0.;
	for(int i=0;i<kernelSize;++i)
	{
		//得到采样的新位置
		vec3 sampleOffset=TBN*samples[i];//tangent -> view space
		sampleOffset=fragPos+sampleOffset*radius;
		
		vec4 offset=vec4(sampleOffset,1.);
		offset=projeciton*offset;//view -> clipPos
		offset.xyz/=offset.w;//透视摄像机 齐次坐标对齐
		offset.xyz=offset.xyz*.5+.5;//cliPos->uv => [-1,1] -> [0,1]
		
		float sampleDepth=texture(gPosition,offset.xy).z;//得到偏移后深度
		
		//是否在检测半径里面
		float rangeCheck=smoothstep(0.,1.,radius/abs(fragPos.z-sampleDepth));
		//如果在偏移内是被遮挡的  则是ao   越远效果越不明显
		occlusion+=(sampleDepth>=sampleOffset.z+bias?1.:0.)*rangeCheck;
	}
	
	//遮挡的平均值
	occlusion=1.-(occlusion/kernelSize);
	
	FragColor=occlusion;
}