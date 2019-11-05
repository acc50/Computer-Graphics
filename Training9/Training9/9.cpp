#include <cmath>
#include <stdio.h>
#include "file2buf.h"			// 쉐이더 파일 읽기 헤더
#include "OpenGL.h"				// OpenGL의 라이브러리가 담긴 헤더

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

int draw_type = 0;
int draw_model = 1;

GLfloat cube_vertices[][3] =
{
	{-1.0, -1.0, -1.0},	// vertex 0
	{1.0, -1.0, -1.0},	// vertex 1
	{1.0, 1.0, -1.0},	// vertex 2
	{-1.0, 1.0, -1.0},	// vertex 3
	{-1.0, -1.0, 1.0},	// vertex 4
	{1.0, -1.0, 1.0},	// vertex 5
	{1.0, 1.0, 1.0},	// vertex 6
	{-1.0, 1.0, 1.0},	// vertex 7
};

GLfloat cube_colors[][3] =
{
	{0.0,0.0,0.0},		//color 0
	{1.0,0.0,0.0},		//color 1
	{1.0,1.0,0.0},		//color 2
	{0.0,1.0,0.0},		//color 3
	{0.0,0.0,1.0},		//color 4
	{1.0,0.0,1.0},		//color 5
	{1.0,1.0,1.0},		//color 6
	{0.0,1.0,1.0}		//color 7	
};

GLfloat pyramid_vertices[][3] =
{
	{-1.0, -1.0, -1.0},	// vertex 0
	{1.0, -1.0, -1.0},	// vertex 1
	{1.0, -1.0, 1.0},	// vertex 2
	{-1.0, -1.0, 1.0},	// vertex 3
	{0.0, 1.0, 0.0}		// vertex 4
};

GLfloat pyramid_colors[][3] =
{
	{0.0,0.0,0.0},		//color 0
	{1.0,0.0,0.0},		//color 1
	{1.0,1.0,0.0},		//color 2
	{0.0,1.0,0.0},		//color 3
	{0.0,0.0,1.0},		//color 4
};

GLfloat matrix[16];

static int window_width = 1000;	//window size
static int window_height = 1000;

void Draw_Axis()
{
	float length = 3.0f;
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	//X-axis (red)
	glColor3f(1, 0, 0);
	glVertex3f(-length, 0, 0);
	glVertex3f(length, 0, 0);

	//Y-axis (green)
	glColor3f(0, 1, 0);
	glVertex3f(0, -length, 0);
	glVertex3f(0, length, 0);

	//Z-axis (blue)
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -length);
	glVertex3f(0, 0, length);

	glEnd();
}

void cube_polygon(int a, int b, int c, int d)
{
	if (draw_type == 0) {
		glBegin(GL_TRIANGLES);
	}
	else if (draw_type == 1) {
		glBegin(GL_LINE_STRIP);
	}
	{
		glColor3fv(cube_colors[a]);
		glVertex3fv(cube_vertices[a]);
		glColor3fv(cube_colors[b]);
		glVertex3fv(cube_vertices[b]);
		glColor3fv(cube_colors[c]);
		glVertex3fv(cube_vertices[c]);
	}
	glEnd();

	if (draw_type == 0) {
		glBegin(GL_TRIANGLES);
	}
	else if (draw_type == 1) {
		glBegin(GL_LINE_STRIP);
	}
	{
		glColor3fv(cube_colors[a]);
		glVertex3fv(cube_vertices[a]);
		glColor3fv(cube_colors[c]);
		glVertex3fv(cube_vertices[c]);
		glColor3fv(cube_colors[d]);
		glVertex3fv(cube_vertices[d]);
	}
	glEnd();
}

void colorcube()
{
	cube_polygon(4, 5, 6, 7);
	cube_polygon(0, 3, 2, 1);
	cube_polygon(1, 2, 6, 5);
	cube_polygon(0, 4, 7, 3);
	cube_polygon(3, 7, 6, 2);
	cube_polygon(0, 1, 5, 4);
}

void cube_pyramid(int a, int b, int c)
{
	if (draw_type == 0) {
		glBegin(GL_TRIANGLES);
	}
	else if (draw_type == 1) {
		glBegin(GL_LINE_STRIP);
	}
	{
		glColor3fv(pyramid_colors[a]);
		glVertex3fv(pyramid_vertices[a]);
		glColor3fv(pyramid_colors[b]);
		glVertex3fv(pyramid_vertices[b]);
		glColor3fv(pyramid_colors[c]);
		glVertex3fv(pyramid_vertices[c]);
	}
	glEnd();

}

void colorpyramid()
{
	cube_pyramid(0, 1, 2);
	cube_pyramid(2, 3, 0);
	cube_pyramid(3, 2, 4);
	cube_pyramid(2, 1, 4);
	cube_pyramid(1, 0, 4);
	cube_pyramid(0, 3, 4);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Axis();

	glPushMatrix();
	
	//glTranslatef(1.0f, 1.0f, 0.0f);
	glRotatef(30.0f, 1.0f, -1.0f, 0.0f);
	glTranslatef(0.0f, 0.5f, 0.0f);

	if (draw_model == 0) {
		colorcube();
	}
	else if (draw_model == 1) {
		colorpyramid();
	}
	glPopMatrix();


	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glViewport(0, 0, w, h);

	window_width = w;
	window_height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		glFrustum(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w, 2.0* (GLfloat)h / (GLfloat)w, 2.0, 20.0);
	}
	else
	{
		glFrustum(-2.0, 2.0, -2.0 * (GLfloat)w / (GLfloat)h, 2.0* (GLfloat)w / (GLfloat)h, 2.0, 20.0);
	}

	glTranslatef(0, 0, -5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

void idleFunc()
{
	glutPostRedisplay();
}

bool plus_timer_switch = false, minus_timer_switch = false;
int timer_direction = 1;
bool culling = false;

void Timer(int direction)
{
	glRotatef(3.0, 0, direction, 0);

	if (plus_timer_switch)
		glutTimerFunc(50, Timer, 1);

	if (minus_timer_switch)
		glutTimerFunc(50, Timer, -1);
}

void keyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		draw_model = 0;
		break;

	case 'p':
		draw_model = 1;
		break;

	case 'h':
		//glEnable(GL_FRONT_AND_BACK);
		if (!culling) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			culling = true;
		}
		else {
			glDisable(GL_CULL_FACE);
			culling = false;
		}
		break;

	case 'y':

		if (timer_direction == 1) {
			plus_timer_switch = true;
			minus_timer_switch = false;
			glutTimerFunc(50, Timer, 1);
			timer_direction = -1;
		}
		else if(timer_direction == -1){
			plus_timer_switch = false;
			minus_timer_switch = true;
			glutTimerFunc(50, Timer, -1);
			timer_direction = 1;
		}

		break;

	case 'Y':

		plus_timer_switch = false;
		minus_timer_switch = false;

		break;

	case 'w':
	case 'W':
		draw_type = 1 - draw_type;
		break;
	}

	glutPostRedisplay();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("OpenGL ch03 : 3D Graphics");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idleFunc);

	glutKeyboardFunc(keyFunc);

	glutMainLoop();
	return 0;
}