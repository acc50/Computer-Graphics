#pragma once

#include <iostream>
#include "openGL.h"
#include <cmath>

#define PI 3.14159265359879
#define DEG2RAD PI/180				// 각도 -> 라디안


GLfloat Max(GLfloat a, GLfloat b);
GLfloat Min(GLfloat a, GLfloat b);
GLfloat clamp(GLfloat min, GLfloat x, GLfloat max);

class Robot
{
private:

	glm::vec3 Pos = glm::vec3(0.0f);

	glm::vec3 start_Pos = glm::vec3(0.0f);
	glm::vec3 move_Pos = glm::vec3(0.0f);

	GLfloat angle = 0.0f;
	double t = 0;
	double i = 0;
	double rate = 0;
	double detect_range = 2.0;
	double being_range = 0.2;
	double speed = 2;
	int number = 0;

	bool is_set_move_point = false;
	bool is_detect_camera = false;
	bool is_being_camera = false;

public:
	Robot();
	~Robot();

	void Set_Robot(int num);
	void Render(GLuint ShaderProgram, GLuint EBO, GLuint VBO);

	
	void Set_MovePoint();
	void Set_CameraPoint(GLfloat ex, GLfloat ey, GLfloat ez);
	void Move(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat ax, GLfloat ay, GLfloat az);
	void Detect_Camera(GLfloat x, GLfloat y, GLfloat z);
	void Check_Being_Camera(GLfloat cx, GLfloat cy, GLfloat cz);
	glm::vec3 get_Pos();
	void set_Pos_x(GLfloat x);
	void set_Pos_z(GLfloat z);

};

