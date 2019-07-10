#version 460 core

out vec4 fragColor;

in vec3 outColor;
in vec2 texcoord;

uniform sampler2D ourTexture;
uniform sampler2D secondaryTex;

void main()
{
	fragColor=mix(texture(ourTexture,texcoord),texture(secondaryTex,texcoord),1.0)*vec4(outColor,1);
}