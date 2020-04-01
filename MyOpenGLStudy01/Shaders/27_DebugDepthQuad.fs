#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_Plane;
uniform float far_Plane;

// perspective 1/depth back to linear
float LinearizeDepth(float depth)
{
	float z=depth*2.-1.;//back to NDC
	return(2.*near_Plane*far_Plane)/(far_Plane+near_Plane-2*(far_Plane-near_Plane));
}

void main()
{
	float depthValue=texture(depthMap,TexCoords).r;
	// FragColor = vec4(vec3(LinearizeDepth(depth)/far_Plane),1.0);// perspective
	FragColor=vec4(vec3(depthValue),1.);//orthographic
}