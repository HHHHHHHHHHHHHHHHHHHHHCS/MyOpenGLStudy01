#version 460 core

out vec4 FragColor;

void main()
{
	FragColor=vec4(1.,1.,1.,1.);
	
	if(gl_FragCoord.x<400)
	FragColor*=.5;
	if(gl_FragCoord.y<400)
	FragColor*=.25;
	
}