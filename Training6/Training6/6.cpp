#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "file2buf.h"			// ���̴� ���� �б� ���
#include "OpenGL.h"				// OpenGL�� ���̺귯���� ��� ���
#include "Object_Triangle.h"		// Circle�� Ŭ������ ��� ���

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

int Window_Width = 1200, Window_Height = 1000;

Triangle *triangles = new Triangle[10];		// Ŭ������ ���� ���� 10�� �����Ҵ�

int n = 10;			// �ﰢ���� ����

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
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

	for (int i = 0; i < n; ++i)
		triangles[i].setTriangle();

	glutDisplayFunc(drawScene);						 	 // ��� �Լ��� ����
	glutReshapeFunc(Reshape);							 // �ٽ� �׸��� �Լ� ����

	for (int i = 0; i < n; ++i)
		glutTimerFunc(100, Timer, i);
	
	glutMainLoop();										 // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�


	// �׸��� �κ� ����

	for (int i = 0; i < n; ++i) {	
		triangles[i].draw();				//  ���� ���� ������ŭ �׷���
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) // �ݹ� �Լ�: �ٽ� �׸���
{
	glViewport(0, 0, w, h);
}

void Timer(int value)		// ���� ������ ��ȭ, �������� �����̻� �Ѿ�� �ʱ�ȭ �۾��� ����
{
	triangles[value].update();
	triangles[value].physics_update();
	glutPostRedisplay();
	glutTimerFunc(100, Timer, value);		// n��° ���� Ÿ�̸Ӹ� ����ؼ� �θ�
}