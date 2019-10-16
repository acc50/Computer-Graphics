#pragma once

class Circle {		// Circle 클래스 선언
private:
	float x;
	float y;
	float radius;
	float radius_temp;			// 반지름이 일정이상 커지면 이 값에 저장된 반지름으로 돌아옴
	float r, g, b;
	int draw_type = 1;
	bool disappear_true = false;		// 이 값이 true이면 원은 화면밖으로 벗어날 수 있음

public:

	void setCircle(float x, float y, float r, float g, float b, float radius_temp);
	void Change_Draw_type(int type);
	void draw();
	void update();				// 원의 반지름 증가 처리
	void physics_update();		// 원의 반지름 증가시 돌아오는것 처리
};