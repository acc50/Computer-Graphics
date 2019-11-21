#include <iostream>
#include <Windows.h>
#include <vector>
#include "file2buf.h"
#include "InitApp.h"
#include "OpenGL.h"
#include "Polygon.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

bool rotate = false;
bool lineOrFill = true;

void Timer(int a);
void Keyboard(unsigned char key, int x, int y);

GLuint ShaderProgram;

GLuint VBO;
GLuint EBO;

GLuint routeVBO;

Polygons *polygon = new Polygons;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitProgram(ShaderProgram);
	CreateTriangle(EBO, VBO);

	polygon->Set_Polygons();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Timer(int a)
{
	polygon->Update();
	
	if (polygon->Get_I() >= RATE) {
		delete polygon;
	}

	glutPostRedisplay();
	glutTimerFunc(100, Timer, 1);
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ShaderProgram);

	polygon->Render(VBO, EBO, ShaderProgram);

	glutSwapBuffers();


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key){
	case 'c':
		glutTimerFunc(100, Timer, 1);
		break;

	case 'A':
		
		break;

	default:
		break;
	}
}