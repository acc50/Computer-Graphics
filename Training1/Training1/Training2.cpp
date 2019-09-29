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
void Mouse(int button, int state, int x, int y);

int Window_Width = 800;
int Window_Height = 600;

float red = 1.0f, green = 1.0f, blue = 1.0f;
float Rectangle_red = 0.0f, Rectangle_green = 0.0f, Rectangle_blue = 0.0f;

int main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(Window_Width, Window_Height); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성

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
	glutMouseFunc(Mouse);
	glutMainLoop();										 // 이벤트 처리 시작 
}

GLvoid drawScene()
{
	glClearColor(red, green, blue, 1.0f); // 바탕색을 ‘white’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기


	// 그리기 부분 구현

	glColor3f(Rectangle_red, Rectangle_green, Rectangle_blue);
	glRectf(-0.5f, -0.5f, +0.5f, +0.5f);		// 윈도우창 중앙이 0.0f
												// -1.0f ~ 1.0f 까지가 범위


	//--- 그리기 관련 부분이 여기에 포함된다


	glFlush(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기
{
	glViewport(0, 0, w, h);
}void Mouse(int button, int state, int x, int y){	int x1, x2, y1, y2;		x1 = Window_Width / 2 - Window_Width / 4;	y1 = Window_Height / 2 - Window_Height / 4;	x2 = Window_Width / 2 + Window_Width / 4;	y2 = Window_Height / 2 + Window_Height / 4;	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {		if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {			Rectangle_red = uid(dre);			Rectangle_green = uid(dre);			Rectangle_blue = uid(dre);		}		else {			red = uid(dre);			green = uid(dre);			blue = uid(dre);		}	}	glutPostRedisplay();}