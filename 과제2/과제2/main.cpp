#include <iostream>
#include <Windows.h>
#include "file2buf.h"
#include "InitApp.h"
#include "openGL.h"
#include "Robot.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 1000
#define HEIGHT 800

enum Dir{ STOP = 0, FRONT, BACK, LEFT, RIGHT };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid draw_board();
GLvoid draw_cube();
GLvoid draw_obstacle();
void Timer(int a);
void Camera_Move_Timer(int a);
void Camera_Rotate_Timer(int a);
void InputKey(unsigned char key, int x, int y);
void KeyUP(unsigned char key, int x, int y);

GLuint ShaderProgram;
GLuint RobotEBO[4], RobotVBO[4];
GLuint boardEBO, boardVBO;
GLuint CubeEBO, CubeVBO;
GLuint OEBO, OVBO;

glm::vec3 EYE = glm::vec3(0.0f, 1.0f, 6.0f);
glm::vec3 AT = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 T_EYE = glm::vec3(EYE.x, EYE.y + 1.0f, EYE.z + 2.0f);
glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

Robot *robots = new Robot[4];
GLint TIMER_SPEED = 40;
GLint CAMERA_TIMER_SPEED = 50;
bool is_camera_move = false;		// ī�޶��� �̵�
bool is_camera_rotate = false;		// ī�޶��� ȸ��
bool is_at_rotate = false;			// at�� ȸ��
bool is_third_person_view = false;	// 3��Ī ����
int angle_sign = 1;
int dir = 0;
int angle = 0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
	CreateRobot(RobotEBO, RobotVBO);
	CreateCube(CubeEBO, CubeVBO);
	CreateObstacle(OEBO, OVBO);

	for (int i = 0; i < 4; ++i)
		robots[i].Set_Robot(i + 1);

	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(InputKey);
	glutKeyboardUpFunc(KeyUP);
	glutTimerFunc(10, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Timer(int a)
{
	// -------------------------  �κ� �̵� �κ� ---------------------------

	for (int i = 0; i < 4; ++i) {
		robots[i].Move(EYE.x, EYE.y, EYE.z, AT.x, AT.y, AT.z);
	}

	// -------------------------  �κ� �̵� �κ� ---------------------------


	// ------------------------- �κ��� ��ֹ� üũ ------------------------

	glm::vec3 r_pos[4];

	GLfloat x, z;

	for (int i = 0; i < 4; ++i) {
		r_pos[i] = robots[i].get_Pos();

		x = r_pos[i].x;
		z = r_pos[i].z;

		// -------------------------- �浹�� �Ͼ�� ���� ----------------------------
		if (x > LENGTH + 0.3f) {
			continue;
		}

		else if (x < -LENGTH - 0.3f) {					// ����� ���� + ��ü�� ���� 0.3f
			continue;
		}

		else if (z > LENGTH + 0.3f) {
			continue;
		}

		else if (z < -LENGTH - 0.3f) {
			continue;
		}
		// -------------------------- �浹�� �Ͼ�� ���� ----------------------------

		// �浹�� �Ͼ�� ���� �����̸� �Ʒ��� �������� �ʰ� ���� for�� �������� �Ѿ




		// ------------------------- ���� for�� ���� ����� �浹 --------------------------
		
		if (x > LENGTH + 0.15f) {					// ����� �����ʿ��� �浹
			robots[i].set_Pos_x((GLfloat)LENGTH + 0.3f);
		}

		else if(x < -LENGTH - 0.15f) {					// ����� ���ʿ��� �浹
			robots[i].set_Pos_x(-(GLfloat)LENGTH - 0.3f);
		}

		if (z > LENGTH + 0.15f) {		// ����� ���ʿ��� �浹
			robots[i].set_Pos_z((GLfloat)LENGTH + 0.3f);
		}

		else if (z < -LENGTH - 0.15f) {					// ����� ���ʿ��� �浹
			robots[i].set_Pos_z(-(GLfloat)LENGTH - 0.3f);
		}

		robots[i].Set_MovePoint();

		// --------------------		�浹 ó�� -------------------------------------
		

	}



	// ------------------------- �κ��� ��ֹ� üũ ------------------------


	glutPostRedisplay();
	glutTimerFunc(TIMER_SPEED, Timer, a);
}

void Camera_Move_Timer(int a)
{

	// -------------------------  ī�޶� �̵� �κ� ---------------------------
	
	
	// ���� AT �� EYE �� �������� �ΰ� ������ ����� �Ÿ� ���

	GLfloat step = 10;

	glm::vec3 front_dir = AT - EYE;
	front_dir = front_dir / (sqrt(front_dir.x * front_dir.x + front_dir.y * front_dir.y + front_dir.z * front_dir.z));
	glm::vec3 back_dir = glm::vec3(-front_dir.x, -front_dir.y, -front_dir.z);
	
	glm::vec3 left_dir = glm::vec3(1.0f);		// UP X front_dir
	left_dir.x = UP.y * front_dir.z - UP.z * front_dir.y;
	left_dir.y = UP.z * front_dir.x - UP.x * front_dir.z;
	left_dir.z = UP.x * front_dir.y - UP.y * front_dir.x;

	glm::vec3 right_dir = glm::vec3(-left_dir.x, -left_dir.y, -left_dir.z);


	
	if (dir == FRONT) {
		EYE += front_dir / step;
		AT += front_dir / step;
	}

	else if (dir == BACK) {
		EYE += back_dir / step;
		AT += back_dir / step;
	}

	else if (dir == LEFT) {
		EYE += left_dir / step;
		AT += left_dir / step;
	}

	else if (dir == RIGHT) {
		EYE += right_dir / step;
		AT += right_dir / step;
	}

	T_EYE = glm::vec3(EYE.x - front_dir.x * 2, EYE.y + 1.0f, EYE.z - front_dir.z * 2);	// 3��Ī ���� ���� EYE ��ġ ����

	// -------------------------  ī�޶� �̵� �κ� ---------------------------


	glutPostRedisplay();

	if (is_camera_move)
		glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, a);
}

