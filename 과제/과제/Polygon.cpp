#include "Polygon.h"

#include <iostream>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 1);
std::uniform_int_distribution<int> ver_num(3, 5);
std::uniform_real_distribution<double> urd(-0.7f, 0.7f);
std::uniform_real_distribution<double> control_y(0.0f, 0.2f);
std::uniform_real_distribution<double> control_x(-0.3f, 0.3f);
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

	int k = ver_num(dre);

	if (k == 3) {
		this->vertex_count = 3;
	}
	else {

		this->vertex_count = 4;
	}



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

void Polygons::Sliced_Set_Polygons(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int n, int index_1, int index_2, int q, GLfloat pos_x, GLfloat pos_y)
{

	this->Pos.x = pos_x;
	this->Pos.y = pos_y;

	this->is_sliced = true;

	this->vertex_count = n;

	if (n == 3) {

		tri_vertexData[index_1 * 6] = x1;
		tri_vertexData[index_1 * 6 + 1] = y1;
		tri_vertexData[index_2 * 6] = x2;
		tri_vertexData[index_2 * 6 + 1] = y2;

		if (q == 0) {		//좌상단
			tri_vertexData[6] = -0.1f;
			tri_vertexData[7] = 0.1f;
		}
		else if (q == 1) {		//우상단
			tri_vertexData[6] = 0.1f;
			tri_vertexData[7] = 0.1f;
		}
		else if (q == 2) {		//우하단
			tri_vertexData[12] = 0.1f;
			tri_vertexData[13] = -0.1f;
		}
		else if (q == 3) {		//좌하단
			tri_vertexData[0] = -0.1f;
			tri_vertexData[1] = -0.1f;
		}
	}

	else if (n == 4) {
		
		squ_vertexData[index_1 * 6] = x1;
		squ_vertexData[index_1 * 6 + 1] = y1;
		squ_vertexData[index_2 * 6] = x2;
		squ_vertexData[index_2 * 6 + 1] = y2;


	}

	Transform = glm::translate(Transform, glm::vec3(start_pos_x, start_pos_y, 0.0f));



	cp_x = pos_x + control_x(dre);
	cp_y = pos_y + control_y(dre);
	ep_x, ep_y;
	if (cp_x < pos_x) {		// 왼쪽으로 튕김
		ep_x = cp_x - 0.2f;
	}
	else {		// 오른쪽으로 튕김

		ep_x = cp_x + 0.2f;
	}
	ep_y = -1.4f;

	this->Set_Curve(pos_x, pos_y, cp_x, cp_y, ep_x, ep_y);		// 잘린 후 생성된 도형의 스플라인
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


	if (is_draw_route && (route_vertexData[0] == 0.0f)) {		// 초기화 X면

		double z = 0;
		int r_num = 0;

		GLfloat r = color(dre);
		GLfloat g = color(dre);
		GLfloat b = color(dre);


		for (int k = 0; k < 100; k = k+10) {
			z = (double)k / (double)100;

			route_vertexData[6 * r_num] = (2 * z*z - 3 * z + 1)*start_pos_x + (-4 * z*z + 4 * z)*control_pos_x + (2 * z*z - z)*end_pos_x;
			route_vertexData[6 * r_num + 1] = (2 * z*z - 3 * z + 1)*start_pos_y + (-4 * z*z + 4 * z)*control_pos_y + (2 * z*z - z)*end_pos_y;
			route_vertexData[6 * r_num + 3] = r;
			route_vertexData[6 * r_num + 4] = g;
			route_vertexData[6 * r_num + 5] = b;


			r_num = r_num + 1;
		}
	}

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

	if (this->is_draw_route)
		this->Draw_Route(ShaderProgram);

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

void Polygons::Set_Curve(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)		// 잘린 도형의 스플라인
{
	this->start_pos_x = x1;
	this->start_pos_y = y1;

	control_pos_x = x1 + control_x(dre);
	control_pos_y = y1 + control_y(dre);

	ep_x, ep_y;
	if (cp_x < x1) {		// 왼쪽으로 튕김
		end_pos_x = control_pos_x - 0.2f;
	}
	else {		// 오른쪽으로 튕김

		end_pos_x = control_pos_x + 0.2f;
	}
	end_pos_y = -1.4f;

	/*this->control_pos_x = x2;
	this->control_pos_y = y2;
	this->end_pos_x = x3;
	this->end_pos_y = y3;*/

	this->route_vertexData[0] = 0.0f;

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

void Polygons::Draw_Route(GLuint ShaderProgram)
{

	glm::mat4 rt = glm::mat4(1.0f);

	GLuint modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rt));

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(route_vertexData), route_vertexData, GL_STATIC_DRAW);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPointSize(10.0f);

	glDrawArrays(GL_POINTS, 0, 10);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}