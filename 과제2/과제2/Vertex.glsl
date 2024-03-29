#version 330 core

layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vColor;
out vec3 outColor;
uniform mat4 trans;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * trans * vec4(vPos,1.0);
	outColor=vColor;
}