void Camera_Rotate_Timer(int a)
{

	// -------------------------  ī�޶� ȸ�� �κ� ---------------------------

	if (is_camera_rotate) {

		glm::mat4 r = glm::mat4(1.0f);

		r = glm::rotate(r, glm::radians(angle_sign * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat3 rm = glm::mat3(r);

		EYE = EYE * rm;

		T_EYE = T_EYE * rm;
	}

	if (is_at_rotate) {

		// AT �� EYE ���� �������� ȸ��

		glm::mat4 r = glm::mat4(1.0f);

		r = glm::rotate(r, glm::radians(angle_sign * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat3 rm = glm::mat3(r);
		
		AT = AT - EYE;			// EYE �� �����̵�
		AT = AT * rm;			// AT �� ȸ��
		AT = AT + EYE;			// �̵��ߴ� ��ŭ �ٽ� ���̵�

		T_EYE = T_EYE - EYE;
		T_EYE = T_EYE * rm;
		T_EYE = T_EYE + EYE;

	}




	// -------------------------  ī�޶� ȸ�� �κ� ---------------------------

	glutPostRedisplay();

	if (is_camera_rotate || is_at_rotate)
		glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Rotate_Timer, a);
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(ShaderProgram);


	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");

	glm::mat4 view = glm::mat4(1.0f);		// �亯ȯ

	if (is_third_person_view)
		view = glm::lookAt(T_EYE, AT, UP);

	else
		view = glm::lookAt(EYE, AT, UP);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// �亯ȯ


	glm::mat4 projection = glm::mat4(1.0f);		// ������ȯ
	projection = glm::perspective(glm::radians(120.0f), (float)WIDTH / (float)HEIGHT, 0.0f, 2000.0f);	// ���װ���, ��Ⱦ��, �����Ÿ�, �հŸ�
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// ������ȯ

	draw_board();
	for (int i = 0; i < 4; ++i)
		robots[i].Render(ShaderProgram, RobotEBO[i], RobotVBO[i]);
	draw_cube();
	draw_obstacle();

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

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}

GLvoid draw_cube()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ
	glm::mat4 tx = glm::mat4(1.0f);
	glm::mat4 rm = glm::mat4(1.0f);


	glm::vec3 front_dir = AT - EYE;
	front_dir = front_dir / (sqrt(front_dir.x * front_dir.x + front_dir.y * front_dir.y + front_dir.z * front_dir.z));

	tx = glm::translate(tx, glm::vec3(EYE.x - front_dir.x / 4, EYE.y, EYE.z - front_dir.z / 4));

	if(is_camera_rotate)
		rm = glm::rotate(rm, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	model = model * tx * rm;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ



	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeEBO);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}

GLvoid draw_obstacle()
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// �𵨺�ȯ

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ

	glBindBuffer(GL_ARRAY_BUFFER, OVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OEBO);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glEnableVertexAttribArray(frag_id);
	glVertexAttribPointer(frag_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(pos_id);
	glDisableVertexAttribArray(frag_id);
}


void InputKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		if (!is_camera_move) {
			dir = FRONT;

			is_camera_move = true;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, 1);
		}

		break;

	case 'a':
		if (!is_camera_move) {
			dir = LEFT;

			is_camera_move = true;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, 1);
		}

		break;

	case 's':
		if (!is_camera_move) {
			dir = BACK;

			is_camera_move = true;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, 1);
		}

		break;

	case'd':
		if (!is_camera_move) {
			dir = RIGHT;

			is_camera_move = true;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, 1);
		}

		break;


	case 'y':			// ī�޶� ���� ȸ��

		if (!is_at_rotate && !is_camera_rotate) {
			is_at_rotate = true;
			angle_sign = 1;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Rotate_Timer, 1);

		}


		break;

	case 'Y':

		if (!is_at_rotate && !is_camera_rotate) {
			is_at_rotate = true;
			angle_sign = -1;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Rotate_Timer, 1);

		}

		break;

	case 'c':			// ���� ���� ȸ��

		if (!is_camera_rotate && !is_at_rotate) {
			is_camera_rotate = true;
			angle_sign = 1;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Rotate_Timer, 1);
		}

		break;

	case 'C':

		if (!is_camera_rotate && !is_at_rotate) {
			is_camera_rotate = true;
			angle_sign = -1;
			glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Rotate_Timer, 1);
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

