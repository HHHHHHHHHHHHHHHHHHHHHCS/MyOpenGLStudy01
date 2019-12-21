#version 460 core
// points：绘制GL_POINTS图元时（1）。
// lines：绘制GL_LINES或GL_LINE_STRIP时（2）
// lines_adjacency：GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）//一个四边形或者两个三角形
// triangles：GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
// triangles_adjacency：GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）//两个四边形
layout(points)in;//输入一个点
// points
// line_strip
// triangle_strip
layout(triangle_strip,max_vertices=5)out;//输出最大五个点

//输入的数据   因为输入的可能是多个点  所以这里是数组
//GLSL提供给我们一个内建(Built-in)变量
// vec4  gl_Position;
// float gl_PointSize;
// float gl_ClipDistance[];
in VS_OUT
{
	vec3 color;
}gs_in[];

out vec3 fColor;//因为把这五个点组成了一个片元 所以只用一个颜色

void BuildHouse(vec4 position)
{
	fColor=gs_in[0].color;//前面四个点为原来的颜色
	gl_Position=position+vec4(-.2,-.2,0.,0.);//1:bootom-left
	EmitVertex();//把gl_Position和要out的fColor注册进去
	gl_Position=position+vec4(.2,-.2,0.,0.);//2:bottom-right
	EmitVertex();
	gl_Position=position+vec4(-.2,.2,0.,0.);//3:top-left
	EmitVertex();
	gl_Position=position+vec4(.2,.2,0.,0.);//4:top-right
	EmitVertex();
	gl_Position=position+vec4(0.,.4,0.,0.);//5:top
	fColor=vec3(1.,1.,1.);
	EmitVertex();
	EndPrimitive();//把上面注册的push and clear
}

void main()
{
	BuildHouse(gl_in[0].gl_Position);
}