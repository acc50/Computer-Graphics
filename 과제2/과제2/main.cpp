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
bool is_camera_move = false;		// 카메라의 이동
bool is_camera_rotate = false;		// 카메라의 회전
bool is_at_rotate = false;			// at의 회전
bool is_third_person_view = false;	// 3인칭 시점
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
	// -------------------------  로봇 이동 부분 ---------------------------

	for (int i = 0; i < 4; ++i) {
		robots[i].Move(EYE.x, EYE.y, EYE.z, AT.x, AT.y, AT.z);
	}

	// -------------------------  로봇 이동 부분 ---------------------------


	// ------------------------- 로봇과 장애물 체크 ------------------------

	glm::vec3 r_pos[4];

	GLfloat x, z;

	for (int i = 0; i < 4; ++i) {
		r_pos[i] = robots[i].get_Pos();

		x = r_pos[i].x;
		z = r_pos[i].z;

		// -------------------------- 충돌이 일어나지 않음 ----------------------------
		if (x > LENGTH + 0.3f) {
			continue;
		}

		else if (x < -LENGTH - 0.3f) {					// 블록의 길이 + 몸체의 길이 0.3f
			continue;
		}

		else if (z > LENGTH + 0.3f) {
			continue;
		}

		else if (z < -LENGTH - 0.3f) {
			continue;
		}
		// -------------------------- 충돌이 일어나지 않음 ----------------------------

		// 충돌이 일어나지 않을 조건이면 아래를 실행하지 않고 다음 for문 조건으로 넘어감




		// ------------------------- 위의 for문 전부 통과시 충돌 --------------------------
		
		if (x > LENGTH + 0.15f) {					// 블록의 오른쪽에서 충돌
			robots[i].set_Pos_x((GLfloat)LENGTH + 0.3f);
		}

		else if(x < -LENGTH - 0.15f) {					// 블록의 왼쪽에서 충돌
			robots[i].set_Pos_x(-(GLfloat)LENGTH - 0.3f);
		}

		if (z > LENGTH + 0.15f) {		// 블록의 앞쪽에서 충돌
			robots[i].set_Pos_z((GLfloat)LENGTH + 0.3f);
		}

		else if (z < -LENGTH - 0.15f) {					// 블록의 뒷쪽에서 충돌
			robots[i].set_Pos_z(-(GLfloat)LENGTH - 0.3f);
		}

		robots[i].Set_MovePoint();

		// --------------------		충돌 처리 -------------------------------------
		

	}



	// ------------------------- 로봇과 장애물 체크 ------------------------


	glutPostRedisplay();
	glutTimerFunc(TIMER_SPEED, Timer, a);
}

void Camera_Move_Timer(int a)
{

	// -------------------------  카메라 이동 부분 ---------------------------
	
	
	// 현재 AT 과 EYE 를 기준으로 두고 움직일 방향과 거리 계산

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

	T_EYE = glm::vec3(EYE.x - front_dir.x * 2, EYE.y + 1.0f, EYE.z - front_dir.z * 2);	// 3인칭 시점 기준 EYE 위치 갱신

	// -------------------------  카메라 이동 부분 ---------------------------


	glutPostRedisplay();

	if (is_camera_move)
		glutTimerFunc(CAMERA_TIMER_SPEED, Camera_Move_Timer, a);
}

void Camera_Rotate_Timer(int a)
{

	// -------------------------  카메라 회전 부분 ---------------------------

	if (is_camera_rotate) {

		glm::mat4 r = glm::mat4(1.0f);

		r = glm::rotate(r, glm::radians(angle_sign * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat3 rm = glm::mat3(r);

		EYE = EYE * rm;

		T_EYE = T_EYE * rm;
	}

	if (is_at_rotate) {

		// AT 을 EYE 점을 기준으로 회전

		glm::mat4 r = glm::mat4(1.0f);

		r = glm::rotate(r, glm::radians(angle_sign * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat3 rm = glm::mat3(r);
		
		AT = AT - EYE;			// EYE 를 원점이동
		AT = AT * rm;			// AT 을 회전
		AT = AT + EYE;			// 이동했던 만큼 다시 역이동

		T_EYE = T_EYE - EYE;
		T_EYE = T_EYE * rm;
		T_EYE = T_EYE + EYE;

	}




	// -------------------------  카메라 회전 부분 ---------------------------

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

	glm::mat4 view = glm::mat4(1.0f);		// 뷰변환

	if (is_third_person_view)
		view = glm::lookAt(T_EYE, AT, UP);

	else
		view = glm::lookAt(EYE, AT, UP);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// 뷰변환


	glm::mat4 projection = glm::mat4(1.0f);		// 투영변환
	projection = glm::perspective(glm::radians(120.0f), (float)WIDTH / (float)HEIGHT, 0.0f, 2000.0f);	// 뷰잉각도, 종횡비, 가까운거리, 먼거리
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// 투영변환

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
	// 행렬 변환값 받아옴
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 rm = glm::mat4(1.0f);
	rm = glm::rotate(rm, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//model = model * rm;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환



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

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 tx = glm::mat4(1.0f);
	glm::mat4 rm = glm::mat4(1.0f);


	glm::vec3 front_dir = AT - EYE;
	front_dir = front_dir / (sqrt(front_dir.x * front_dir.x + front_dir.y * front_dir.y + front_dir.z * front_dir.z));

	tx = glm::translate(tx, glm::vec3(EYE.x - front_dir.x / 4, EYE.y, EYE.z - front_dir.z / 4));

	if(is_camera_rotate)
		rm = glm::rotate(rm, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	model = model * tx * rm;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환



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

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

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


	case 'y':			// 카메라 기준 회전

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

	case 'c':			// 중점 기준 회전

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

	case 'y':			// 카메라 기준 회전

		is_at_rotate = false;

		break;

	case 'Y':

		is_at_rotate = false;
		break;

	case 'c':			// 중점 기준 회전

		is_camera_rotate = false;

		break;

	case 'C':

		is_camera_rotate = false;

		break;

	case '1':
		if (is_third_person_view) {					// 3인칭 시점일 때 1인칭으로 전환

			is_third_person_view = false;

		}

		break;

	case '3':
		if (!is_third_person_view) {				// 3인칭 시점이 아닐 때 3인칭으로 전환

			is_third_person_view = true;
		}

		break;

	default:
		break;
	}

	glutPostRedisplay();
}