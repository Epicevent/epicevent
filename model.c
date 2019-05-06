
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"


void print_model(MODEL* pmodel) {
	printf(":::::::::::::model :::::::::::::::::\n ");
	printf("dimmension : %d\
		number of face : %d \
		number of vertex : %d \
		is loaded :%d\n", pmodel->dimmension, pmodel->number_of_face, pmodel->number_of_vertex, pmodel->is_loaded);
	int i, j;

	if (pmodel->GLvertices)
	{
		printf("VERTEX\n");
		for (j = 0; j < pmodel->number_of_vertex; j++) {
			for (i = 0; i < pmodel->dimmension; i++) {
				printf("%f ", pmodel->GLvertices[i + pmodel->dimmension * j]);
			}
			printf("\n");
		}
	}
	if (pmodel->GLindicies) {
		printf("FACE (INDICIES)\n");
		for (j = 0; j < pmodel->number_of_face; j++) {
			for (i = 0; i < 3; i++) {
				printf("%d ", pmodel->GLindicies[i + 3 * j]);
			}
			printf("\n");
		}
	}
	if (pmodel->gravity_center) {
		printf("CENTER \n");
		printf("(%f , %f , %f)\n" ,pmodel->gravity_center[0],pmodel->gravity_center[1],pmodel->gravity_center[2]);
	}

}
void set_graviy_center(MODEL* pmodel) {
	if (!pmodel)
		return;
	if (!pmodel->number_of_vertex)
		return;
	GLfloat* centers;
	int i, j;
	centers = (GLfloat*)malloc(sizeof(GLfloat) * pmodel->dimmension );
	for (i = 0; i < pmodel->dimmension; i++)
		centers[i] = 0;

	for (i = 0; i < pmodel->dimmension; i++) {
		for (j = 0; j < pmodel->number_of_vertex; j++) {
			centers[i] +=  (pmodel->vertices[j][i+1]);
		}
	} 
	
	for (i = 0; i < pmodel->dimmension; i++)
		centers[i] /= pmodel->number_of_vertex;
	pmodel->gravity_center = centers;

}


void free_model(MODEL* pmodel) {
	int i;
	if (!pmodel)
		return;
	for (i = 0; i < pmodel->number_of_vertex; i++)
	{
		free(pmodel->vertices[i]);
	}
	for (i = 0; i < pmodel->number_of_face; i++) {
		free(pmodel->indices_of_each_face[i]);
	}
	free(pmodel->vertices);
	free(pmodel->indices_of_each_face);
	free(pmodel->GLvertices);
	free(pmodel->GLindicies);
	free(pmodel->gravity_center);
	free(pmodel);

}
void check_TRIANGES(MODEL* pmodel, int* pbool) {
	int bool_val = 0;
	int i;
	if (pmodel ) {
		bool_val = 1;
		for (i = 0; i < pmodel->number_of_vertex; i++) {
			if (pmodel->vertices[i][0] != 3)
			{
				bool_val = 0;
				break;
			}
		}
		for (i = 0; i < pmodel->number_of_face; i++) {
			if (pmodel->indices_of_each_face[i][0] != 3)
			{
				bool_val = 0;
				break;
			}
		}
		*pbool = bool_val;
	}

}

void Glvertices_and_indices(MODEL* pmodel) { // only for Triangles in 3D
	if (!pmodel)
		return;
	if (pmodel->is_loaded && pmodel->GLvertices && pmodel->GLindicies)
		return;
	GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat) * pmodel->number_of_vertex * pmodel->dimmension);//24
	GLubyte* indicies = (GLubyte*)malloc(sizeof(GLubyte) * pmodel->number_of_face * 3);//36
	// order is vvvvery important 

	int i, j;
	for (i = 0; i < pmodel->number_of_vertex; i++) {
		if (pmodel->vertices[i][0] != 3)
			return;
		for (j = 1; j <= 3; j++) {
			vertices[3 * i + j - 1] = pmodel->vertices[i][j];
		}
	}
	for (i = 0; i < pmodel->number_of_face; i++) {
		if (pmodel->indices_of_each_face[i][0] != 3)
			return;
		for (j = 1; j <= 3; j++) {
			indicies[3 * i + j - 1] = pmodel->indices_of_each_face[i][j];
		}
	}
	pmodel->GLvertices = vertices;
	pmodel->GLindicies = indicies;
	pmodel->is_loaded = 1;
}        


