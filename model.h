#pragma once
#include "glut.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>    // strtof 함수가 선언된 헤더 파일
#define BUFFER_SIZE 1024
#define VERTICES 1 
#define FACIES 2

typedef struct {
	int number_of_vertex;
	float** vertices;
	int number_of_face;
	int** indices_of_each_face;
	int is_loaded;
	int dimmension;
	GLfloat* GLvertices;
	GLubyte* GLindicies;
	GLfloat* gravity_center; 

} MODEL;
void print_model(MODEL* pmodel);
void check_phase(char** string_return, int* ex_counter_return, char* string_input);
void interpreter_float(float** return_array, char* data);
void interpreter_int(int** return_array, char* data);

void readdata(MODEL** return_data, char* filename);
void set_graviy_center(MODEL* pmodel);

void free_model(MODEL* pmodel);
void check_TRIANGES(MODEL* pmodel, int* pbool);
void Glvertices_and_indices(MODEL* pmodel ); // only for Triangles