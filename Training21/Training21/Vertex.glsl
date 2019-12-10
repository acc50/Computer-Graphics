#version 330 core

layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vNormal;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(trans * vec4(vPos, 1.0));
	gl_Position = projection * view * trans * vec4(vPos,1.0);
	Normal = normalize(vNormal);
}

