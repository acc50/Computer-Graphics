#pragma once

class Inner_Triangle {		// Circle Ŭ���� ����
private:
	float r[3], g[3], b[3];
	int direction;
	float pointx;
	float pointy;
	float x1, y1, x2, y2, x3, y3;

public:

	void setTriangle();
	void ChangeColor();
	void draw();
	void update();				// ���� ������ ���� ó��
	void physics_update();		// ���� ������ ������ ���ƿ��°� ó��
};