void KeyUP(unsigned char key, int x, int y)
{
	GLfloat step = 0.5f;

	switch (key)
	{
	case 'w':
		if (dir == FRONT) {
			dir = STOP;
			is_camera_move = false;
		}

		break;

	case 'a':
		if (dir == LEFT) {
			dir = STOP;
			is_camera_move = false;
		}

		break;

	case 's':
		if (dir == BACK) {
			dir = STOP;
			is_camera_move = false;
		}

		break;

	case'd':
		if (dir == RIGHT) {
			dir = STOP;
			is_camera_move = false;
		}
		break;

	case 'y':			// ī�޶� ���� ȸ��

		is_at_rotate = false;

		break;

	case 'Y':

		is_at_rotate = false;
		break;

	case 'c':			// ���� ���� ȸ��

		is_camera_rotate = false;

		break;

	case 'C':

		is_camera_rotate = false;

		break;

	case '1':
		if (is_third_person_view) {					// 3��Ī ������ �� 1��Ī���� ��ȯ

			is_third_person_view = false;

		}

		break;

	case '3':
		if (!is_third_person_view) {				// 3��Ī ������ �ƴ� �� 3��Ī���� ��ȯ

			is_third_person_view = true;
		}

		break;

	default:
		break;
	}

	glutPostRedisplay();
}