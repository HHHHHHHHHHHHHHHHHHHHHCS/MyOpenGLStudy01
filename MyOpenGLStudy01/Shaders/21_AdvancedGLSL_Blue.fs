#version 460 core

out vec4 FragColor;

void main()
{
	if(gl_FrontFacing)
	FragColor=vec4(0.,1.,0.,1.);
	else
	FragColor=vec4(0.,.5,0.,1.);
}