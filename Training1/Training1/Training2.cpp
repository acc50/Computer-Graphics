// Training1.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/freeglut_ext.h>

std::default_random_engine dre;
std::uniform_real_distribution<float> uid(0.0f, 1.0f);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

int Window_Width = 800;
int Window_Height = 600;

float red = 1.0f, green = 1.0f, blue = 1.0f;
float Rectangle_red = 0.0f, Rectangle_green = 0.0f, Rectangle_blue = 0.0f;

int main(int argc, char** argv) // ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(Window_Width, Window_Height); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLE" << std::endl;
		;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene);						 	 // ��� �Լ��� ����
	glutReshapeFunc(Reshape);							 // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMainLoop();										 // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene()
{
	glClearColor(red, green, blue, 1.0f); // �������� ��white���� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�


	// �׸��� �κ� ����

	glColor3f(Rectangle_red, Rectangle_green, Rectangle_blue);
	glRectf(-0.5f, -0.5f, +0.5f, +0.5f);		// ������â �߾��� 0.0f
												// -1.0f ~ 1.0f ������ ����


	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�


	glFlush(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸���
{
	glViewport(0, 0, w, h);
}void Mouse(int button, int state, int x, int y){	int x1, x2, y1, y2;		x1 = Window_Width / 2 - Window_Width / 4;	y1 = Window_Height / 2 - Window_Height / 4;	x2 = Window_Width / 2 + Window_Width / 4;	y2 = Window_Height / 2 + Window_Height / 4;	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {		if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {			Rectangle_red = uid(dre);			Rectangle_green = uid(dre);			Rectangle_blue = uid(dre);		}		else {			red = uid(dre);			green = uid(dre);			blue = uid(dre);		}	}	glutPostRedisplay();}