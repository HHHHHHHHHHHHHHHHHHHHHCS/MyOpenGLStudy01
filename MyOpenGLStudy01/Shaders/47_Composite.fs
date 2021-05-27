#version 460 core

layout(location=0)out vec4 frag;

//binding 这样就不能用  texture了
layout(binding=0)uniform sampler2D accum;

layout(binding=1)uniform sampler2D reveal;

const float EPSILON=.00001f;

bool IsApproximatelyEqual(float a,float b)
{
	return abs(a-b)<=(abs(a)<abs(b)?abs(a):abs(b))*EPSILON;
}

float Max3(vec3 v)
{
	return max(max(v.x,v.y),v.z);
}

void main()
{
	ivec2 coords=ivec2(gl_FragCoord.xy);
	
	float revealage=texelFetch(reveal,coords,0).r;
	
	if(IsApproximatelyEqual(revealage,1.f))
	{
		discard;
	}
	
	vec4 accumulation=texelFetch(accum,coords,0);
	
	if(isinf(Max3(abs(accumulation.rgb))))
	{
		accumulation.rgb=vec3(accumulation.a);
	}
	
	vec3 average_color=accumulation.rgb/max(accumulation.a,EPSILON);
	
	frag=vec4(average_color,1.f-revealage);
}