void check_phase(char** string_return, int* ex_counter_return, char* string_input) {

	char* phase = NULL; // 
	printf("-------\nstring in : %s",string_input);
	printf("\n");
	int ex_counter = 0;
	int i = 0;
	int alpha_start_index = -1;
	int alpha_end_index = -1;
	int digit_start_index = -1;
	if (string_input != NULL) {
		phase = (char*)malloc(BUFFER_SIZE);
		int input_len = strlen(string_input);
		for (i = 0; i < input_len; i++) {
			if (isalpha(string_input[i]))
			{
				if (alpha_start_index == -1)
					alpha_start_index = i;
				alpha_end_index = i;
			}
			if (digit_start_index == -1 && isdigit(string_input[i]))
				digit_start_index = i;
		}
		if (alpha_start_index != -1 &&
			alpha_end_index != -1) {
			int n = alpha_end_index - alpha_start_index;
			if (!strncmp(string_input + alpha_start_index, "VERTEX", n)) {
				strncpy(phase, "vertices", 9);
				ex_counter = atoi(string_input + digit_start_index);
			}
			else if (!strncmp(string_input + alpha_start_index, "FACE", n)) {
				strncpy(phase, "facies", 7);
				ex_counter = atoi(string_input + digit_start_index);
			}
			else {
				printf("잘못된 파일 형식입니다. \n");
				string_input[alpha_end_index + 1] = 0;
				printf("%s 을 해석하지 못하였습니다. ", string_input);
			}
		}
		else {
			if (digit_start_index == -1) {
				free(phase);
				phase = NULL;
			}
			else {
				strncpy(phase, "numbers", 8);
			}
		}
	}
	*string_return = phase;
	*ex_counter_return = ex_counter;
}

void interpreter_int(int** return_array,char* data) {
	int* ret_array_int = NULL;
	char* start = data;
	char* end = NULL;
	int max_num;
	int counter = 0;
	int indices_inf = 0;
	if (data != NULL) {
		max_num = 1 + (strlen(data) + 1) / 2;
		ret_array_int = (int*)malloc(sizeof(int) * max_num);
		while (1) {
			indices_inf = (int)(strtof(start, &end));

			ret_array_int[counter + 1] = indices_inf;
			if (start == end)
				break;
			printf("  %d  ", indices_inf);
			start = end;
			counter += 1;
		}

		printf(" (%d)\n", counter);
		ret_array_int[0] = counter;
		ret_array_int = (int*)realloc(ret_array_int, (counter + 1) * sizeof(int));
		*return_array = ret_array_int;
		printf("%d %d %d %d\n", (*return_array)[0], (*return_array)[1], (*return_array)[2], (*return_array)[3]);
	}


}

void interpreter_float(float** return_array,char* data) {
	float* ret_array_float = NULL;
	char* start = data;
	char* end = NULL;
	int max_num;
	int counter = 0;
	float vetices_pos = 0;
	if (data != NULL) {
		max_num = 1 + (strlen(data) + 1) / 2;
		ret_array_float = (float*)malloc(sizeof(float) * max_num);

		while (1) {
			vetices_pos = strtof(start, &end);

			ret_array_float[counter + 1] = vetices_pos;
			if (start == end)
				break;
			printf("  %f  ", vetices_pos);
			start = end;
			counter += 1;
		}

		printf(" (%d)\n", counter);
		ret_array_float[0] = counter;
		ret_array_float = (float*)realloc(ret_array_float, (counter + 1) * sizeof(float));
		*return_array = ret_array_float;
		printf("%f %f %f %f\n", (*return_array)[0], (*return_array)[1], (*return_array)[2], (*return_array)[3]);
	}


}
void readdata(MODEL * *return_data, char* filename) {
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL) {
		printf("파일을 열 수 없습니다. 실행파일이 있는 곳에 model.dat이 있는지 확인해 주세요");
		return;
	}
	char* buffer[BUFFER_SIZE];
	char* pStr;
	MODEL* pmodel = (MODEL*)malloc(sizeof(MODEL));
	pmodel->is_loaded = 0;
	pmodel->GLindicies = NULL;
	pmodel->GLvertices = NULL;
	pmodel->gravity_center = NULL;
	int ex_counter = 0;
	int in_counter = 0;
	char* phase = NULL;
	int no_malloc = 0;
	int phase_state = 0;
	while (!feof(pFile))
	{
	
		pStr = fgets(buffer, sizeof(buffer), pFile);
		
		check_phase(&phase, &ex_counter, pStr);

		if (phase ==NULL){
			printf("READ FILE END \n");
		}
		else {
			in_counter += 1;
			printf("%s\n", phase);
			if ((!strcmp(phase, "numbers"))) {
				if (phase_state == VERTICES) {
					interpreter_float(&pmodel->vertices[in_counter - 1], pStr);
				}
				else if (phase_state == FACIES) {
					interpreter_int(&pmodel->indices_of_each_face[in_counter - 1], pStr);
				}
		}
			else if (!strcmp(phase, "vertices")) {
				pmodel->vertices = (float**)malloc(sizeof(float*) * ex_counter);
				pmodel->number_of_vertex = ex_counter;
				phase_state = VERTICES;
				in_counter = 0;


			}
			else if (!strcmp(phase, "facies")) {
				pmodel->indices_of_each_face = (int**)malloc(sizeof(int*) * ex_counter);
				pmodel->number_of_face = ex_counter;
				phase_state = FACIES;
				in_counter = 0;
			}
			free(phase);
			phase = NULL;
		}

	}
	free(phase);
	pmodel->dimmension = pmodel->vertices[0][0];
	*return_data = pmodel;
}