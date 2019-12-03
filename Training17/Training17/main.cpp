#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Windows.h>
#include "file2buf.h"
#include "InitApp.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO, EBO;
GLuint midVBO, midEBO;
GLuint upVBO, upEBO;
GLuint boardEBO, boardVBO;
GLuint lVBO, lEBO;

GLfloat down_move = 0.0f, mid_move = 0.0f, up_move = 0.0f;


float angle = 0;

bool y_rotate = false;
GLfloat light_y_angle = 0.0f;
bool light_switch = true;

bool is_move = true;
int move_dir = 1;

bool is_camera_rotate = false;
bool is_standard_camera = false;

GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(0.0f, 1.0f, 2.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec4 LightPos = glm::vec4(-4.0, 1.0, 0.0, 1.0);		// 광원 위치
glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

GLvoid draw_cube_down(float);
GLvoid draw_cube_mid(float);
GLvoid draw_cube_up(float);
GLvoid draw_board();
GLvoid draw_polygon();

void Timer(int a);
void y_Rotate_Timer(int a);
void z_Rotate_Timer(int a);
void Camera_Timer(int a);
void InputKey(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);

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
	CreateCube(EBO, VBO);

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
	if (is_move) {
		if (move_dir == 1) {
			down_move += 0.01f;
			mid_move += 0.025f;
			up_move += 0.05f;

			if (down_move > 1.0f)
				move_dir = -1;
		}

		else if (move_dir == -1) {
			down_move -= 0.01f;
			mid_move -= 0.025f;
			up_move -= 0.05f;

			if (down_move < -1.0f)
				move_dir = 1;
		}
	}

	glutPostRedisplay();

	if(is_move)
		glutTimerFunc(10, Timer, a);
}

void y_Rotate_Timer(int a)
{
	if (y_rotate)
		light_y_angle = 1.0f;

	glutPostRedisplay();

	if(y_rotate)
		glutTimerFunc(20, y_Rotate_Timer, a);
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

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(ShaderProgram);


	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");

	glm::mat4 view = glm::mat4(1.0f);		// 뷰변환
	view = glm::lookAt(glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z), glm::vec3(AT.x, AT.y, AT.z), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(camera_y), glm::vec3(0.0f, 1.0f, 0.0f));
	
	
	glm::mat4 itx = glm::mat4(1.0f);
	itx = glm::translate(itx, glm::vec3(-CameraPos.x, -CameraPos.y, -CameraPos.z));		// 역변환

	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z));		// 제자리 변환
	
	view = view * rotate;


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// 뷰변환


	glm::mat4 projection = glm::mat4(1.0f);		// 투영변환
	projection = glm::perspective(glm::radians(120.0f), (float)WIDTH / (float)HEIGHT, 0.0f, 2000.0f);	// 뷰잉각도, 종횡비, 가까운거리, 먼거리
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// 투영변환

	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(light_y_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	LightPos = LightPos * r;

	int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos");
	glUniform3f(viewPosLocation, CameraPos.x, CameraPos.y, CameraPos.z);
	int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos");
	glUniform3f(lightPosLocation, LightPos.x, LightPos.y, LightPos.z);
	int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor");
	glUniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b);									// 백색광

	draw_board();
	draw_cube_down(angle);
	draw_cube_mid(angle);
	draw_cube_up(angle);
	draw_polygon();

	glutSwapBuffers();


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

