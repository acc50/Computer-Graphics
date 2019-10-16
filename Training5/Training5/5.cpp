#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "file2buf.h"			// 쉐이더 파일 읽기 헤더
#include "OpenGL.h"				// OpenGL의 라이브러리가 담긴 헤더
#include "Object_Circle.h"		// Circle의 클래스가 담긴 헤더

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_real_distribution<float> random_radius(0.03f, 0.1f);		// 클릭시 원의 초기 반지름 랜덤 생성
uniform_real_distribution<float> random_color(0.0f, 1.0f);	// 원의 색상 랜덤 생성

int Window_Width = 1200, Window_Height = 1000;

Circle *circles = new Circle[10];		// 클래스로 만든 원을 10개 동적할당

int n = 0;			// 현재 몇번째 원인지 알 수 있는 변수
int cnt = 0;		// 원의 갯수

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
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

	glutDisplayFunc(drawScene);						 	 // 출력 함수의 지정
	glutReshapeFunc(Reshape);							 // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();										 // 이벤트 처리 시작 
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기


	// 그리기 부분 구현

	for (int i = 0; i < cnt; ++i) {	
		circles[i].draw();				//  원을 원의 갯수만큼 그려줌
	}

	//--- 그리기 관련 부분이 여기에 포함된다

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기
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

	//	-- 위의 작업은 마우스 좌표를 -1.0f ~ 1.0f 의 뷰포트 공간좌표로 바꿈

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {			// 왼쪽클릭시 n번째 원의 초기 생성
		circles[n].setCircle(x1, y1, random_color(dre),
			random_color(dre), random_color(dre), random_radius(dre));

		glutTimerFunc(100, Timer, n);		// n번째 원의 애니메이션 타이머 설정

		n = (n + 1) % 10;			// 원은 10개가 유지되어야 하고 10개가 넘어가면 처음으로 돌아감
		cnt = cnt + 1;
		if (cnt > 10)			// 원의 갯수는 10개를 넘어갈 수 없음
			cnt = 10;
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {			// 원의 그리기 타입 변경
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

void Timer(int value)		// 원의 반지름 변화, 반지름이 일정이상 넘어가면 초기화 작업을 해줌
{
	circles[value].update();
	circles[value].physics_update();
	glutPostRedisplay();
	glutTimerFunc(100, Timer, value);		// n번째 원의 타이머를 계속해서 부름
}