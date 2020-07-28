#version 460 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPostion;
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
	vec3 fragPos=texture(gPostion,TexCoords).xyz;
	vec3 normal=normalize(texture(gNormal,TexCoords).rgb);
	//4*4的噪音块缩放 到屏幕  uv repeat
	vec3 randomVec=normalize(texture(texNoise,TexCoords*noiseScale).xyz);
	
	//创建TBN  切线到屏幕空间
	vec3 tangent=normalize(randomVec-normal*dot(randomVec,normal));
	vec3 bitangent=cross(normal,tangent);
	mat3 TBN=mat3(tangent,bitangent,normal);

	//迭代样本核并计算遮挡因子
	//TODO:
}