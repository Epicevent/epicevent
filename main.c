
#include "glut.h"
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>

MODEL* g_modeldata = NULL;
MODEL* g_modeldata1 = NULL;
MODEL* g_modeldata2 = NULL;
MODEL* g_modeldata3 = NULL;
MODEL* g_modeldata4 = NULL;
GLfloat G_radian = 0.f;
const float rad_constant = 180 / M_PI;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_angle_velocity = - 0.001f;
GLfloat global_centerx = -2, global_centery = 1, global_centerz = 0;
void draw_data(MODEL* modeldata) {
	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer(modeldata->dimmension, GL_FLOAT, 0, modeldata->GLvertices);
	// draw a model

	// NUM OF EDGES OF TRIANGE  == 3 ..
	glDrawElements(GL_TRIANGLES, modeldata->number_of_face * 3 , GL_UNSIGNED_BYTE, modeldata->GLindicies);
	glDisableClientState(GL_VERTEX_ARRAY);
}

int sign(GLfloat re_num) {
	return (0 < re_num) - (0 > re_num);
}

void size_up_and_down_mr(GLfloat angle, GLfloat radius, GLfloat minima_ratio) {
	// radius and minima_ratio must be positive
	radius = sign(radius) * radius;
	minima_ratio = sign(minima_ratio) * minima_ratio;
	//minima_ratio must less than 1
	if (minima_ratio > 1)
		minima_ratio = 1;
	float start_angle = acos(sqrt((1-minima_ratio) / radius));
	float x = cos(angle/2 + start_angle);
	float ratio = radius * x * x;
	ratio += minima_ratio;
	glScalef(ratio, ratio, ratio);
}

void size_up_and_down(GLfloat angle , GLfloat radius){
	size_up_and_down_mr(angle, radius, 0);
}  

void render_model( MODEL *a_modeldata ,void (*func)(MODEL* )){
	glPushMatrix();
		glLoadIdentity();
		func(a_modeldata);
		draw_data(a_modeldata);
	glPopMatrix();
}

void rotate_itself(MODEL* a_modeldata) {
	glTranslatef(a_modeldata->gravity_center[0], a_modeldata->gravity_center[1], a_modeldata->gravity_center[2]);
	glRotatef(rad_constant * G_radian, 0, 0, 1);
	glTranslatef(-a_modeldata->gravity_center[0], -a_modeldata->gravity_center[1], -a_modeldata->gravity_center[2]);
}

void revolve_around_global_center(MODEL* a_modeldata) {
	glTranslatef(global_centerx,global_centery,global_centerz);
	glRotatef(rad_constant * G_radian, 0, 0, 1);
	glTranslatef(-global_centerx, -global_centery, -global_centerz);
}

void Increase_and_decrease(MODEL* a_modeldata) {
	glTranslatef(a_modeldata->gravity_center[0], a_modeldata->gravity_center[1], a_modeldata->gravity_center[2]);
	size_up_and_down_mr(G_radian, 1, 0.5);
	glTranslatef(-a_modeldata->gravity_center[0], -a_modeldata->gravity_center[1], -a_modeldata->gravity_center[2]);
}

void dummy(MODEL* a_modeldata) {

}

void	RenderScene(void)
{
	// Clear the window with current clearing glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.5f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	render_model(g_modeldata, dummy);
	glutSwapBuffers();
}


void SetupRC(void)
{
	// setup clear color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}


void idle(void) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	g_timeDelta = currentTime - g_prevTime;
	G_radian += g_angle_velocity * g_timeDelta;
	if (G_radian > M_PI)
		G_radian -= 2* M_PI;
	if (G_radian < -M_PI)
		G_radian += 2 * M_PI;
	g_prevTime = currentTime;
	glutPostRedisplay();



}

void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat  aspectRatio;
	if (h == 0)
		h = 1;
	aspectRatio = (GLfloat)h / (GLfloat)w;

	glViewport(0, 0, w, h);

	
	
}

void setting_model(MODEL* a_modeldata) {
	int check_triangles;
	check_TRIANGES(a_modeldata, &check_triangles);
	if (check_triangles) {
		Glvertices_and_indices(a_modeldata);
		set_graviy_center(a_modeldata);
	}
	else {
		printf("primtive GL_TRIANGLE 적용불가능한 사례는 적용하지 않았습니다.");
	}
	print_model(a_modeldata);
}

void initialize_models() {
	MODEL* pmodel, * pmodel1, * pmodel2, * pmodel3, * pmodel4;
	readdata(&pmodel, "Chair.dat");
	//readdata(&pmodel1, "model1.dat");
	//readdata(&pmodel2, "model2.dat");
	//readdata(&pmodel3, "model3.dat");
	//readdata(&pmodel4, "model4.dat");
	g_modeldata = pmodel;
	//g_modeldata1 = pmodel1;
	//g_modeldata2 = pmodel2;
	//g_modeldata3 = pmodel3;
	//g_modeldata4 = pmodel4;
	setting_model(g_modeldata);
	//setting_model(g_modeldata1);
	//setting_model(g_modeldata2);
	//setting_model(g_modeldata3);
	//setting_model(g_modeldata4);

}

void ortho_projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-300.f, 300.f, -300.f, 300.f, 300.f, -300.f);

}

void perspective_projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,600,300,-300);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'q') {
		glMatrixMode(GL_PROJECTION);
		glRotated(1, 0, 0, 1);
	}
	if (key == 'e') {
		glMatrixMode(GL_PROJECTION);
		glRotated(-1, 0, 0, 1);
	}

	if (key == 'w') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(0,0,-1);
	}
	if (key == 's') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(0, 0, );
	}

	if (key == 'a') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(1, 0, 0);
	}

	if (key == 'd') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(-1, 0, 0);
	}

	if (key == 'r') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(0, 1, 0);
	}
	if (key == 'f') {
		glMatrixMode(GL_PROJECTION);
		glTranslated(0, -1, 0);
	}
	if (key == 'p') {
		perspective_projection();
	}
	if (key == 'o') {
		ortho_projection();
	}

}

int	main(int argc, char* argv[])
{

	initialize_models();
	glutInit(&argc, argv); // initialize GL context

	// single or double buffering | RGBA color mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

 // open window
	glutCreateWindow("20152424 - rocky kim");
	//define callback
		// callback function
		glutDisplayFunc(RenderScene);
		glutIdleFunc(idle);
		glutReshapeFunc(ChangeSize);
		glutKeyboardFunc(keyboard);

	SetupRC();			// initialize render context(RC)
	ortho_projection();


	glutMainLoop();		// run GLUT framework


	free_model(g_modeldata);
	//free_model(g_modeldata1);
	//free_model(g_modeldata2);
	//free_model(g_modeldata3);
	//free_model(g_modeldata4);
	return 0;
} 