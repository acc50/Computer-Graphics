#include "Robot.h"
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<GLfloat> urd(1.5f, 4.5f);
std::uniform_real_distribution<double> set_rate(80, 110);
std::uniform_int_distribution<int> uid(0, 360);
std::uniform_int_distribution<int> uid2(0, 1);

Robot::Robot()
{
}


Robot::~Robot()
{
}

void Robot::Set_Robot(int num)
{
	int location = uid2(dre);
	if (location == 0)
		location = -1;
	this->Pos.x = location * urd(dre);		// y 값은 변화 X

	location = uid2(dre);
	if (location == 0)
		location = -1;
	this->Pos.z = location * urd(dre);

	this->rate = set_rate(dre);
	this->number = num;
}

void Robot::Render(GLuint ShaderProgram, GLuint EBO, GLuint VBO)
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	rm = glm::rotate(rm, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tx = glm::translate(tx, glm::vec3(Pos.x, Pos.y, Pos.z));

	model = model * tx * rm;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환



	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}

void Robot::Set_MovePoint()
{
	this->i = 0;				// i 초기화
	this->angle = uid(dre);

	double radius = 0.5;

	double x = radius * cos(angle * DEG2RAD);
	double z = radius * sin(angle * DEG2RAD);

	this->start_Pos = Pos;		// 시작점은 현재 위치
	this->move_Pos.x = start_Pos.x + x;
	this->move_Pos.z = start_Pos.z + z;
	
	is_set_move_point = true;
}

void Robot::Set_CameraPoint(GLfloat ex, GLfloat ey, GLfloat ez)
{
	this->i = 0;

	this->start_Pos = Pos;
	this->move_Pos.x = ex;
	this->move_Pos.z = ez;
}

void Robot::Move(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat ax, GLfloat ay, GLfloat az)
{
	if (this->is_being_camera) {		// (1) 카메라에 속했다면

		this->Set_CameraPoint(ex, ey, ez);		// 카메라 위치에 따라 움직일 곳 갱신

		i += this->speed * 2.0;
		t = i / this->rate;

		Pos.x = (1 - t) * start_Pos.x + t * move_Pos.x;
		Pos.z = (1 - t) * start_Pos.z + t * move_Pos.z;

		Pos.x = clamp(-5.0f + 0.3f, Pos.x, 5.0f - 0.3f);			// 이동범위 제한
		Pos.z = clamp(-5.0f + 0.3f, Pos.z, 5.0f - 0.3f);

	}


	else {								// (2) 카메라에 속하지 않았다면

		if (!this->is_detect_camera) {			// 1. 카메라 감지 X

			this->Detect_Camera(ex, ey, ez);	// 카메라가 감지범위 내로 들어오는지 확인

			if (!this->is_set_move_point)		// 이동방향 설정 X 시 이동방향 설정
				this->Set_MovePoint();

			else {								// 이동방향이 정해졌으면 움직임


				// -------------------------  이동 부분 ---------------------------

				if (Pos.x >= 5.0f || Pos.x <= -5.0f || Pos.z >= 5.0f || Pos.z <= -5.0f) {	// 가장자리 도달 시 방향 다시 설정
					this->is_set_move_point = false;
				}


				i += this->speed;
				t = i / this->rate;

				Pos.x = (1 - t) * start_Pos.x + t * move_Pos.x;
				Pos.z = (1 - t) * start_Pos.z + t * move_Pos.z;

				Pos.x = clamp(-5.0f + 0.3f, Pos.x, 5.0f - 0.3f);			// 이동범위 제한
				Pos.z = clamp(-5.0f + 0.3f, Pos.z, 5.0f - 0.3f);


				if (i >= this->rate) {								// 이동이 끝나면 방향을 다시 설정해야함
					this->is_set_move_point = false;
				}

				// -------------------------  이동 부분 ---------------------------

			}
		}

		else {								// 2. 카메라 감지 O

			this->Detect_Camera(ex, ey, ez);		// 카메라가 감지범위를 벗어나는지 확인

			i += this->speed * 1.5;
			t = i / this->rate;

			Pos.x = (1 - t) * start_Pos.x + t * move_Pos.x;
			Pos.z = (1 - t) * start_Pos.z + t * move_Pos.z;

			Pos.x = clamp(-5.0f + 0.3f, Pos.x, 5.0f - 0.3f);			// 이동범위 제한
			Pos.z = clamp(-5.0f + 0.3f, Pos.z, 5.0f - 0.3f);

			this->Check_Being_Camera(ex, ey, ez);
		}
	}
}

void Robot::Detect_Camera(GLfloat cx, GLfloat cy, GLfloat cz)		// 카메라 좌표를 받음
{

	double x = cx - this->Pos.x;
	double z = cz - this->Pos.z;
	double distance = sqrt(x * x + z * z);

	if (distance <= (double)this->detect_range) {		// 탐지범위보다 거리가 좁으면
		is_detect_camera = true;
		is_set_move_point = true;
		i = 0;

		this->start_Pos = Pos;		// 시작지점은 현재위치
		this->move_Pos.x = cx;		// 카메라 x,z 위치가 움직일 지점
		this->move_Pos.z = cz;
	}
	else {											// 카메라가 탐지범위 밖이면

		if (is_detect_camera) {			// 카메라가 이미 탐지되어있으면
			is_detect_camera = false;
			is_set_move_point = false;
		}
	}
}

void Robot::Check_Being_Camera(GLfloat cx, GLfloat cy, GLfloat cz)
{
	double x = cx - this->Pos.x;
	double z = cz - this->Pos.z;
	double distance = sqrt(x * x + z * z);

	if (distance <= (double)this->being_range) {
		this->is_being_camera = true;
	}
}

glm::vec3 Robot::get_Pos()
{
	return this->Pos;
}

void Robot::set_Pos_x(GLfloat x)
{
	this->Pos.x = x;
}

void Robot::set_Pos_z(GLfloat z)
{
	this->Pos.z = z;
}

GLfloat Max(GLfloat a, GLfloat b)
{
	if (a > b)
		return a;
	else
		return b;
}

GLfloat Min(GLfloat a, GLfloat b)
{
	if (a < b)
		return a;
	else
		return b;
}

GLfloat clamp(GLfloat min, GLfloat x, GLfloat max)
{
	return Max(min, Min(x, max));
}