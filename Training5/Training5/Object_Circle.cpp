#include <iostream>
#include <cmath>
#include "Object_Circle.h"
#include "OpenGL.h"

using namespace std;
constexpr auto PI = 3.141592653589;;

void Circle::setCircle(float x, float y, float r, float g, float b, float radius_temp)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->g = g;
	this->b = b;
	this->radius = radius_temp;
	this->radius_temp = radius_temp;
}

void Circle::Change_Draw_type(int type)
{
	this->draw_type = type;
}

void Circle::draw()
{
	float x1, y1;

	glColor3f(this->r, this->g, this->b);		// 원의 색상 설정
	glPushMatrix();								// 행렬에 넣는 함수

	{
		if (this->draw_type == 1) {
			glPointSize(2.0f);
			glBegin(GL_POINTS);
		}
		else if (this->draw_type == 2) {
			glLineWidth(2.0f);
			glBegin(GL_LINE_STRIP);
		}

		for (int angle = 0; angle <= 360; ++angle) {			// 0~360도를 돌며 각 좌표를 행렬에 담음
			x1 = cos(angle * (PI / 180)) * this->radius;
			y1 = sin(angle * (PI / 180)) * this->radius;

			glVertex2f(x + x1, y + y1);
		}
		glEnd();
	}

	glPopMatrix();			// 행렬에서 꺼냄
	
}

void Circle::update()
{
	this->radius = radius + 0.015f;
}

void Circle::physics_update()
{
	if (this->radius > 0.5f) {
		this->radius = radius_temp;
	}
}