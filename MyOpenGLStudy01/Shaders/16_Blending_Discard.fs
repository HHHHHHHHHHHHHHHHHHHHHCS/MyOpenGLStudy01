#version 460 core
out vec4 FragColor;

uniform sampler2D grassTexture;

in vec2 TexCoords;

void main()
{
	vec4 texColor=texture(grassTexture,TexCoords);
	if(texColor.a<.1)
	{
		discard;
	}
	
	FragColor=texColor;
}