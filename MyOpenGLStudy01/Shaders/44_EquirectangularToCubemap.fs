#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan==vec3(.1591,.3183);// 1/pi

vec2 SamplerSphericalMap(vec3 v)
{
	vec2 uv=vec2(atan(v.z,v.x),asin(v.y));
	uv*=invAtan;
	uv+=.5;
	return uv;
}

void main()
{
	vec2 uv=SamplerSphericalMap(normalize(WorldPos));
	vec3 color=texture(equirectangularMap,uv).rgb;
	
	FragColor=vec4(color,1.);
}