GLvoid draw_cube_down(float x)
{
	GLfloat vertex[] = {
		-0.5f, 1.0f, -0.5f,		-1.0f, 1.0f, -1.0f,  //0번점
		-0.5f, 1.0f, 0.5f,		-1.0f ,1.0f, 1.0f,  //1번점
		0.5f, 1.0f, 0.5f,		1.0f, 1.0f,  1.0f,  //2번점
		0.5f, 1.0f, -0.5f,		1.0f, 1.0f, -1.0f,  //3번점

		-0.5f, 0.0f, -0.5f,		-1.0f, -1.0f, -1.0f,  //4번점
		-0.5f, 0.0f, 0.5f,		-1.0f, -1.0f, 1.0f,  //5번점
		0.5f, 0.0f, 0.5f,		1.0f, -1.0f,  1.0f,  //6번점
		0.5f, 0.0f, -0.5f,		1.0f, -1.0f, -1.0f,  //7번점
	};

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLint gIndices[]
	{
		0,1,2,
		0,2,3,  //윗면

		1,5,6,
		1,6,2, //앞면

		2,6,7,
		2,7,3, //우측면

		0,4,5,
		0,5,1, //좌측면

		5,4,6,
		4,7,6,// 밑면

		0,7,4, //뒷면
		0,3,7
	};

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);


	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 scale = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(0.5f, 0.3f, 0.5f));


	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(0.0f, 0.5f, 1.0f));

	glm::mat4 move = glm::mat4(1.0f);
	move = glm::translate(move, glm::vec3(down_move, 0.0f, 0.0f));

	model = model * scale * tx * move;


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	//GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");		//	location=0 이므로 attribpointer 에서 첫인자에 0넣으면 됨
	//glEnableVertexAttribArray(pos_id);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 1.0, 0.0, 0.2);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_cube_mid(float x)
{

	GLfloat vertex[] = {
		-0.5f, 1.0f, -0.5f,		-1.0f, 1.0f, -1.0f,  //0번점
		-0.5f, 1.0f, 0.5f,		-1.0f ,1.0f, 1.0f,  //1번점
		0.5f, 1.0f, 0.5f,		1.0f, 1.0f,  1.0f,  //2번점
		0.5f, 1.0f, -0.5f,		1.0f, 1.0f, -1.0f,  //3번점

		-0.5f, 0.0f, -0.5f,		-1.0f, -1.0f, -1.0f,  //4번점
		-0.5f, 0.0f, 0.5f,		-1.0f, -1.0f, 1.0f,  //5번점
		0.5f, 0.0f, 0.5f,		1.0f, -1.0f,  1.0f,  //6번점
		0.5f, 0.0f, -0.5f,		1.0f, -1.0f, -1.0f,  //7번점
	};

	glGenBuffers(1, &midVBO);

	glBindBuffer(GL_ARRAY_BUFFER, midVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLint gIndices[]
	{
		0,1,2,
		0,2,3,  //윗면

		1,5,6,
		1,6,2, //앞면

		2,6,7,
		2,7,3, //우측면

		0,4,5,
		0,5,1, //좌측면

		5,4,6,
		4,7,6,// 밑면

		0,7,4, //뒷면
		0,3,7
	};

	glGenBuffers(1, &midEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, midEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);


	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 scale = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(0.2f, 0.2f, 0.3f));


	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(0.0f, 2.3f, 2.0f));

	glm::mat4 move = glm::mat4(1.0f);
	move = glm::translate(move, glm::vec3(mid_move, 0.0f, 0.0f));

	glm::mat4 rm = glm::mat4(1.0f);
	rm = glm::rotate(rm, glm::radians(y_angle), glm::vec3(0.0f, 1.0f, 0.0f));

	model = model * scale * tx * move * rm;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	//GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");		//	location=0 이므로 attribpointer 에서 첫인자에 0넣으면 됨
	//glEnableVertexAttribArray(pos_id);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.2, 0.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid draw_cube_up(float x)
{

	GLfloat vertex[] = {
		-0.5f, 1.0f, -0.5f,		-1.0f, 1.0f, -1.0f,  //0번점
		-0.5f, 1.0f, 0.5f,		-1.0f ,1.0f, 1.0f,  //1번점
		0.5f, 1.0f, 0.5f,		1.0f, 1.0f,  1.0f,  //2번점
		0.5f, 1.0f, -0.5f,		1.0f, 1.0f, -1.0f,  //3번점

		-0.5f, 0.0f, -0.5f,		-1.0f, -1.0f, -1.0f,  //4번점
		-0.5f, 0.0f, 0.5f,		-1.0f, -1.0f, 1.0f,  //5번점
		0.5f, 0.0f, 0.5f,		1.0f, -1.0f,  1.0f,  //6번점
		0.5f, 0.0f, -0.5f,		1.0f, -1.0f, -1.0f,  //7번점
	};
	glGenBuffers(1, &upVBO);

	glBindBuffer(GL_ARRAY_BUFFER, upVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLint gIndices[]
	{
		0,1,2,
		0,2,3,  //윗면

		1,5,6,
		1,6,2, //앞면

		2,6,7,
		2,7,3, //우측면

		0,4,5,
		0,5,1, //좌측면

		5,4,6,
		4,7,6,// 밑면

		0,7,4, //뒷면
		0,3,7
	};

	glGenBuffers(1, &upEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, upEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);


	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 scale = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(0.1f, 0.4f, 0.1f));


	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(0.0f, 1.5f, 6.0f));


	glm::mat4 move = glm::mat4(1.0f);
	move = glm::translate(move, glm::vec3(up_move, 0.0f, 0.0f));


	glm::mat4 rm = glm::mat4(1.0f);
	rm = glm::rotate(rm, glm::radians(z_angle), glm::vec3(0.0f, 0.0f, 1.0f));


	glm::mat4 rm_itx = glm::mat4(1.0f);
	rm_itx = glm::translate(rm_itx, glm::vec3(-up_move, -1.7f, -2.0f));

	glm::mat4 rm_tx = glm::mat4(1.0f);
	rm_tx = glm::translate(rm_itx, glm::vec3(up_move, 1.7f, 2.0f));


	model = model * scale * tx * move * rm;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	//GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");		//	location=0 이므로 attribpointer 에서 첫인자에 0넣으면 됨
	//glEnableVertexAttribArray(pos_id);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.2, 1.0, 0.2);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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

	//GLuint pos_id = glGetAttribLocation(ShaderProgram, "vPos");		//	location=0 이므로 attribpointer 에서 첫인자에 0넣으면 됨
	//glEnableVertexAttribArray(pos_id);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//GLuint frag_id = glGetAttribLocation(ShaderProgram, "vColor");
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
		-0.2f,  -0.2f,  0.0f,		-1.0f, -1.0f, 0.0f,  //0번점
		-0.2f,  0.2f,  0.0f,		-1.0f, 1.0f, 0.0f,  //1번점
		 0.2f,  0.2f,  0.0f,		1.0f, 1.0f, 0.0f,  //2번점
		 0.2f,  -0.2f,  0.0f,		1.0f, -1.0f, 0.0f   //3번점
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

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 rm = glm::mat4(1.0f);
	glm::mat4 tx = glm::mat4(1.0f);

	tx = glm::translate(tx, glm::vec3(LightPos.x, LightPos.y, LightPos.z));

	model = model * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환


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

		if (y_rotate) {
			y_rotate = false;
		}

		else {
			y_rotate = true;
			glutTimerFunc(10, y_Rotate_Timer, 1);
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