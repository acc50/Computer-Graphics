#pragma once

class Triangle {		// Circle Ŭ���� ����
private:
	float r, g, b;
	int direction;
	float pointx;
	float pointy;
	float x1, y1, x2, y2, x3, y3;

public:

	void setTriangle();
	void draw();
	void update();				// ���� ������ ���� ó��
	void physics_update();		// ���� ������ ������ ���ƿ��°� ó��
};