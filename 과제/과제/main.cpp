#include <iostream>
#include <Windows.h>
#include <vector>
#include "file2buf.h"
#include "InitApp.h"
#include "OpenGL.h"
#include "Polygon.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define MIN_SPEED 100		// 0.110 초마다 timer 가 돌도록 함
#define MAX_SPEED 40		// 0.010 초마다 timer 가 돌도록 함
#define WIDTH 800
#define HEIGHT 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Timer(int a);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Draw_Line();

GLfloat Min(GLfloat min[]);
GLfloat Max(GLfloat max[]);
void Triangle_Slice(Polygons *tmp, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void Square_Slice(Polygons *tmp, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void Set_Spline(Polygons *tmp);


GLuint ShaderProgram;

Polygons *head = nullptr;
Polygons *polygon;
Polygons *temp;
Polygons *del = nullptr;

int generate_timer = 0;
bool is_clicked = false;
bool is_draw_route = true;
bool is_draw_fill = true;

GLfloat mouse_start_x = 0.0f, mouse_start_y = 0.0f;		// 전역변수 정의
GLfloat mouse_end_x = 0.0f, mouse_end_y = 0.0f;

double GENERATE = 30;						// generate timer 가 GENERATE 보다 커지면 도형 생성
double TIMER_SPEED = 70;					// generate * timer_speed  = 생성시간
double TIME = GENERATE * TIMER_SPEED;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Slice");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitProgram(ShaderProgram);

	glutDisplayFunc(drawScene);
	glutTimerFunc(TIMER_SPEED, Timer, 1);			// 타이머
	glutKeyboardFunc(Keyboard);						// 키보드

	glutMouseFunc(Mouse);							// 마우스
	glutMotionFunc(MouseMotion);					// 마우스 움직임

	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Timer(int a)
{	
	// ------------------------ 생성 부분 ------------------------

	generate_timer++;

	if ((double)generate_timer >= GENERATE) {
		Polygons *polygon = new Polygons;
		polygon->Set_Polygons();
		generate_timer = 0;


		if (head == nullptr) {
			head = polygon;
		}

		else if (head != nullptr) {
			temp = head;
			while (temp->next != nullptr) {	// 연결리스트의 끝까지 감
				temp = temp->next;
			}

			temp->next = polygon;
			polygon->pre = temp;
		}

	}



	// ------------------------ 생성 부분 ------------------------


	// ------------------------ 업데이트 부분 ------------------------
	

	if (head != nullptr) {

		for (Polygons* tmp = head; tmp != nullptr; tmp = tmp->next) {		// 연결리스트를 돌면서 그림
			tmp->Update();

			if (is_draw_fill)
				tmp->Draw_Fill();
			else
				tmp->Draw_Line();
		}

	}

	// ------------------------ 업데이트 부분 ------------------------


	
	// ------------------------ 삭제 부분 ------------------------

	if (head != nullptr) {

		for (Polygons* tmp = head; tmp != nullptr; ) {		// 연결리스트를 돌면서 그림

			if (tmp->Get_I() >= RATE) {		// 경로 끝까지 갔다면

				if (tmp == head) {		// 삭제할 곳이 head

					if (tmp->next == nullptr) { 	// 연결리스트가 현재 1개일 경우
						head = nullptr;

					}
					
					else {							// 1개가 아닌경우
						head = tmp->next;
						head->pre = nullptr;

					}

				}

				else if (tmp->next == nullptr) {		// 삭제할 곳이 리스트 끝
					tmp->pre->next = nullptr;

				}

				else {									// 삭제할 곳이 리스트 중간
					tmp->pre->next = tmp->next;
					tmp->next->pre = tmp->pre;

				}
				del = tmp;

			}	// 삭제

			tmp = tmp->next;

			if (del != nullptr) {
				delete del;
				del = nullptr;
			}


		}		// 연결리스트 for문

	}

	// ------------------------ 업데이트 부분 ------------------------


	glutPostRedisplay();
	glutTimerFunc(TIMER_SPEED, Timer, 1);
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ShaderProgram);

	if (head != nullptr) {
		
		for (Polygons *tmp = head; tmp != nullptr; tmp = tmp->next) {

			tmp->Render(ShaderProgram);


		}
		
	}


	if (is_clicked)
		Draw_Line();		// 드래그 직선

	glutSwapBuffers();


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key){
	case 'a':		// 속도 +
	case 'A':
		if(TIMER_SPEED > MAX_SPEED)
			TIMER_SPEED -= 10;

		GENERATE = TIME / TIMER_SPEED;		// 생성하는데 걸리는 시간은 동일하게 하기위해 나누기 연산을 함

		break;

	case 's':			// 속도 -
	case 'S':
		if (TIMER_SPEED < MIN_SPEED)
			TIMER_SPEED += 10;

		GENERATE = TIME / TIMER_SPEED;

		break;

	case 'R':
	case 'r':

		if (is_draw_route)
			is_draw_route = false;
		else
			is_draw_route = true;

		break;

	case 'm':			// 그리기모드
	case 'M':
		if (is_draw_fill)
			is_draw_fill = false;
		else
			is_draw_fill = true;


		break;

	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		is_clicked = true;
		mouse_start_x = (x - WIDTH / 2) / (GLfloat)(WIDTH / 2);
		mouse_start_y = (HEIGHT / 2 - y) / (GLfloat)(HEIGHT / 2);

		mouse_end_x = mouse_start_x;
		mouse_end_y = mouse_end_y;

	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		is_clicked = false;



		// ------------------------------------------------------충돌처리 해야할 부분 ------------------------------------------------------

		if (head != nullptr) {

			for (Polygons *tmp = head; tmp != nullptr; tmp = tmp->next) {

				if (tmp->Get_vertex_number() == 3) {

				}

				else if (tmp->Get_vertex_number() == 4) {
					Square_Slice(tmp, mouse_start_x, mouse_start_y, mouse_end_x, mouse_end_y);
				}


			}


		}


		// ---------------------------------------------------------------------------------------------------------------------------------


	}

}

