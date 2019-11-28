#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

#define None 0
#define Inversion 1
#define Grayscale 2
#define Sharpen 3

#define ColorMode Sharpen

void main()
{
	#if(ColorMode==Inversion)
	
	FragColor=texture(screenTexture,TexCoords);
	FragColor.rgb=1-FragColor.rgb;
	
	#elif(ColorMode==Grayscale)
	
	FragColor=texture(screenTexture,TexCoords);
	float average=dot(vec3(.2126,.7152,.0722),FragColor.rgb);
	FragColor.rgb=vec3(average,average,average);
	
	#elif(ColorMode==Sharpen)
	
	float x=1/300.0;//dFdx(TexCoords).x;
	float y=1/300.0;//dFdy(TexCoords).y;
	
	vec2 offsets[9]=vec2[](
		vec2(-x,y),// 左上
		vec2(0.f,y),// 正上
		vec2(x,y),// 右上
		vec2(-x,0.f),// 左
		vec2(0.f,0.f),// 中
		vec2(x,0.f),// 右
		vec2(-x,-y),// 左下
		vec2(0.f,-y),// 正下
		vec2(x,-y)// 右下
	);
	
	float kernel[9]=float[](
		-1,-1,-1,
		-1,9,-1,
		-1,-1,-1
	);
	
	vec3 sampleTex[9];
	for(int i=0;i<9;i++)
	{
		sampleTex[i]=vec3(texture(screenTexture,TexCoords.xy+offsets[i]));
	}
	vec3 col=vec3(0.);
	for(int i=0;i<9;i++)
	col+=sampleTex[i]*kernel[i];
	FragColor=vec4(col,1.);
	
	#endif
}