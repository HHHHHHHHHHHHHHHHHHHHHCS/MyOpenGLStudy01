#version 460 core

out vec4 FragColor;

in vec3 WorldPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan=vec2(.1591,.3183);//1/2pi  , 1/pi

vec2 SampleSphericalMap(vec3 v)
{
	//3D 坐标 反算 2D UV
	vec2 uv=vec2(atan(v.z,v.x),asin(v.y));
	uv*=invAtan;//-0.5,0.5
	uv+=.5;//0,1
	return uv;
}

void main()
{
	vec2 uv=SampleSphericalMap(normalize(WorldPos));
	vec3 color=texture(equirectangularMap,uv).rgb;
	
	FragColor=vec4(color,1.);
}