void MouseMotion(int x, int y)
{
	if (is_clicked) {
		mouse_end_x = (x - WIDTH / 2) / (GLfloat)(WIDTH / 2);
		mouse_end_y = (HEIGHT / 2 - y) / (GLfloat)(HEIGHT / 2);
	}

}

void Draw_Line()
{
	GLfloat vertexData[] = {
		mouse_start_x, mouse_start_y, 0.0f,
		mouse_end_x, mouse_end_y, 0.0f
	};

	glm::mat4 model = glm::mat4(1.0f);

	GLuint modelLocation = glGetUniformLocation(ShaderProgram, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);

	GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(pos_id);
}

void Triangle_Slice(Polygons *tmp, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	GLfloat p1_x = x1;
	GLfloat p1_y = y1;
	GLfloat p2_x = x2;
	GLfloat p2_y = y2;


	

}

void Square_Slice(Polygons *tmp, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	GLfloat dx = x2 - x1;
	GLfloat dy = y2 - y1;


	GLfloat min_x = tmp->Get_Pos_x() -0.1f;							// 좌표 중앙값  - 0.1f
	GLfloat max_x = min_x + 0.2f;									// min_x 와의 차이 -> 0.2f
	GLfloat min_y = tmp->Get_Pos_y() - 0.1f;
	GLfloat max_y = min_y + 0.2f;

	GLfloat p[4] = { -dx, dx, -dy, dy };
	GLfloat q[4] = { x1 - min_x, max_x - x1, y1 - min_y, max_y - y1 };
	GLfloat u[4] = { (q[0] / p[0]), (q[1] / p[1]), (q[2] / p[2]), (q[3] / p[3]) };

	GLfloat max[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat min[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int i = 0; i < 4; ++i) {		// p의 값에 따라 max, min 에 넣음
		if (p[i] < 0) {
			max[i] = u[i];
		}

		else if (p[i] > 0) {
			min[i] = u[i];
		}

	}

	GLfloat u_start = Max(max);
	GLfloat u_end = Min(min);

	GLfloat new_x1, new_x2, new_y1, new_y2;

	if (u_start > u_end) {		// 충돌 X
		
	}

	else if (u_start < u_end && !tmp->is_sliced) {	// 충돌 O -> 새 좌표값 찾아야함
		new_x1 = x1 + u_start * dx;
		new_x2 = x1 + u_end * dx;
		new_y1 = y1 + u_start * dy;
		new_y2 = y1 + u_end * dy;

		std::cout << "x1: " << new_x1 << " y1: " << new_y1 << " x2: " << new_x2 << " y2: " << new_y2 << std::endl;


		// ---------------------------------------- 잘린 도형의 부분 설정 -------------------------------
		
		tmp->is_sliced = true;
		GLfloat range = 0.01f;


		Polygons *polygon = new Polygons;		// 새로운 도형 생성

		// ----------------------------------------- 도형 자르기 ----------------------------------------

		if ((new_y1 >= min_y - range && new_y1 <= min_y + range )  && (new_y2 >= max_y - range && new_y2 <= max_y + range)) 
		{		// 세로로 자름 사각형 2개로 나뉨 , 아래에서 위로 자름

			tmp->squ_vertexData[12] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[13] = new_y2 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y1 - tmp->Get_Pos_y();


			polygon->Sliced_Set_Polygons(new_x1, new_y1, new_x2, new_y2);
		}

		else if ((new_y1 >= max_y - range && new_y1 <= max_y + range) && (new_y2 >= min_y - range && new_y2 <= min_y + range)) 
		{	// 세로로 자름 사각형 2개로 나뉨 , 위에서 아래로 자름

			tmp->squ_vertexData[12] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[13] = new_y1 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y2 - tmp->Get_Pos_y();

			polygon->Sliced_Set_Polygons(new_x1, new_y1, new_x2, new_y2);
		}

		else if ((new_x1 >= max_x - range && new_x1 <= max_x + range) && (new_x2 >= min_x - range && new_x2 <= min_x + range))
		{	// 가로로 자름 사각형 2개로 나뉨 , 오른쪽에서 왼쪽으로 자름

			tmp->squ_vertexData[0] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[1] = new_y2 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y1 - tmp->Get_Pos_y();
		}

		else if ((new_x1 >= min_x - range && new_x1 <= min_x + range) && (new_x2 >= max_x - range && new_x2 <= max_x + range))
		{	// 가로로 자름 사각형 2개로 나뉨 , 왼쪽에서 오른쪽으로 자름

			tmp->squ_vertexData[0] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[1] = new_y1 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y2 - tmp->Get_Pos_y();
		}

		else {		// 삼각형, 오각형으로 나뉨
			tmp->Set_Vertex_Count(5);

			if (((new_x1 >= min_x - range && new_x1 <= min_x + range) || (new_x2 >= min_x - range && new_x2 <= min_x + range)) &&
				((new_y1 >= max_y - range && new_y1 <= max_y + range) || (new_y2 >= max_y - range && new_y2 <= max_y + range)))
			{		// 왼쪽, 위쪽이 잘리면

				if (new_x1 < new_x2) {		// 왼->오

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = new_x1 - tmp->Get_Pos_x();		//좌상단
					tmp->pen_vertexData[7] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[12] = new_x2 - tmp->Get_Pos_x();		//좌상단
					tmp->pen_vertexData[13] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = 0.1f;		//우상단
					tmp->pen_vertexData[19] = 0.1f;

					tmp->pen_vertexData[24] = 0.1f;		// 우하단
					tmp->pen_vertexData[25] = -0.1f;

				}

				else if (new_x1 >= new_x2) {		// 오->왼

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = new_x2 - tmp->Get_Pos_x();		//좌상단
					tmp->pen_vertexData[7] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[12] = new_x1 - tmp->Get_Pos_x();		//좌상단
					tmp->pen_vertexData[13] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = 0.1f;		//우상단
					tmp->pen_vertexData[19] = 0.1f;

					tmp->pen_vertexData[24] = 0.1f;		// 우하단
					tmp->pen_vertexData[25] = -0.1f;
				}

			}


			else if (((new_x1 >= max_x - range && new_x1 <= max_x + range) || (new_x2 >= max_x - range && new_x2 <= max_x + range)) &&
				((new_y1 >= max_y - range && new_y1 <= max_y + range) || (new_y2 >= max_y - range && new_y2 <= max_y + range)))
			{		// 오른쪽 위쪽 잘리면

				if (new_x1 < new_x2) {		// 왼->오

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = new_x1 - tmp->Get_Pos_x();		//우상단
					tmp->pen_vertexData[13] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = new_x2 - tmp->Get_Pos_x();		//우상단
					tmp->pen_vertexData[19] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = 0.1f;		// 우하단
					tmp->pen_vertexData[25] = -0.1f;

				}

				else if (new_x1 >= new_x2) {		// 오->왼

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = new_x2 - tmp->Get_Pos_x();		//우상단
					tmp->pen_vertexData[13] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = new_x1 - tmp->Get_Pos_x();		//우상단
					tmp->pen_vertexData[19] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = 0.1f;		// 우하단
					tmp->pen_vertexData[25] = -0.1f;
				}
			}


			else if (((new_x1 >= min_x - range && new_x1 <= min_x + range) || (new_x2 >= min_x - range && new_x2 <= min_x + range)) &&
				((new_y1 >= min_y - range && new_y1 <= min_y + range) || (new_y2 >= min_y - range && new_y2 <= min_y + range)))
			{		// 왼쪽 아래쪽 잘리면

				if (new_x1 < new_x2) {		// 왼->오

					tmp->pen_vertexData[0] = new_x1 - tmp->Get_Pos_x();		// 좌하단
					tmp->pen_vertexData[1] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//우상단
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = 0.1f;		// 우하단
					tmp->pen_vertexData[19] = -0.1f;

					tmp->pen_vertexData[24] = new_x2 - tmp->Get_Pos_x();			// 좌하단
					tmp->pen_vertexData[25] = new_y2 - tmp->Get_Pos_y();

				}

				else if (new_x1 >= new_x2) {		// 오->왼

					tmp->pen_vertexData[0] = new_x2 - tmp->Get_Pos_x();		// 좌하단
					tmp->pen_vertexData[1] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//우상단
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = 0.1f;		// 우상단
					tmp->pen_vertexData[19] = -0.1f;

					tmp->pen_vertexData[24] = new_x1 - tmp->Get_Pos_x();			// 좌하단
					tmp->pen_vertexData[25] = new_y1 - tmp->Get_Pos_y();
				}
			}


			else if (((new_x1 >= max_x - range && new_x1 <= max_x + range) || (new_x2 >= max_x - range && new_x2 <= max_x + range)) &&
				((new_y1 >= min_y - range && new_y1 <= min_y + range) || (new_y2 >= min_y - range && new_y2 <= min_y + range)))
			{		// 오른쪽 아래쪽 잘리면

				if (new_x1 < new_x2) {		// 왼->오

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//우상단
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = new_x2 - tmp->Get_Pos_x();		// 우하단
					tmp->pen_vertexData[19] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = new_x1 - tmp->Get_Pos_x();			// 우하단
					tmp->pen_vertexData[25] = new_y1 - tmp->Get_Pos_y();

					std::cout << "오른쪽, 아래쪽 슬라이스 왼->오" << std::endl;
				}

				else if (new_x1 >= new_x2) {		// 오->왼

					tmp->pen_vertexData[0] = -0.1f;		// 좌하단
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//좌상단
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//우상단
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = new_x1 - tmp->Get_Pos_x();		// 우하단
					tmp->pen_vertexData[19] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = new_x2 - tmp->Get_Pos_x();			// 우하단
					tmp->pen_vertexData[25] = new_y2 - tmp->Get_Pos_y();

					std::cout << "오른쪽, 아래쪽 슬라이스 오->왼" << std::endl;
				}
			}

		}


		// ----------------------------------------- 도형 자르기 ----------------------------------------

		Set_Spline(tmp);



		// ---------------------------------------- 잘린 도형의 부분 설정 -------------------------------



		// ---------------------------------------- 잘린 새로운 도형 리스트 연결 -------------------------------


		if (head == nullptr) {
			head = polygon;
		}

		else if (head != nullptr) {
			temp = head;
			while (temp->next != nullptr) {	// 연결리스트의 끝까지 감
				temp = temp->next;
			}

			temp->next = polygon;
			polygon->pre = temp;
		}

		// ---------------------------------------- 잘린 새로운 도형 리스트 연결 -------------------------------


	}

}

GLfloat Min(GLfloat min[])
{
	GLfloat m = 100.0f;

	for (int i = 0; i < 4; ++i) {
		
		if (min[i] <= m) {
			m = min[i];
		}

	}

	return m;
}

GLfloat Max(GLfloat max[])
{
	GLfloat m = -1.0f;

	for (int i = 0; i < 4; ++i) {

		if (max[i] >= m) {
			m = max[i];
		}

	}

	return m;

}

void Set_Spline(Polygons *tmp)
{
	GLfloat x1, y1, x2, y2, x3, y3;

	x1 = tmp->Get_Pos_x();
	y1 = tmp->Get_Pos_y();


	x2 = -0.6f;
	y2 = 0.0f;

	x3 = -0.4f;
	y3 = -1.4f;

	tmp->Set_Curve(x1, y1, x2, y2, x3, y3);



}