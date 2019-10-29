#pragma once

class Triangle {		// Circle 클래스 선언
private:
	float r, g, b;
	int direction;
	float pointx;
	float pointy;
	float x1, y1, x2, y2, x3, y3;

public:

	void setTriangle();
	void draw();
	void update();				// 원의 반지름 증가 처리
	void physics_update();		// 원의 반지름 증가시 돌아오는것 처리
};