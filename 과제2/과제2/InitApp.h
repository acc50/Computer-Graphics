#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "file2buf.h"
#include "openGL.h"

#define LENGTH 0.3

bool InitProgram(unsigned int& ShaderProgram);

bool Check(unsigned int ShaderProgram);

void CreateBoard(GLuint& EBO, GLuint& VBO);
void CreateRobot(GLuint EBO[4], GLuint VBO[4]);
void CreateCube(GLuint& EBO, GLuint& VBO);
void CreateObstacle(GLuint& EBO, GLuint&VBO);