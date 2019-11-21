#version 330 core

layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vColor;
out vec3 outColor;
uniform mat4 trans;
void main()
{
	gl_Position=trans*vec4(vPos.x,vPos.y,vPos.z,1.0);
	outColor=vColor;
}

