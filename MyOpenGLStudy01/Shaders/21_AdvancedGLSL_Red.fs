#version 460 core

out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
}fs_in;

void main()
{
	FragColor=vec4((fs_in.TexCoords.x+.fs_inTexCoords.y)/2.,0.,0.,1.);
}