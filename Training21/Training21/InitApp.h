#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "file2buf.h"
#include "openGL.h"

bool InitProgram(unsigned int& ShaderProgram);

bool Check(unsigned int ShaderProgram);

void CreateCone(GLuint& EBO, GLuint& VBO);
void CreateBoard(GLuint& EBO, GLuint& VBO);
void CreatePolygon(GLuint& VBO, GLuint& EBO);
void CreateSphere(GLuint& EBO, GLuint& VBO);
