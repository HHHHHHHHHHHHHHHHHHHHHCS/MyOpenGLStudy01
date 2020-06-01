#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec3 aTexCoords;
layout(location=3)in vec3 aTangent;
layout(location=4)in vec3 aBitangent;

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}vs_out;

uniform mat4 viewProjection;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec4 worldPos=model*vec4(aPos,1.);
	vs_out.FragPos=worldPos.xyz;
	vs_out.TexCoords=aTexCoords;
	
	//当多顶点共享数据的时候 , TBN可能不会互相垂直 ,
	//这样TBN 就不是正交矩阵了 , 导致法线贴图偏移
	//格拉姆-施密特正交化 让TBN 重新正交垂直
	mat3 normalMatrix=transpose(inverse(mat3(model)));
	vec3 T=normalize(normalMatrix*aTangent);
	vec3 N=normalize(normalMatrix*aNormal);
	T=normalize(T-dot(T,N)*N);
	vec3 B=cross(N,T);
	
	//TBN 原来是 切线空间 -> 对象空间
	//由于TBN是正交矩阵，所以求TBN的逆等价于求它的转置。
	//这种方法 对比 把TBN 放到像素阶段求法线的好处是   顶点运算少 节约运算数
	mat3 TBN=transpose(mat3(T,B,N));
	vs_out.TangentLightPos=TBN*lightPos;
	vs_out.TangentViewPos=TBN*viewPos;
	vs_out.TangentFragPos=TBN*vs_out.FragPos;
	
	gl_Position=viewProjection*worldPos;
	
}