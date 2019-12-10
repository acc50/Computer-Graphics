#pragma once

#include "openGL.h"

class snow
{
private:

	GLfloat fall_speed = 0.1f;
	glm::vec3 Pos = glm::vec3(1.0f);

public:
	snow();
	~snow();

	void Init();
	void Render(GLuint ShaderProgram, GLuint VBO, GLuint EBO);
	void Fall();

};

