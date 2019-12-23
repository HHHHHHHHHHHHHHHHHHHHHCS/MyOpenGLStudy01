#version 460 core
layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;

in VS_OUT
{
	vec2 texCoords;
}gs_in[];

out vec2 TexCoords;

uniform float time;

vec4 Explode(vec4 position,vec3 normal)
{
	float magnitude=2.;
	vec3 direction=normal*((sin(time)+1.)/2.)*magnitude;
	return position+vec4(direction,0.);
}

vec3 GetNormal()
{
	vec3 a=gl_in[0].gl_Position.xyz-gl_in[1].gl_Position.xyz;
	vec3 b=gl_in[2].gl_Position.xyz-gl_in[1].gl_Position.xyz;
	return normalize(cross(a,b));
}

void main()
{
	vec3 normal=GetNormal();
	
	gl_Position=Explode(gl_in[0].gl_Position,normal);
	TexCoords=gs_in[0].texCoords;
	EmitVertex();
	gl_Position=Explode(gl_in[1].gl_Position,normal);
	TexCoords=gs_in[1].texCoords;
	EmitVertex();
	gl_Position=Explode(gl_in[2].gl_Position,normal);
	TexCoords=gs_in[2].texCoords;
	EmitVertex();
	EndPrimitive();
}