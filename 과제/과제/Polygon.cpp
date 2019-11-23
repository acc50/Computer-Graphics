#include "Polygon.h"

#include <iostream>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 1);
std::uniform_int_distribution<int> ver_num(4, 4);
std::uniform_real_distribution<double> urd(-0.7f, 0.7f);
std::uniform_real_distribution<double> control_y(0.0f, 0.2f);
std::uniform_real_distribution<double> control_x(-0.2f, 0.2f);
std::uniform_real_distribution<float> color(0.0f, 1.0f);

#define LEFT_X -1.2
#define RIGHT_X 1.2

Polygons::Polygons()
{

}

Polygons::~Polygons()
{

}

void Polygons::Set_Polygons()
{

	start_point = uid(dre);
	this->vertex_count = ver_num(dre);


	if (this->start_point == LEFT) {

		start_pos_x = (float)LEFT_X;
		start_pos_y = urd(dre);

		end_pos_x = (float)RIGHT_X;
		end_pos_y = urd(dre);

	}
	else {

		start_pos_x = (float)RIGHT_X;
		start_pos_y = urd(dre);

		end_pos_x = (float)LEFT_X;
		end_pos_y = urd(dre);
	}

	control_pos_x = control_x(dre);

	if (start_pos_y >= end_pos_y) {
		control_pos_y = start_pos_y - control_y(dre);
	}
	else {
		control_pos_y = end_pos_y + control_y(dre);
	}


	/*std::cout << start_pos_x << ", " << start_pos_y << "     " << end_pos_x << ", " << end_pos_y << "     " <<
		control_pos_x << ", " << control_pos_y << std::endl;

	std::cout << "시작점, 끝점, 컨트롤점" << std::endl;*/		// 확인용

	Transform = glm::translate(Transform, glm::vec3(start_pos_x, start_pos_y, 0.0f));

}

void Polygons::Sliced_Set_Polygons(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int n, int index_1, int index_2)
{
	this->is_sliced = true;

	this->vertex_count = n;

	// index * 6 , index * 6 + 1  -> 버텍스 좌표 x, y

	if (n == 3) {

	}

	else if (n == 4) {

	}

}

void Polygons::Update()
{

	Transform = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);

	//		-------------------------------- 베지어 곡선 부분 ------------------------------------

	i++;
	t = i / RATE;

	Pos.x = (2 * t*t - 3 * t + 1)*start_pos_x + (-4 * t*t + 4 * t)*control_pos_x + (2 * t*t - t)*end_pos_x;
	Pos.y = (2 * t*t - 3 * t + 1)*start_pos_y + (-4 * t*t + 4 * t)*control_pos_y + (2 * t*t - t)*end_pos_y;

	translate = glm::mat4(1.0f);
	translate = glm::translate(translate, glm::vec3(Pos.x, Pos.y, Pos.z));


	Transform = Transform * translate;

	//std::cout << "i값 " << i << "  t값 " << t << " " << Pos.x << ", " << Pos.y  << std::endl; 



	//		----------------------------------------------------------------------------------------


}

double Polygons::Get_I()
{
	return this->i;
}

void Polygons::Render(GLuint ShaderProgram)
{
	
	if (this->vertex_count == 3) {
		Draw_Triangle(ShaderProgram);
	}

	else if (this->vertex_count == 4) {
		Draw_Square(ShaderProgram);
	}

	else if (this->vertex_count == 5) {
		Draw_Pentagon(ShaderProgram);
	}

}

void Polygons::Draw_Fill()
{
	this->is_fill = true;
}

void Polygons::Draw_Line()
{
	this->is_fill = false;
}

float Polygons::Get_start_x()
{
	return this->start_pos_x;
}

float Polygons::Get_start_y()
{
	return this->start_pos_y;
}

float Polygons::Get_control_x()
{
	return this->control_pos_x;
}

float Polygons::Get_control_y()
{
	return this->control_pos_y;
}

float Polygons::Get_end_x()
{
	return this->end_pos_x;
}

float Polygons::Get_end_y()
{
	return this->end_pos_y;
}

float Polygons::Get_Pos_x()
{
	return this->Pos.x;
}

float Polygons::Get_Pos_y()
{
	return this->Pos.y;
}

void Polygons::Set_Curve(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
{
	this->start_pos_x = x1;
	this->start_pos_y = y1;
	this->control_pos_x = x2;
	this->control_pos_y = y2;
	this->end_pos_x = x3;
	this->end_pos_y = y3;

	this->i = 0;
	this->t = 0;
}

void Polygons::Draw_Triangle(GLuint ShaderProgram)
{

	GLuint modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertexData), tri_vertexData, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, &tri_indexData, GL_STATIC_DRAW);


	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	if (this->is_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}

void Polygons::Draw_Square(GLuint ShaderProgram)
{

	GLuint modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squ_vertexData), squ_vertexData, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &squ_indexData, GL_STATIC_DRAW);


	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	if (this->is_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}

void Polygons::Draw_Pentagon(GLuint ShaderProgram)
{
	GLuint modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pen_vertexData), pen_vertexData, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 9, &pen_indexData, GL_STATIC_DRAW);


	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	if (this->is_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}