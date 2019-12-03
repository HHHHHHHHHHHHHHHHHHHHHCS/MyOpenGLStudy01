#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

#define None 0
#define Inversion 1
#define Grayscale 2
#define Sharpen 3
#define Blur 4
#define EdgeDetection 5

#define ColorMode EdgeDetection

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
	
	float x=1/300.;
	float y=1/300.;
	
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
	
	#elif(ColorMode==Blur)
	
	float x=1/300.;
	float y=1/300.;
	
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
	
	float kernel[9]=float[]
	(
		1./16,2./16,1./16,
		2./16,4./16,2./16,
		1./16,2./16,1./16
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
	
	#elif(ColorMode==EdgeDetection)
	
	float x=dFdx(TexCoords).x;
	float y=dFdy(TexCoords).y;
	
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
	
	float kernel[9]=float[]
	(
		1.,1.,1.,
		1.,-8.,1.,
		1.,1.,1.
	);
	
	vec3 sampleTex[9];
	for(int i=0;i<9;i++)
	{
		sampleTex[i]=vec3(texture(screenTexture,TexCoords.xy+offsets[i]));
	}
	
	vec3 col=vec3(0.);
	for(int i=0;i<9;i++)
	col+=sampleTex[i]*kernel[i];
	FragColor=vec4(clamp(col,0.,1.),1.);
	
	#endif
}