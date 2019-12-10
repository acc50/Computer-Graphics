#include <iostream>
#include <Windows.h>
#include "file2buf.h"
#include "InitApp.h"
#include "openGL.h"
#include "snow.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint boardEBO, boardVBO;
GLuint lVBO, lEBO;
GLuint eVBO, eEBO;
GLuint vVBO, vEBO;
GLuint mVBO, mEBO;
GLuint CVBO, CEBO;
GLuint sVBO[20], sEBO[20];

float angle = 0;

bool z_rotate = false;
GLfloat light_z_angle = 0.0f;
bool light_switch = true;

bool is_move = true;
int move_dir = 1;

bool is_camera_rotate = false;
bool is_standard_camera = false;

GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;

snow *snows = new snow[20];


glm::vec3 CameraPos = glm::vec3(0.0f, 0.6f, 3.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec4 LightPos = glm::vec4(-4.0, 1.0, 0.0, 1.0);		// ���� ��ġ
glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

GLvoid draw_board();
GLvoid draw_polygon();
GLvoid draw_earth();
GLvoid draw_mercury();
GLvoid draw_venus();
GLvoid draw_cone();

void Timer(int a);
void z_Rotate_Timer(int a);
void Camera_Timer(int a);
void InputKey(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("17");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	CreateBoard(boardEBO, boardVBO);
	CreateSphere(eEBO, eVBO);
	CreateSphere(mEBO, mVBO);
	CreateSphere(vEBO, vVBO);
	CreateCone(CEBO, CVBO);

	for (int i = 0; i < 20; ++i) {
		CreateSphere(sEBO[i], sVBO[i]);
	}

	for (int i = 0; i < 20; ++i) {
		snows[i].Init();
	}


	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(InputKey);
	glutSpecialFunc(SpecialKey);
	glutTimerFunc(10, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Timer(int a)
{
	angle = angle + 1;
	
	for (int i = 0; i < 20; ++i)
		snows[i].Fall();

	glutPostRedisplay();

	glutTimerFunc(30, Timer, a);
}

void z_Rotate_Timer(int a)
{
	if (z_rotate)
		light_z_angle = 1.0f;

	glutPostRedisplay();

	if(z_rotate)
		glutTimerFunc(20, z_Rotate_Timer, a);
}

void Camera_Timer(int a)
{
	if (is_camera_rotate) {
		camera_y += 0.5f;
	}

	glutPostRedisplay();

	if (is_camera_rotate)
		glutTimerFunc(20, Camera_Timer, a);
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);


	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");

	glm::mat4 view = glm::mat4(1.0f);		// �亯ȯ
	view = glm::lookAt(glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z), glm::vec3(AT.x, AT.y, AT.z), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(camera_y), glm::vec3(0.0f, 1.0f, 0.0f));
	
	
	glm::mat4 itx = glm::mat4(1.0f);
	itx = glm::translate(itx, glm::vec3(-CameraPos.x, -CameraPos.y, -CameraPos.z));		// ����ȯ

	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z));		// ���ڸ� ��ȯ
	
	view = view * rotate;


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// �亯ȯ


	glm::mat4 projection = glm::mat4(1.0f);		// ������ȯ
	projection = glm::perspective(glm::radians(120.0f), (float)WIDTH / (float)HEIGHT, 0.0f, 2000.0f);	// ���װ���, ��Ⱦ��, �����Ÿ�, �հŸ�
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// ������ȯ

	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(light_z_angle), glm::vec3(0.0f, 0.0f, 1.0f));
	LightPos = LightPos * r;

	int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos");
	glUniform3f(viewPosLocation, CameraPos.x, CameraPos.y, CameraPos.z);
	int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos");
	glUniform3f(lightPosLocation, LightPos.x, LightPos.y, LightPos.z);
	int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor");
	glUniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b);									// �����

	draw_board();
	draw_polygon();
	draw_cone();
	draw_earth();
	draw_mercury();
	draw_venus();

	for (int i = 0; i < 20; ++i)
		snows[i].Render(ShaderProgram, sVBO[i], sEBO[i]);

	glutSwapBuffers();


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

