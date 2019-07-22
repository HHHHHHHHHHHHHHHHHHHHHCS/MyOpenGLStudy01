#version 460 core

out vec4 fragColor;

in vec3 outColor;
in vec2 texcoord;

uniform sampler2D ourTexture;
uniform sampler2D secondaryTex;
uniform float mixVal = 0.2;

void main()
{
	fragColor=mix(texture(ourTexture,texcoord),texture(secondaryTex,vec2(1-texcoord.x,texcoord.y)),mixVal);//*vec4(outColor,1);
}