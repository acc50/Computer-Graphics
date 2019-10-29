#include <iostream>
#include <cmath>
#include <random>
#include "Object_Triangle.h"
#include "OpenGL.h"

using namespace std;
constexpr auto PI = 3.141592653589;

random_device rd;
default_random_engine dre(rd());
uniform_real_distribution<float> urd(-0.8f, 0.8f);
uniform_real_distribution<float> random_color(0.0f, 1.0f);
uniform_int_distribution<int> uid(1, 360);

void Triangle::setTriangle()
{
	while (1) {							// ���� �簢���ۿ��� �ﰢ�� ����
		this->pointx = urd(dre);
		this->pointy = urd(dre);

		if ((this->pointx > 0.45f || this->pointx < -0.45f) && (this->pointy > 0.45f || this->pointy < -0.45f))
			break;
	}

	this->r = random_color(dre);
	this->g = random_color(dre);
	this->b = random_color(dre);
	this->direction = uid(dre);
}

void Triangle::ChangeColor()
{
	this->r = random_color(dre);
	this->g = random_color(dre);
	this->b = random_color(dre);
}

void Triangle::draw()
{

	glColor3f(this->r, this->g, this->b);		// ���� ����
	glPushMatrix();								// ��Ŀ� �ִ� �Լ�

	{
		glBegin(GL_TRIANGLES);

		x1 = pointx + cos(direction * (PI / 180)) * 0.15f;
		y1 = pointy + sin(direction * (PI / 180)) * 0.15f;
		glVertex2f(x1, y1);

		x2 = pointx + cos((direction + 90) * (PI / 180)) * 0.07f;
		y2 = pointy + sin((direction + 90) * (PI / 180)) * 0.07f;
		glVertex2f(x2, y2);

		x3 = pointx + cos((direction - 90) * (PI / 180)) * 0.07f;
		y3 = pointy + sin((direction - 90) * (PI / 180)) * 0.07f;
		glVertex2f(x3, y3);



		glEnd();
	}

	glPopMatrix();	
	
}

void Triangle::update()
{
	float x1, y1;

	x1 = cos(direction * (PI / 180)) * 0.02f;
	y1 = sin(direction * (PI / 180)) * 0.02f;

	pointx += x1;
	pointy += y1;
}

void Triangle::physics_update()
{
	if (this->y1 >= 1.0f || this->y2 >= 1.0f || this->y3 >= 1.0f) {					//��
		if (direction >= 90 && direction <= 180) {		// �������� �����߿� �浹
			int temp = 180 - direction;
			direction = 180 + temp;
		}

		else if (direction >= 0 && direction <= 90) {		// ������ ���� �����߿� �浹
			int temp = -direction;
			direction = 360 + temp;
		}
		this->ChangeColor();
	}

	else if (this->x1 <= -1.0f || this->x2 <= -1.0f || this->x3 <= -1.0f) {			//����
		if (direction >= 180 && direction <= 270) {		// ���� �Ʒ��� �����߿� �浹
			int temp = 270 - direction;
			direction = 270 + temp;
		}

		else if (direction >= 90 && direction <= 180) {		// ���� ���� �����߿� �浹
			int temp = direction - 90;
			direction = 90 - temp;
		}
		this->ChangeColor();
	}

	else if (this->x1 >= 1.0f || this->x2 >= 1.0f || this->x3 >= 1.0f) {			//������
		if (direction >= 270 && direction <= 360) {			//  ������ �Ʒ��� �����߿� �浹
			int temp = direction - 270;
			direction = 270 - temp;
		}

		else if (direction >= 0 && direction <= 90) {	//  ������ ���� �����߿� �浹
			int temp = 90 - direction;
			direction = 90 + temp;
		}
		this->ChangeColor();
	}

	else if (this->y1 <= -1.0f || this->y2 <= -1.0f || this->y3 <= -1.0f) {			//�Ʒ�
		if (direction >= 270 && direction <= 360) {			// ������ �Ʒ��� �����߿� �浹
			int temp = 360 - direction;
			direction = temp;
		}

		else if (direction >= 180 && direction <= 270) {		// ���� �Ʒ��� �����߿� �浹
			int temp = direction - 180;
			direction = 180 - temp;
		}
		this->ChangeColor();
	}


	if ((this->y1 <= 0.3f && this->y1 >= -0.3f && this->x1 >= -0.3f && this->x1 <= 0.3f)||
		(this->y2 <= 0.3f && this->y2 >= -0.3f && this->x2 >= -0.3f && this->x2 <= 0.3f)||
		(this->y3 <= 0.3f && this->y3 >= -0.3f && this->x3 >= -0.3f && this->x3 <= 0.3f)) 
	{			// ���� �簢�� �浹

		if ((this->x1 <= -0.25f && this->y1 >= -0.3f && this->y1 <= 0.3f)||
			(this->x2 <= -0.25f && this->y2 >= -0.3f && this->y2 <= 0.3f)||
			(this->x3 <= -0.25f && this->y3 >= -0.3f && this->y3 <= 0.3f)) 
		{		// ���� �� �浹
			if (direction >= 270 && direction <= 360) {			//  ������ �Ʒ��� �����߿� �浹
				int temp = direction - 270;
				direction = 270 - temp;
			}

			else if (direction >= 0 && direction <= 90) {	//  ������ ���� �����߿� �浹
				int temp = 90 - direction;
				direction = 90 + temp;
			}
			this->ChangeColor();
		}

		else if ((this->x1 >= 0.25f && this->y1 >= -0.3f && this->y1 <= 0.3f)||
				(this->x2 >= 0.25f && this->y2 >= -0.3f && this->y2 <= 0.3f)||
				(this->x3 >= 0.25f && this->y3 >= -0.3f && this->y3 <= 0.3f)) 
		{		// ������ �� �浹
			if (direction >= 180 && direction <= 270) {		// ���� �Ʒ��� �����߿� �浹
				int temp = 270 - direction;
				direction = 270 + temp;
			}

			else if (direction >= 90 && direction <= 180) {		// ���� ���� �����߿� �浹
				int temp = direction - 90;
				direction = 90 - temp;
			}
			this->ChangeColor();
		}

		else if ((this->y1 <= -0.25f && this->x1 >= -0.3f && this->x1 <= 0.3f)||
				(this->y2 <= -0.25f && this->x2 >= -0.3f && this->x2 <= 0.3f)||
				(this->y3 <= -0.25f && this->x3 >= -0.3f && this->x3 <= 0.3f))
		{		// �Ʒ��� �� �浹
			if (direction >= 90 && direction <= 180) {		// �������� �����߿� �浹
				int temp = 180 - direction;
				direction = 180 + temp;
			}

			else if (direction >= 0 && direction <= 90) {		// ������ ���� �����߿� �浹
				int temp = -direction;
				direction = 360 + temp;
			}
			this->ChangeColor();
		}

		else if ((this->y1 >= 0.25f  && this->x1 >= -0.3f && this->x1 <= 0.3f)||
				(this->y2 >= 0.25f  && this->x2 >= -0.3f && this->x2 <= 0.3f)||
				(this->y3 >= 0.25f  && this->x3 >= -0.3f && this->x3 <= 0.3f))
		{		// ���� �� �浹
			if (direction >= 270 && direction <= 360) {			// ������ �Ʒ��� �����߿� �浹
				int temp = 360 - direction;
				direction = temp;
			}

			else if (direction >= 180 && direction <= 270) {		// ���� �Ʒ��� �����߿� �浹
				int temp = direction - 180;
				direction = 180 - temp;
			}
			this->ChangeColor();
		}


	}


	direction = direction % 360;
}