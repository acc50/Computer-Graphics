#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "file2buf.h"

bool InitProgram(unsigned int& ShaderProgram);

bool Check(unsigned int ShaderProgram);

void CreateCube(GLuint& EBO, GLuint& VBO);
void CreateBoard(GLuint& EBO, GLuint& VBO);
void CreatePolygon(GLuint& VBO, GLuint& EBO);
