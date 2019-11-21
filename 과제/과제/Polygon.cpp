#include "Polygon.h"

#include <iostream>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 1);
std::uniform_real_distribution<double> urd(-0.7f, 0.7f);
std::uniform_real_distribution<double> control_y(0.0f, 0.2f);
std::uniform_real_distribution<double> control_x(-0.2f, 0.2f);

#define LEFT_X -0.7
#define RIGHT_X 0.7

Polygons::Polygons()
{

}

Polygons::~Polygons()
{

}

void Polygons::Set_Polygons()
{

	start_point = uid(dre);

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


	std::cout << start_pos_x << ", " << start_pos_y << "     " << end_pos_x << ", " << end_pos_y << "     " <<
		control_pos_x << ", " << control_pos_y << std::endl;

	std::cout << "시작점, 끝점, 컨트롤점" << std::endl;

	Transform = glm::translate(model, glm::vec3(start_pos_x, start_pos_y, 0.0f));
}

void Polygons::Update()
{

	Transform = glm::mat4(1.0f);
	Inverstrans = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);
	rotate = glm::mat4(1.0f);


	//rotate = glm::rotate(rotate, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	//Inverstrans = glm::translate(Inverstrans, glm::vec3(-Pos.x, Pos.y, Pos.z));
	//Transform = translate * rotate * Inverstrans;
	
	
	/*translate = glm::translate(translate, glm::vec3(Pos.x, Pos.y, Pos.z));

	Transform = translate;


	Pos.x += 0.1f;
	angle += 10.0f;*/



	//		-------------------------------- 베지어 곡선 부분 ------------------------------------

	i += 2;
	t = i / RATE;

	Pos.x = (2 * t*t - 3 * t + 1)*start_pos_x + (-4 * t*t + 4 * t)*control_pos_x + (2 * t*t - t)*end_pos_x;
	Pos.y = (2 * t*t - 3 * t + 1)*start_pos_y + (-4 * t*t + 4 * t)*control_pos_y + (2 * t*t - t)*end_pos_y;

	translate = glm::mat4(1.0f);
	translate = glm::translate(translate, glm::vec3(Pos.x, Pos.y, Pos.z));

	Transform = translate;

	std::cout << "i값 " << i << "  t값 " << t << " " << Pos.x << ", " << Pos.y << std::endl;


	if (this->is_visiable_root) {		// 경로 그리기
		
		for (double j = 0; j <= 100; j = j + 2) {



		}


	}

	//		----------------------------------------------------------------------------------------




	/*rotate = glm::rotate(rotate, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Inverstrans = glm::translate(Inverstrans, glm::vec3(-Pos.x, Pos.y, Pos.z));
	translate = glm::translate(translate, glm::vec3(Pos.x, Pos.y, Pos.z));

	Transform = Inverstrans * rotate * translate;

	model = model * Transform;*/


}

double Polygons::Get_I()
{
	return this->i;
}

void Polygons::Render(GLuint VBO, GLuint EBO, GLuint ShaderProgram)
{

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);

}

void Polygons::Draw_Route()
{
	this->is_visiable_root = true;
}

void Polygons::Hide_Route()
{
	this->is_visiable_root = false;
}

void Polygons::Create_Polygon()
{
	if (this->vertex_count == 3) {

	}

	else if (this->vertex_count == 4) {

	}

	else if (this->vertex_count == 5) {

	}

	else if (this->vertex_count == 6) {

	}

	else if (this->vertex_count == 7) {

	}

	else if (this->vertex_count == 7) {

	}
}