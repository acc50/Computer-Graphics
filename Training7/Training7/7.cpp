#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "file2buf.h"			// 쉐이더 파일 읽기 헤더
#include "OpenGL.h"				// OpenGL의 라이브러리가 담긴 헤더
#include "Object_Triangle.h"		// Circle의 클래스가 담긴 헤더
#include "Object_Inner_Triangle.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

int Window_Width = 1200, Window_Height = 1000;

Triangle *triangles = new Triangle[10];		// 클래스로 만든 원을 10개 동적할당
Inner_Triangle *Inner_triangles = new Inner_Triangle[2];

int n = 10;			// 삼각형의 갯수

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Timer(int value);

int main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(Window_Width, Window_Height); // 윈도우의 크기 지정
	glutCreateWindow("Training3"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLE" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	for (int i = 0; i < n; ++i)
		triangles[i].setTriangle();

	for (int i = 0; i < 2; ++i)
		Inner_triangles[i].setTriangle();

	glutDisplayFunc(drawScene);						 	 // 출력 함수의 지정
	glutReshapeFunc(Reshape);							 // 다시 그리기 함수 지정

	for (int i = 0; i < n; ++i)
		glutTimerFunc(100, Timer, i);
	
	glutMainLoop();										 // 이벤트 처리 시작 
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기


	// 그리기 부분 구현

	for (int i = 0; i < n; ++i) {	
		triangles[i].draw();
	}

	for (int i = 0; i < 2; ++i)
		Inner_triangles[i].draw();

	glColor3f(0.0f, 0.0f, 0.0f);		// 색상 설정

	glLineWidth(3.0f);

	glPushMatrix();								// 행렬에 넣는 함수

	{
		glBegin(GL_LINE_STRIP);
		glLineWidth(2.0f);

		glVertex2f(-0.3f, -0.3f);
		glVertex2f(-0.3f, 0.3f);
		glVertex2f(0.3f, 0.3f);
		glVertex2f(0.3f, -0.3f);
		glVertex2f(-0.3f, -0.3f);

		glEnd();
	}

	glPopMatrix();

	//--- 그리기 관련 부분이 여기에 포함된다

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기
{
	glViewport(0, 0, w, h);
}

void Timer(int value)		// 원의 반지름 변화, 반지름이 일정이상 넘어가면 초기화 작업을 해줌
{
	triangles[value].update();
	triangles[value].physics_update();

	if (value < 2) {
		Inner_triangles[value].update();
		Inner_triangles[value].physics_update();
	}

	glutPostRedisplay();
	glutTimerFunc(50, Timer, value);		// n번째 원의 타이머를 계속해서 부름
}