#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "file2buf.h"
#include "OpenGL.h"
#define NUMBER_OF_VERTICES 30
bool InitProgram(unsigned int& ShaderProgram);

bool Check(unsigned int ShaderProgram);

void CreateAxis(GLuint& Axis);

void CreateTriangle(GLuint& EBO, GLuint& VBO);
