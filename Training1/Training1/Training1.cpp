// Training1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);

float red = 1.0f, green = 1.0f, blue = 1.0f;
bool TimerSwitch = false;

int main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{ 
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLE" << std::endl;
		;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene);						 	 // 출력 함수의 지정
	glutReshapeFunc(Reshape);							 // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop();										 // 이벤트 처리 시작 
}

GLvoid drawScene()
{ 
	glClearColor( red, green, blue, 1.0f ); // 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기


	// 그리기 부분 구현

	//--- 그리기 관련 부분이 여기에 포함된다



	glFlush(); // 화면에 출력하기
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
	case 'R':
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;

		break;

	case 'g':
	case 'G':
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;

		break;

	case 'b':
	case 'B':
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;

		break;

	case 'a':
	case 'A':
		red = uid(dre);
		green = uid(dre);
		blue = uid(dre);

		break;

	case 'w':
	case 'W':
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;

		break;

	case 'k':
	case 'K':
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;

		break;

	case 't':
	case 'T':
		glutTimerFunc(500, Timer, 1);
		TimerSwitch = true;

		break;

	case 's':
	case 'S':
		TimerSwitch = false;

		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void Timer(int value)
{
	if (TimerSwitch) {
		red = uid(dre);
		green = uid(dre);
		blue = uid(dre);

		glutPostRedisplay();
		glutTimerFunc(500, Timer, 1);
	}
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기
{
	glViewport(0, 0, w, h);
}