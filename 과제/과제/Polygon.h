#pragma once
#include "OpenGL.h"
#include <Windows.h>
#include <random>

#define RATE 100

enum starting { LEFT = 0, RIGHT };

class Polygons
{
private:
	int vertex_count = 0;
	float angle = 0.0f;

	int start_point = 0;

	double i = 0;
	double t = 0;

	double start_pos_x, start_pos_y;
	double control_pos_x, control_pos_y;
	double end_pos_x, end_pos_y;

	bool is_visiable_root = false;

	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 Transform = model;

	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 Inverstrans = glm::mat4(1.0f);
	glm::mat4 rotate = glm::mat4(1.0f);

public:
	Polygons();
	~Polygons();
	void Create_Polygon();
	void Set_Polygons();
	
	void Update();
	void Render(GLuint VBO, GLuint EBO, GLuint ShaderProgram);

	void Draw_Route();
	void Hide_Route();

	double Get_I();

};