#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "file2buf.h"			// ���̴� ���� �б� ���
#include "OpenGL.h"				// OpenGL�� ���̺귯���� ��� ���
#include "Object_Circle.h"		// Circle�� Ŭ������ ��� ���

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_real_distribution<float> random_radius(0.03f, 0.1f);		// Ŭ���� ���� �ʱ� ������ ���� ����
uniform_real_distribution<float> random_color(0.0f, 1.0f);	// ���� ���� ���� ����

int Window_Width = 1200, Window_Height = 1000;

Circle *circles = new Circle[10];		// Ŭ������ ���� ���� 10�� �����Ҵ�

int n = 0;			// ���� ���° ������ �� �� �ִ� ����
int cnt = 0;		// ���� ����

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);

int main(int argc, char** argv) // ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(Window_Width, Window_Height); // �������� ũ�� ����
	glutCreateWindow("Training3"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLE" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene);						 	 // ��� �Լ��� ����
	glutReshapeFunc(Reshape);							 // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();										 // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�


	// �׸��� �κ� ����

	for (int i = 0; i < cnt; ++i) {	
		circles[i].draw();				//  ���� ���� ������ŭ �׷���
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸���
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat x1, y1;
	float x_size = Window_Width / 2, y_size = Window_Height / 2;

	if (x < x_size) {
		x1 = -(GLfloat)((GLfloat)(x_size - x) / (GLfloat)x_size);
	}
	else {
		x1 = (GLfloat)((GLfloat)(x - x_size) / (GLfloat)x_size);
	}

	if (y < y_size) {
		y1 = (GLfloat)((GLfloat)(y_size - y) / (GLfloat)y_size);
	}
	else {
		y1 = -(GLfloat)((GLfloat)(y - y_size) / (GLfloat)y_size);
	}

	//	-- ���� �۾��� ���콺 ��ǥ�� -1.0f ~ 1.0f �� ����Ʈ ������ǥ�� �ٲ�

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {			// ����Ŭ���� n��° ���� �ʱ� ����
		circles[n].setCircle(x1, y1, random_color(dre),
			random_color(dre), random_color(dre), random_radius(dre));

		glutTimerFunc(100, Timer, n);		// n��° ���� �ִϸ��̼� Ÿ�̸� ����

		n = (n + 1) % 10;			// ���� 10���� �����Ǿ�� �ϰ� 10���� �Ѿ�� ó������ ���ư�
		cnt = cnt + 1;
		if (cnt > 10)			// ���� ������ 10���� �Ѿ �� ����
			cnt = 10;
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {			// ���� �׸��� Ÿ�� ����
	case '1':
		for (int i = 0; i < cnt; ++i) {
			circles[i].Change_Draw_type(1);
		}
		break;

	case '2':
		for (int i = 0; i < cnt; ++i) {
			circles[i].Change_Draw_type(2);
		}
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)		// ���� ������ ��ȭ, �������� �����̻� �Ѿ�� �ʱ�ȭ �۾��� ����
{
	circles[value].update();
	circles[value].physics_update();
	glutPostRedisplay();
	glutTimerFunc(100, Timer, value);		// n��° ���� Ÿ�̸Ӹ� ����ؼ� �θ�
}