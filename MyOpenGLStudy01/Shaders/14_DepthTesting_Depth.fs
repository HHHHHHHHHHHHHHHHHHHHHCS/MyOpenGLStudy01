#version 460 core

out vec4 FragColor;

const float near=.1;
const float far=100.;

float LinearizeDepth(float depth)
{
	float z=depth*2.-1.;//从[0,1]变成[-1,1]
	return(2.*near*far)/(far+near-z*(far-near));//[near,fars]
}

void main()
{
	//gl_FragCoord从片段着色器中直接访问。
	//gl_FragCoord的x和y分量代表了片段的屏幕空间坐标（其中(0, 0)位于左下角）。
	//gl_FragCoord中也包含了一个z分量，它包含了片段真正的深度值。
	//但是这时候的深度值是非线性的  是 1/z 的
	//FragColor=vec4(vec3(gl_FragCoord.z),1.);
	float depth=LinearizeDepth(gl_FragCoord.z)/far;//除以far 让他回归到[0,1]
	FragColor=vec4(vec3(depth),1.);
}