GLvoid draw_board()
{
	// ��� ��ȯ�� �޾ƿ�
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 rm = glm::mat4(1.0f);
	rm = glm::rotate(rm, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//model = model * rm;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ



	glBindBuffer(GL_ARRAY_BUFFER, boardVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boardEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.5, 0.5, 0.5);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_polygon()
{
	GLfloat vertex[] = {
		-0.2f,  -0.2f,  0.0f,		-1.0f, -1.0f, 0.0f,  //0����
		-0.2f,  0.2f,  0.0f,		-1.0f, 1.0f, 0.0f,  //1����
		 0.2f,  0.2f,  0.0f,		1.0f, 1.0f, 0.0f,  //2����
		 0.2f,  -0.2f,  0.0f,		1.0f, -1.0f, 0.0f   //3����
	};

	glGenBuffers(1, &lVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLint gIndices[]
	{
		0,2,1,
		0,3,2
	};

	glGenBuffers(1, &lEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);


	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	tx = glm::translate(tx, glm::vec3(LightPos.x, LightPos.y, LightPos.z));

	model = model * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_earth()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	rm = glm::rotate(rm, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tx = glm::translate(tx, glm::vec3(1.5f, 0.0f, 0.0f));


	model = model * rm * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ



	glBindBuffer(GL_ARRAY_BUFFER, eVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.5f, 0.5f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 2880, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_mercury()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 rx = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	rx = glm::rotate(rx, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	rm = glm::rotate(rm, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tx = glm::translate(tx, glm::vec3(2.0f, 1.0f, 0.0f));

	glm::mat4 t = glm::mat4(1.0f);
	t = glm::translate(t, glm::vec3(-1.0f, 0.0f, -1.0f));

	model = model * t * rx * rm * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ



	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.5f, 0.5f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 2880, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_venus()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 rx = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	rx = glm::rotate(rx, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	rm = glm::rotate(rm, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tx = glm::translate(tx, glm::vec3(2.0f, 1.0f, 0.0f));

	model = model * rx * rm * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ



	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.5f, 0.5f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 2880, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_cone()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");


	glm::mat4 model = glm::mat4(1.0f); //����
	glm::mat4 rm = glm::mat4(1.0f); //ȸ��


	//model = model * rm;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glBindBuffer(GL_ARRAY_BUFFER, CVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));//3��° ���ڴ� ���������� �󸶳� ��������, �ǵڿ� ���ڴ� ��� �����ұ� x,y,z,r,g,b,�ϱ�  3�������ͽ����ؼ� 6ĭ�������� ����������ġ
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void InputKey(unsigned char key, int x, int y)
{
	GLfloat step = 0.5f;

	switch (key)
	{
	case'm':
		if (light_switch) {
			light_switch = false;
			lightColor.r = 0.0f; 
			lightColor.g = 0.0f; 
			lightColor.b = 0.0f;
		}
		else {
			light_switch = true;
			lightColor.r = 1.0f;
			lightColor.g = 1.0f;
			lightColor.b = 1.0f;
		}

		break;



	case's':
		
		break;



	case 'r':

		if (z_rotate) {
			z_rotate = false;
		}

		else {
			z_rotate = true;
			glutTimerFunc(10, z_Rotate_Timer, 1);
		}

		break;

	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
	GLfloat step = 0.5f;
	switch (key)
	{
	case GLUT_KEY_DOWN:
		CameraPos.z += step;
		break;

	case GLUT_KEY_UP:
		CameraPos.z -= step;
		break;

	case GLUT_KEY_RIGHT:
		CameraPos.x += step;
		AT.x += step;
		break;

	case GLUT_KEY_LEFT:
		CameraPos.x -= step;
		AT.x -= step;
		break;
	}

	glutPostRedisplay();
}