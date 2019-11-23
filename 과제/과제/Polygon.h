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

	bool is_fill = true;
	
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f);	// ��ġ

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 Transform = model;		// ��ȯ

	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 Inverstrans = glm::mat4(1.0f);

public:
	Polygons();
	~Polygons();

	Polygons *next = nullptr;		// ���Ḯ��Ʈ ���� ����
	Polygons *pre = nullptr;
	bool is_sliced = false;		// �߷ȴ��� Ȯ��

	void Set_Polygons();
	void Sliced_Set_Polygons(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int n, int index_1, int index_2);
	
	void Update();
	void Render(GLuint ShaderProgram);

	void Draw_Fill();
	void Draw_Line();

	double Get_I();
	int Get_vertex_number() { return this->vertex_count; }
	float Get_start_x();
	float Get_start_y();
	float Get_control_x();
	float Get_control_y();
	float Get_end_x();
	float Get_end_y();

	float Get_Pos_x();
	float Get_Pos_y();

	void Set_Vertex_Count(int n) { this->vertex_count = n; };
	void Set_Curve(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);		// �߸� ������ ��� � �缳��

	GLfloat tri_vertexData[18] = {
		0.0f, 0.15f, 0.0f,		1.0f, 1.0f, 1.0f,		// 1����
		-0.1f, -0.07f, 0.0f,	1.0f, 1.0f, 1.0f,		// 2����
		0.1f, -0.07f, 0.0f,		1.0f, 1.0f, 1.0f		// 3����
	};

	unsigned int tri_indexData[3] = {
		0,1,2
	};

	GLfloat squ_vertexData[24] = {
		-0.1f,-0.1f, 0.0f,	1.0f, 1.0f, 1.0f,	// ���ϴ�
		-0.1f, 0.1f, 0.0f,	1.0f, 1.0f, 1.0f,	// �»��
		0.1f, 0.1f, 0.0f,	1.0f, 1.0f, 1.0f,	// ����
		0.1f, -0.1f, 0.0f,	1.0f, 1.0f, 1.0f	// ���ϴ�
	};

	unsigned int squ_indexData[6] = {
		0,3,2,
		0,2,1
	};

	GLfloat pen_vertexData[30] = {
		-0.05f,-0.05f, 0.0f,		1.0f, 1.0f, 1.0f,	// ���ϴ�
		-0.1f, 0.05f, 0.0f,			1.0f, 1.0f, 1.0f,	// �»��
		0.0f, 0.12f, 0.0f,			1.0f, 1.0f, 1.0f,	// ����
		0.1f, 0.05f, 0.0f,			1.0f, 1.0f, 1.0f,	// ����
		0.05f, -0.05f, 0.0f,		1.0f, 1.0f, 1.0f	// ���ϴ�
	};

	unsigned int pen_indexData[9] = {
		0,2,1,
		0,3,2,
		0,4,3
	};


	Polygons *Get_This() {		// �ڱ� �ڽ� �ּ� ��ȯ
		return this;
	}		

	void Draw_Triangle(GLuint ShaderProgram);
	void Draw_Square(GLuint ShaderProgram);
	void Draw_Pentagon(GLuint ShaderProgram);

};