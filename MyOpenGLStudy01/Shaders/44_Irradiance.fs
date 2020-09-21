#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI=3.14159265359;

void main()
{
	vec3 N=normalize(WorldPos);
	
	vec3 irradiance=vec3(0.);
	
	vec3 up=vec3(0.,1.,0.);
	vec3 right=cross(up,N);
	up=cross(N,right);
	
	float sampleDelta=.025;
	float nrSamples=0.f;
	
	for(float phi=0.;phi<2.*PI;phi+=sampleDelta)
	{
		for(float theta=0.;theta<.5*PI;theta+=sampleDelta)
		{
			//tangent space normal
			vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			//tangent space to world
			vec3 sampleVec=tangentSample.x*right+tangentSample.y*up+tangentSample.z*N;
			
			irradiance+=texture(environmentMap,sampleVec).rgb*cos(theta)*sin(theta);
			nrSamples++;
		}
	}
	irradiance=PI*irradiance*(1./float(nrSamples));
	
	FragColor=vec4(irradiance,1.);
}