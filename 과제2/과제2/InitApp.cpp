
#include "InitApp.h"


bool InitProgram(unsigned int& ShaderProgram)
{
	const char* vertexSource = filetobuf("Vertex.glsl");
	const char* fragmentSource = filetobuf("Fragment.glsl");
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	if (!Check(vertexShader)) return false;

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	if (!Check(fragmentShader)) return false;

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (!Check(ShaderProgram)) {
		glDeleteProgram(ShaderProgram);
		return false;

	}

	
}


bool Check(unsigned int ShaderProgram)
{
	GLint state;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &state);
	if (state == GL_FALSE) {
		int infologLength = 0;
		glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char* infolog = new char[infologLength];
			glGetProgramInfoLog(ShaderProgram, infologLength, NULL, infolog);
			std::cout << infolog << std::endl;
			delete[] infolog;
		}
		return false;
	}
	return true;
}

void CreateRobot(GLuint EBO[4], GLuint VBO[4])
{
	GLfloat body_size = 0.3f;
	GLfloat head_size = 0.2f;

	GLfloat vertex[] = {
	-body_size, 2 * body_size, -body_size,		0.0f, 0.5f, 0.0f,  //0번점
	-body_size, 2 * body_size, body_size,		0.0f, 0.5f, 0.0f,  //1번점
	body_size, 2 * body_size, body_size,		0.0f, 0.5f, 0.0f,  //2번점
	body_size, 2 * body_size, -body_size,		0.0f, 0.5f, 0.0f,  //3번점

	-body_size, 0.0f, -body_size,				0.0f, 0.5f, 0.0f,  //4번점
	-body_size, 0.0f, body_size,				0.0f, 0.5f, 0.0f,  //5번점
	body_size, 0.0f, body_size,					0.0f, 0.5f, 0.0f,  //6번점
	body_size, 0.0f, -body_size,				0.0f, 0.5f, 0.0f,  //7번점

	-head_size, 2 * body_size + 2 * head_size, -head_size,		0.5f, 0.3f, 0.0f,  //8번점
	-head_size, 2 * body_size + 2 * head_size, head_size,		0.5f, 0.3f, 0.0f,  //9번점
	head_size, 2 * body_size + 2 * head_size, head_size,		0.5f, 0.3f, 0.0f,  //10번점
	head_size,2 * body_size + 2 * head_size, -head_size,		0.5f, 0.3f, 0.0f,  //11번점

	-head_size, 2 * body_size, -head_size,					0.5f, 0.3f, 0.0f,  //12번점
	-head_size, 2 * body_size, head_size,					0.5f, 0.3f, 0.0f,  //13번점
	head_size, 2 * body_size, head_size,					0.5f, 0.3f, 0.0f,  //14번점
	head_size, 2 * body_size, -head_size,					0.5f, 0.3f, 0.0f   //15번점
	};

	for (int i = 0; i < 4; ++i) {
		glGenBuffers(1, &VBO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	}


	//front
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
		0,3,7,



		8,9,10,
		8,10,11,  //윗면

		9,13,14,
		9,14,10, //앞면

		10,14,15,
		10,15,11, //우측면

		8,12,13,
		8,13,9, //좌측면

		13,12,14,
		12,15,14,// 밑면

		8,15,12, //뒷면
		8,11,15
	};


	for (int i = 0; i < 4; ++i) {
		glGenBuffers(1, &EBO[i]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);
	}

}

void CreateBoard(GLuint& EBO, GLuint& VBO)
{
	GLfloat vertex[] = {
	-5.0f,  0.0f,  5.0f,		0.2f, 0.2f, 0.2f,  //0번점
	-5.0f,  0.0f, -5.0f,		0.2f, 0.2f, 0.2f,  //1번점
	 5.0f,  0.0f, -5.0f,		0.2f, 0.2f, 0.2f,  //2번점
	 5.0f,  0.0f,  5.0f,		0.2f, 0.2f, 0.2f   //3번점
	};

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLint gIndices[]
	{
		0,2,1,
		0,3,2
	};

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);

}

void CreateCube(GLuint &EBO, GLuint &VBO)
{

	GLfloat vertex[] = {
		-0.1f, 0.1f, -0.1f,		0.0f, 0.0f, 0.0f,  //0번점
		-0.1f, 0.1f, 0.1f,		0.0f, 0.0f, 0.0f,  //1번점
		0.1f, 0.1f, 0.1f,		0.0f, 0.0f, 0.0f,  //2번점
		0.1f, 0.1f, -0.1f,		0.0f, 0.0f, 0.0f,  //3번점

		-0.1f, -0.1f, -0.1f,	0.0f, 0.0f, 0.0f,  //4번점
		-0.1f, -0.1f, 0.1f,		0.0f, 0.0f, 0.0f,  //5번점
		0.1f, -0.1f, 0.1f,		0.0f, 0.0f, 0.0f,  //6번점
		0.1f, -0.1f, -0.1f,		0.0f, 0.0f, 0.0f,  //7번점
	};


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);



	//front
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
		0,3,7,
	};


	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);


}

void CreateObstacle(GLuint &EBO, GLuint &VBO)
{
	GLfloat len = (float)LENGTH;

	GLfloat vertex[] = {
			-len, len * 4, -len,		0.0f, 0.0f, 0.0f,  //0번점
			-len, len * 4, len,		0.0f, 0.0f, 0.0f,  //1번점
			len, len * 4, len,		0.0f, 0.0f, 0.0f,  //2번점
			len, len * 4, -len,		0.0f, 0.0f, 0.0f,  //3번점

			-len, 0.0f, -len,		0.0f, 0.0f, 0.0f,  //4번점
			-len, 0.0f, len,		0.0f, 0.0f, 0.0f,  //5번점
			len, 0.0f, len,		0.0f, 0.0f, 0.0f,  //6번점
			len, 0.0f, -len,		0.0f, 0.0f, 0.0f,  //7번점
	};


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);



	//front
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
		0,3,7,
	};


	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);

}