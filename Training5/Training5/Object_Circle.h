#pragma once

class Circle {		// Circle Ŭ���� ����
private:
	float x;
	float y;
	float radius;
	float radius_temp;			// �������� �����̻� Ŀ���� �� ���� ����� ���������� ���ƿ�
	float r, g, b;
	int draw_type = 1;
	bool disappear_true = false;		// �� ���� true�̸� ���� ȭ������� ��� �� ����

public:

	void setCircle(float x, float y, float r, float g, float b, float radius_temp);
	void Change_Draw_type(int type);
	void draw();
	void update();				// ���� ������ ���� ó��
	void physics_update();		// ���� ������ ������ ���ƿ��°� ó��
};