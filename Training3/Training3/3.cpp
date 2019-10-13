#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "file2buf.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_real_distribution<float> random_Pos(-0.2f, 0.2f);
uniform_real_distribution<float> random_color(0.0f, 1.0f);

float quadrant1 = random_Pos(dre), quadrant2 = random_Pos(dre),
	  quadrant3 = random_Pos(dre), quadrant4 = random_Pos(dre);

int Window_Width = 1200, Window_Height = 1000;
int n = 0, i = 0;
float r[4], g[4], b[4];

GLfloat TriShape[4][3][3] = {
	{{-0.5f + quadrant2, 0.5f + quadrant2 + 0.15f, 0.0f},
	{-0.5f + quadrant2 - 0.15f , 0.5f + quadrant2 - 0.15f, 0.0f},
	{-0.5f + quadrant2 + 0.15f, 0.5f + quadrant2 - 0.15f, 0.0f}},

	{{0.5f + quadrant1, 0.5f + quadrant1 + 0.15f, 0.0f},
	{0.5f + quadrant1 - 0.15f, 0.5f + quadrant1 - 0.15f, 0.0f},
	{0.5f + quadrant1 + 0.15f, 0.5f + quadrant1 - 0.15f, 0.0f}},

	{{-0.5f + quadrant3, -0.5f + quadrant3 + 0.15f, 0.0f},
	{-0.5f + quadrant3 - 0.15f, -0.5f + quadrant3 - 0.15f, 0.0f},
	{-0.5f + quadrant3 + 0.15f, -0.5f + quadrant3 - 0.15f, 0.0f}},

	{{0.5f + quadrant4, -0.5f + quadrant4 + 0.15f, 0.0f},
	{0.5f + quadrant4 - 0.15f, -0.5f + quadrant4 - 0.15f, 0.0f},
	{0.5f + quadrant4 + 0.15f, -0.5f + quadrant4 - 0.15f, 0.0f}}
};

GLuint compile_shader()
{
	GLfloat colors[4][3][3] = {
		{{r[0], g[0], b[0]},
		{r[0], g[0], b[0]},
		{r[0], g[0], b[0]}},

		{{r[1], g[1], b[1]},
		{r[1], g[1], b[1]},
		{r[1], g[1], b[1]}},

		{{r[2], g[2], b[2]},
		{r[2], g[2], b[2]},
		{r[2], g[2], b[2]}},

		{{r[3], g[3], b[3]},
		{r[3], g[3], b[3]},
		{r[3], g[3], b[3]}},
	};

	GLuint vao, vbo[2];
	GLuint ShaderProgramID;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), TriShape[n], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors[n], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);




	const GLchar* vertexsource = filetobuf("Vertex.txt");
	const GLchar* fragmentssource = filetobuf("frag.txt");
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR:vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentssource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR:fragmentShader 컴파일 실패\n" << errorLog << std::endl;
		return false;
	}
	ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);

	glBindAttribLocation(ShaderProgramID, 0, "vPos");
	glBindAttribLocation(ShaderProgramID, 1, "vColor");

	glLinkProgram(ShaderProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "EROOR:shader program연결실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(ShaderProgramID);

	glBindVertexArray(vao);

	return ShaderProgramID;

}

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

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


	for (int k = 0; k < 4; k++) {
		r[k] = random_color(dre);
		g[k] = random_color(dre);
		b[k] = random_color(dre);
	}

	glutDisplayFunc(drawScene);						 	 // 출력 함수의 지정
	glutReshapeFunc(Reshape);							 // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutMainLoop();										 // 이벤트 처리 시작 
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기


	// 그리기 부분 구현
	
	GLuint ShaderProgramID[4];

	for (n = 0; n < 4; n++) {
		ShaderProgramID[n] = compile_shader();
		glUseProgram(ShaderProgramID[n]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	GLfloat x1, y1;	float x_size = Window_Width / 2, y_size = Window_Height / 2;

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

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		TriShape[i][0][0] = x1;
		TriShape[i][0][1] = y1 + 0.15f;
		TriShape[i][1][0] = x1 - 0.15f;
		TriShape[i][1][1] = y1 - 0.15f;
		TriShape[i][2][0] = x1 + 0.15f;
		TriShape[i][2][1] = y1 - 0.15f;
		i = (i + 1) % 4;
	}

	glutPostRedisplay();
}