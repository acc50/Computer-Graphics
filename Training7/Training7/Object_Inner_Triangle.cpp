#include <iostream>
#include <cmath>
#include <random>
#include "Object_Inner_Triangle.h"
#include "OpenGL.h"


using namespace std;
constexpr auto PI = 3.141592653589;

random_device rd2;
default_random_engine dre2(rd2());
uniform_real_distribution<float> urd2(-0.3f, 0.3f);
uniform_real_distribution<float> random_color2(0.0f, 1.0f);
uniform_int_distribution<int> uid2(1, 360);

void Inner_Triangle::setTriangle()
{
	this->pointx = urd2(dre2);
	this->pointy = urd2(dre2);

	this->ChangeColor();

	this->direction = uid2(dre2);
}

void Inner_Triangle::ChangeColor()
{
	for (int i = 0; i < 3; ++i) {
		this->r[i] = random_color2(dre2);
		this->g[i] = random_color2(dre2);
		this->b[i] = random_color2(dre2);
	}
}

void Inner_Triangle::draw()
{
	
	glPushMatrix();								// 행렬에 넣는 함수
	

	{
		glBegin(GL_TRIANGLES);

		glColor3f(this->r[0], this->g[0], this->b[0]);		// 색상 설정
		x1 = pointx + cos(direction * (PI / 180)) * 0.10f;
		y1 = pointy + sin(direction * (PI / 180)) * 0.10f;
		glVertex2f(x1, y1);

		glColor3f(this->r[1], this->g[1], this->b[1]);		// 색상 설정
		x2 = pointx + cos((direction + 90) * (PI / 180)) * 0.04f;
		y2 = pointy + sin((direction + 90) * (PI / 180)) * 0.04f;
		glVertex2f(x2, y2);

		
		glColor3f(this->r[2], this->g[2], this->b[2]);		// 색상 설정
		x3 = pointx + cos((direction - 90) * (PI / 180)) * 0.04f;
		y3 = pointy + sin((direction - 90) * (PI / 180)) * 0.04f;
		glVertex2f(x3, y3);



		glEnd();
	}

	glPopMatrix();
}

void Inner_Triangle::update()
{
	float x1, y1;

	x1 = cos(direction * (PI / 180)) * 0.02f;
	y1 = sin(direction * (PI / 180)) * 0.02f;

	pointx += x1;
	pointy += y1;
}

void Inner_Triangle::physics_update()
{
	if (this->y1 >= 0.3f || this->y2 >= 0.3f || this->y3 >= 0.3f) {					//위
		if (direction >= 90 && direction <= 180) {		// 왼쪽위로 가는중에 충돌
			int temp = 180 - direction;
			direction = 180 + temp;
		}

		else if (direction >= 0 && direction <= 90) {		// 오른쪽 위로 가는중에 충돌
			int temp = -direction;
			direction = 360 + temp;
		}
		this->ChangeColor();
	}

	else if (this->x1 <= -0.3f || this->x2 <= -0.3f || this->x3 <= -0.3f) {			//왼쪽
		if (direction >= 180 && direction <= 270) {		// 왼쪽 아래로 가는중에 충돌
			int temp = 270 - direction;
			direction = 270 + temp;
		}

		else if (direction >= 90 && direction <= 180) {		// 왼쪽 위로 가는중에 충돌
			int temp = direction - 90;
			direction = 90 - temp;
		}
		this->ChangeColor();
	}

	else if (this->x1 >= 0.3f || this->x2 >= 0.3f || this->x3 >= 0.3f) {			//오른쪽
		if (direction >= 270 && direction <= 360) {			//  오른쪽 아래로 가는중에 충돌
			int temp = direction - 270;
			direction = 270 - temp;
		}

		else if (direction >= 0 && direction <= 90) {	//  오른쪽 위로 가는중에 충돌
			int temp = 90 - direction;
			direction = 90 + temp;
		}
		this->ChangeColor();
	}

	else if (this->y1 <= -0.3f || this->y2 <= -0.3f || this->y3 <= -0.3f) {			//아래
		if (direction >= 270 && direction <= 360) {			// 오른쪽 아래로 가는중에 충돌
			int temp = 360 - direction;
			direction = temp;
		}

		else if (direction >= 180 && direction <= 270) {		// 왼쪽 아래로 가는중에 충돌
			int temp = direction - 180;
			direction = 180 - temp;
		}
		this->ChangeColor();
	}

	direction = direction % 360;
}