#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	vec3 col=texture(screenTexture,TexCoords).rgb;
	float grayscale=.2126*col.r+.7152*col.g+.0722*col.b;
	FragColor=vec4(vec3(grayscale),1.);
}