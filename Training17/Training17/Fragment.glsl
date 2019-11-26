#version 330 core

out vec4 Fragcolor;
in vec3 outColor;
void main()
{
	Fragcolor = vec4(outColor.r, outColor.g, outColor.b, 1.0);
}