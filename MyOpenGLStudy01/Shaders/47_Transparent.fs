#version 460 core

layout(location=0)out vec4 accum;
layout(location=1)out float reveal;

uniform vec4 Color;

void main()
{
	float weight=clamp(pow(min(1.,Color.a*10.)+.01,3.)*1e8*pow(1.-gl_FragCoord.z*.9,3.),1e-2,3e3);
	
	accum=vec4(Color.rgb*Color.a,Color.a)*weight;
	
	reveal=Color.a;
}
