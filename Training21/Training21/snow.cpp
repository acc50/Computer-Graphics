#include "snow.h"
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<GLfloat> urd(-20.0f, 20.0f);
std::uniform_real_distribution<GLfloat> height(10.0f, 15.0f);

snow::snow()
{
}


snow::~snow()
{
}

void snow::Init()
{
	this->Pos.x = urd(dre);
	this->Pos.y = height(dre);
	this->Pos.z = urd(dre);


}

void snow::Fall()
{
	this->Pos.y -= this->fall_speed;

	if (this->Pos.y < 0.0f) {
		this->Init();
	}
}

void snow::Render(GLuint ShaderProgram, GLuint VBO, GLuint EBO)
{
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// ¸ðµ¨º¯È¯
	glm::mat4 tx = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	tx = glm::translate(tx, glm::vec3(Pos.x, Pos.y, Pos.z));

	model = model * tx;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// ¸ðµ¨º¯È¯

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	glUniform3f(objColorLocation, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 2880, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
