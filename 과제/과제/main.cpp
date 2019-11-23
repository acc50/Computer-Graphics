#include <iostream>
#include <Windows.h>
#include <vector>
#include "file2buf.h"
#include "InitApp.h"
#include "OpenGL.h"
#include "Polygon.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define MIN_SPEED 100		// 0.110 �ʸ��� timer �� ������ ��
#define MAX_SPEED 40		// 0.010 �ʸ��� timer �� ������ ��
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

GLfloat mouse_start_x = 0.0f, mouse_start_y = 0.0f;		// �������� ����
GLfloat mouse_end_x = 0.0f, mouse_end_y = 0.0f;

double GENERATE = 30;						// generate timer �� GENERATE ���� Ŀ���� ���� ����
double TIMER_SPEED = 70;					// generate * timer_speed  = �����ð�
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
	glutTimerFunc(TIMER_SPEED, Timer, 1);			// Ÿ�̸�
	glutKeyboardFunc(Keyboard);						// Ű����

	glutMouseFunc(Mouse);							// ���콺
	glutMotionFunc(MouseMotion);					// ���콺 ������

	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Timer(int a)
{	
	// ------------------------ ���� �κ� ------------------------

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
			while (temp->next != nullptr) {	// ���Ḯ��Ʈ�� ������ ��
				temp = temp->next;
			}

			temp->next = polygon;
			polygon->pre = temp;
		}

	}



	// ------------------------ ���� �κ� ------------------------


	// ------------------------ ������Ʈ �κ� ------------------------
	

	if (head != nullptr) {

		for (Polygons* tmp = head; tmp != nullptr; tmp = tmp->next) {		// ���Ḯ��Ʈ�� ���鼭 �׸�
			tmp->Update();

			if (is_draw_fill)
				tmp->Draw_Fill();
			else
				tmp->Draw_Line();
		}

	}

	// ------------------------ ������Ʈ �κ� ------------------------


	
	// ------------------------ ���� �κ� ------------------------

	if (head != nullptr) {

		for (Polygons* tmp = head; tmp != nullptr; ) {		// ���Ḯ��Ʈ�� ���鼭 �׸�

			if (tmp->Get_I() >= RATE) {		// ��� ������ ���ٸ�

				if (tmp == head) {		// ������ ���� head

					if (tmp->next == nullptr) { 	// ���Ḯ��Ʈ�� ���� 1���� ���
						head = nullptr;

					}
					
					else {							// 1���� �ƴѰ��
						head = tmp->next;
						head->pre = nullptr;

					}

				}

				else if (tmp->next == nullptr) {		// ������ ���� ����Ʈ ��
					tmp->pre->next = nullptr;

				}

				else {									// ������ ���� ����Ʈ �߰�
					tmp->pre->next = tmp->next;
					tmp->next->pre = tmp->pre;

				}
				del = tmp;

			}	// ����

			tmp = tmp->next;

			if (del != nullptr) {
				delete del;
				del = nullptr;
			}


		}		// ���Ḯ��Ʈ for��

	}

	// ------------------------ ������Ʈ �κ� ------------------------


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
		Draw_Line();		// �巡�� ����

	glutSwapBuffers();


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key){
	case 'a':		// �ӵ� +
	case 'A':
		if(TIMER_SPEED > MAX_SPEED)
			TIMER_SPEED -= 10;

		GENERATE = TIME / TIMER_SPEED;		// �����ϴµ� �ɸ��� �ð��� �����ϰ� �ϱ����� ������ ������ ��

		break;

	case 's':			// �ӵ� -
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

	case 'm':			// �׸�����
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



		// ------------------------------------------------------�浹ó�� �ؾ��� �κ� ------------------------------------------------------

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


	GLfloat min_x = tmp->Get_Pos_x() -0.1f;							// ��ǥ �߾Ӱ�  - 0.1f
	GLfloat max_x = min_x + 0.2f;									// min_x ���� ���� -> 0.2f
	GLfloat min_y = tmp->Get_Pos_y() - 0.1f;
	GLfloat max_y = min_y + 0.2f;

	GLfloat p[4] = { -dx, dx, -dy, dy };
	GLfloat q[4] = { x1 - min_x, max_x - x1, y1 - min_y, max_y - y1 };
	GLfloat u[4] = { (q[0] / p[0]), (q[1] / p[1]), (q[2] / p[2]), (q[3] / p[3]) };

	GLfloat max[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat min[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int i = 0; i < 4; ++i) {		// p�� ���� ���� max, min �� ����
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

	if (u_start > u_end) {		// �浹 X
		
	}

	else if (u_start < u_end && !tmp->is_sliced) {	// �浹 O -> �� ��ǥ�� ã�ƾ���
		new_x1 = x1 + u_start * dx;
		new_x2 = x1 + u_end * dx;
		new_y1 = y1 + u_start * dy;
		new_y2 = y1 + u_end * dy;

		std::cout << "x1: " << new_x1 << " y1: " << new_y1 << " x2: " << new_x2 << " y2: " << new_y2 << std::endl;


		// ---------------------------------------- �߸� ������ �κ� ���� -------------------------------
		
		tmp->is_sliced = true;
		GLfloat range = 0.01f;


		Polygons *polygon = new Polygons;		// ���ο� ���� ����

		// ----------------------------------------- ���� �ڸ��� ----------------------------------------

		if ((new_y1 >= min_y - range && new_y1 <= min_y + range )  && (new_y2 >= max_y - range && new_y2 <= max_y + range)) 
		{		// ���η� �ڸ� �簢�� 2���� ���� , �Ʒ����� ���� �ڸ�

			tmp->squ_vertexData[12] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[13] = new_y2 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y1 - tmp->Get_Pos_y();


			polygon->Sliced_Set_Polygons(new_x1, new_y1, new_x2, new_y2);
		}

		else if ((new_y1 >= max_y - range && new_y1 <= max_y + range) && (new_y2 >= min_y - range && new_y2 <= min_y + range)) 
		{	// ���η� �ڸ� �簢�� 2���� ���� , ������ �Ʒ��� �ڸ�

			tmp->squ_vertexData[12] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[13] = new_y1 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y2 - tmp->Get_Pos_y();

			polygon->Sliced_Set_Polygons(new_x1, new_y1, new_x2, new_y2);
		}

		else if ((new_x1 >= max_x - range && new_x1 <= max_x + range) && (new_x2 >= min_x - range && new_x2 <= min_x + range))
		{	// ���η� �ڸ� �簢�� 2���� ���� , �����ʿ��� �������� �ڸ�

			tmp->squ_vertexData[0] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[1] = new_y2 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y1 - tmp->Get_Pos_y();
		}

		else if ((new_x1 >= min_x - range && new_x1 <= min_x + range) && (new_x2 >= max_x - range && new_x2 <= max_x + range))
		{	// ���η� �ڸ� �簢�� 2���� ���� , ���ʿ��� ���������� �ڸ�

			tmp->squ_vertexData[0] = new_x1 - tmp->Get_Pos_x();
			tmp->squ_vertexData[1] = new_y1 - tmp->Get_Pos_y();

			tmp->squ_vertexData[18] = new_x2 - tmp->Get_Pos_x();
			tmp->squ_vertexData[19] = new_y2 - tmp->Get_Pos_y();
		}

		else {		// �ﰢ��, ���������� ����
			tmp->Set_Vertex_Count(5);

			if (((new_x1 >= min_x - range && new_x1 <= min_x + range) || (new_x2 >= min_x - range && new_x2 <= min_x + range)) &&
				((new_y1 >= max_y - range && new_y1 <= max_y + range) || (new_y2 >= max_y - range && new_y2 <= max_y + range)))
			{		// ����, ������ �߸���

				if (new_x1 < new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = new_x1 - tmp->Get_Pos_x();		//�»��
					tmp->pen_vertexData[7] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[12] = new_x2 - tmp->Get_Pos_x();		//�»��
					tmp->pen_vertexData[13] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = 0.1f;		//����
					tmp->pen_vertexData[19] = 0.1f;

					tmp->pen_vertexData[24] = 0.1f;		// ���ϴ�
					tmp->pen_vertexData[25] = -0.1f;

				}

				else if (new_x1 >= new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = new_x2 - tmp->Get_Pos_x();		//�»��
					tmp->pen_vertexData[7] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[12] = new_x1 - tmp->Get_Pos_x();		//�»��
					tmp->pen_vertexData[13] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = 0.1f;		//����
					tmp->pen_vertexData[19] = 0.1f;

					tmp->pen_vertexData[24] = 0.1f;		// ���ϴ�
					tmp->pen_vertexData[25] = -0.1f;
				}

			}


			else if (((new_x1 >= max_x - range && new_x1 <= max_x + range) || (new_x2 >= max_x - range && new_x2 <= max_x + range)) &&
				((new_y1 >= max_y - range && new_y1 <= max_y + range) || (new_y2 >= max_y - range && new_y2 <= max_y + range)))
			{		// ������ ���� �߸���

				if (new_x1 < new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = new_x1 - tmp->Get_Pos_x();		//����
					tmp->pen_vertexData[13] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = new_x2 - tmp->Get_Pos_x();		//����
					tmp->pen_vertexData[19] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = 0.1f;		// ���ϴ�
					tmp->pen_vertexData[25] = -0.1f;

				}

				else if (new_x1 >= new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = new_x2 - tmp->Get_Pos_x();		//����
					tmp->pen_vertexData[13] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[18] = new_x1 - tmp->Get_Pos_x();		//����
					tmp->pen_vertexData[19] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = 0.1f;		// ���ϴ�
					tmp->pen_vertexData[25] = -0.1f;
				}
			}


			else if (((new_x1 >= min_x - range && new_x1 <= min_x + range) || (new_x2 >= min_x - range && new_x2 <= min_x + range)) &&
				((new_y1 >= min_y - range && new_y1 <= min_y + range) || (new_y2 >= min_y - range && new_y2 <= min_y + range)))
			{		// ���� �Ʒ��� �߸���

				if (new_x1 < new_x2) {		// ��->��

					tmp->pen_vertexData[0] = new_x1 - tmp->Get_Pos_x();		// ���ϴ�
					tmp->pen_vertexData[1] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//����
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = 0.1f;		// ���ϴ�
					tmp->pen_vertexData[19] = -0.1f;

					tmp->pen_vertexData[24] = new_x2 - tmp->Get_Pos_x();			// ���ϴ�
					tmp->pen_vertexData[25] = new_y2 - tmp->Get_Pos_y();

				}

				else if (new_x1 >= new_x2) {		// ��->��

					tmp->pen_vertexData[0] = new_x2 - tmp->Get_Pos_x();		// ���ϴ�
					tmp->pen_vertexData[1] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//����
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = 0.1f;		// ����
					tmp->pen_vertexData[19] = -0.1f;

					tmp->pen_vertexData[24] = new_x1 - tmp->Get_Pos_x();			// ���ϴ�
					tmp->pen_vertexData[25] = new_y1 - tmp->Get_Pos_y();
				}
			}


			else if (((new_x1 >= max_x - range && new_x1 <= max_x + range) || (new_x2 >= max_x - range && new_x2 <= max_x + range)) &&
				((new_y1 >= min_y - range && new_y1 <= min_y + range) || (new_y2 >= min_y - range && new_y2 <= min_y + range)))
			{		// ������ �Ʒ��� �߸���

				if (new_x1 < new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//����
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = new_x2 - tmp->Get_Pos_x();		// ���ϴ�
					tmp->pen_vertexData[19] = new_y2 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = new_x1 - tmp->Get_Pos_x();			// ���ϴ�
					tmp->pen_vertexData[25] = new_y1 - tmp->Get_Pos_y();

					std::cout << "������, �Ʒ��� �����̽� ��->��" << std::endl;
				}

				else if (new_x1 >= new_x2) {		// ��->��

					tmp->pen_vertexData[0] = -0.1f;		// ���ϴ�
					tmp->pen_vertexData[1] = -0.1f;

					tmp->pen_vertexData[6] = -0.1f;		//�»��
					tmp->pen_vertexData[7] = 0.1f;

					tmp->pen_vertexData[12] = 0.1f;		//����
					tmp->pen_vertexData[13] = 0.1f;

					tmp->pen_vertexData[18] = new_x1 - tmp->Get_Pos_x();		// ���ϴ�
					tmp->pen_vertexData[19] = new_y1 - tmp->Get_Pos_y();

					tmp->pen_vertexData[24] = new_x2 - tmp->Get_Pos_x();			// ���ϴ�
					tmp->pen_vertexData[25] = new_y2 - tmp->Get_Pos_y();

					std::cout << "������, �Ʒ��� �����̽� ��->��" << std::endl;
				}
			}

		}


		// ----------------------------------------- ���� �ڸ��� ----------------------------------------

		Set_Spline(tmp);



		// ---------------------------------------- �߸� ������ �κ� ���� -------------------------------



		// ---------------------------------------- �߸� ���ο� ���� ����Ʈ ���� -------------------------------


		if (head == nullptr) {
			head = polygon;
		}

		else if (head != nullptr) {
			temp = head;
			while (temp->next != nullptr) {	// ���Ḯ��Ʈ�� ������ ��
				temp = temp->next;
			}

			temp->next = polygon;
			polygon->pre = temp;
		}

		// ---------------------------------------- �߸� ���ο� ���� ����Ʈ ���� -------------------------------


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