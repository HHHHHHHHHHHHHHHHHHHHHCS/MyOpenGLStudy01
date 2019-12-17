#version 460 core

out vec4 FragColor;

/*

顶点着色器的输出向量gl_Position，和片段着色器的gl_FragCoord。
gl_FragDepth 只能在片元着色器下
如果着色器没有写入值到gl_FragDepth，它会自动取用gl_FragCoord.z的值

depth_<condition>
注意条件模式下 要GLSL Version>=420
any	默认值。提前深度测试是禁用的，你会损失很多性能
greater	你只能让深度值比gl_FragCoord.z更大
less	你只能让深度值比gl_FragCoord.z更小
unchanged	如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
*/
// layout(depth_greater)out float gl_FragDepth;

void main()
{
	//是否是正面
	if(gl_FrontFacing)
	FragColor=vec4(0.,0.,1.,1.);
	else
	FragColor=vec4(0.,0.,.5,1.);
	
	// gl_FragDepth=gl_FragCoord.z+.1;
	
}