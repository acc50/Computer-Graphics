
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

void CreateCube(GLuint& EBO, GLuint& VBO)
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
		0,3,7
	};

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), &gIndices, GL_STATIC_DRAW);



}

void CreateBoard(GLuint& EBO, GLuint& VBO)
{
	GLfloat vertex[] = {
	-1.0f,  0.0f,  1.0f,		0.0f,0.0f,1.0f,  //0번점
	-1.0f,  0.0f, -2.0f,		0.0f,0.0f,1.0f,  //1번점
	 1.0f,  0.0f, -2.0f,		0.0f,0.0f,1.0f,  //2번점
	 1.0f,  0.0f,  1.0f,		0.0f,0.0f,1.0f   //3번점
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

void CreatePolygon(GLuint& VBO, GLuint& EBO)
{
	GLfloat vertex[] = {
		-0.1f,  -0.1f,  0.0f,		0.0f,0.0f,1.0f,  //0번점
		-0.1f,  0.1f,  0.0f,		0.0f,0.0f,1.0f,  //1번점
		 0.1f,  0.1f,  0.0f,		0.0f,0.0f,1.0f,  //2번점
		 0.1f,  -0.1f,  0.0f,		0.0f,0.0f,1.0f   //3번점
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