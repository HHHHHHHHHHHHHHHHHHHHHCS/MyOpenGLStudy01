#version 460 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main()
{
	//textureSize 后面的int 是lod
	vec2 texelSize=1./vec2(textureSize(ssaoInput,0));
	
	float result=0.;
	
	for(int x=-2;x<2;++x)
	{
		for(int y=-2;y<2;++y)
		{
			vec2 offset=vec2(float(x),float(y))*texelSize;
			result+=texture(ssaoInput,TexCoords+offset).r;
		}
	}
	
	FragColor=result/(4.*4.);
}