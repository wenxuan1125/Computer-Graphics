#include "Object.h"
#include "glut.h"
#include <math.h>
#define pi acos(-1)

int windowSize[2];
float theta_r = 0;
float theta_add = 0.5;
float scale = 1;
bool elephant2 = false;
bool elephant3 = false;
bool elephant4 = false;
bool elephant5 = false;
bool elephant6 = false;
bool stop = false;
bool color = false;
float last_theta_add = 0.5;

void light();
void display();
void reshape(GLsizei, GLsizei);
void LoadModel(Object*);
void DrawBasis();
void specialkeyboard(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);

Object* elephant = new Object("elephant.obj");
Object* cube = new Object("cube.obj");
Object* teapot = new Object("teapot.obj");


int main(int argc, char** argv)
{
	//std::cout << pi << endl;
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//light();
	glutCreateWindow("HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialkeyboard);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void light()
{
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 50.0, 75.0, 80.0, 1.0 };
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///TO DO Enable DepthTest
	///
	glEnable(GL_DEPTH_TEST);

	///TO DO: add lighting, Viewport Matrix, Projection Matrix, ModelView Matrix
	///
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	light();
	//Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, windowSize[0] / (GLdouble)windowSize[1], 1.0, 1000.0);

	//Viewport Matrix
	glViewport(0, 0, windowSize[0], windowSize[1]);

	

	///TO DO: add model, translate, rotate, scale
	/// 
	
	
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glRotatef(theta_r, 0, 1, 0);

	glScalef(scale, scale, scale);	//keyboard

	glPushMatrix();
	DrawBasis();

	glTranslatef(-3, 0, 3);
	LoadModel(teapot);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 0, -3);
	glRotatef(theta_r * 3, 0, 1, 0);
	//glScalef(2, 2, 2);
	
	LoadModel(elephant);
	

	if (elephant2) {
		glPushMatrix();
		glTranslatef(-2, 0, 2);
		glScalef(0.6, 0.6, 0.6);
		LoadModel(elephant);

		glPopMatrix();
	}
	if (elephant3) {
		glPushMatrix();
		glTranslatef(2, 0, 2);
		glScalef(0.4, 0.4, 0.4);
		LoadModel(elephant);

		glPopMatrix();
	}

	glTranslatef(-2, 0.5, -2);
	glScalef(0.5, 0.5, 0.5);
	
	LoadModel(cube);
	glPopMatrix();

	
	if (elephant4) {
		glPushMatrix();
		glTranslatef(3, 0, 5);
		glRotatef(theta_r * 3, 0, 1, 0);
		glScalef(0.8, 0.8, 0.8);
		LoadModel(elephant);

		if (elephant5) {
			glPushMatrix();
			glTranslatef(-2, 0, 2);
			glScalef(0.4, 0.4, 0.4);
			LoadModel(elephant);

			glPopMatrix();
		}
		glTranslatef(-1.5, 0.5, -1.5);
		glScalef(0.4, 0.4, 0.4);

		LoadModel(cube);
		glPopMatrix();
	}

	if (elephant6) {
		glPushMatrix();
		glTranslatef(-3, 0, -5);
		glRotatef(theta_r * 3, 0, 1, 0);
		glScalef(0.8, 0.8, 0.8);
		LoadModel(elephant);
		glTranslatef(-1.5, 0.5, -1.5);
		glScalef(0.4, 0.4, 0.4);

		LoadModel(cube);
		glPopMatrix();
	}
	
	/*
	glRotatef(theta_r, 0, 1, 0);

	glPushMatrix();
	DrawBasis();
	glTranslatef(-3, 0, 3);
	LoadModel(teapot);
	glPopMatrix();

	glPushMatrix();
	//glScalef(0.5, 0.5, 0.5);
	glTranslatef(3, 0, -3);
	glRotatef(theta_r * 3, 0, 1, 0);
	LoadModel(elephant);
	glTranslatef(-2, 0.5, -2);
	glScalef(0.5, 0.5, 0.5);
	LoadModel(cube);
	glPopMatrix();*/
	
	
	
	theta_r = theta_r + theta_add;

	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadModel(Object* Model)
{
	for (size_t i = 0; i < Model->fNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			glNormal3fv(Model->nList[Model->faceList[i][j].nIndex].ptr);
			glVertex3fv(Model->vList[Model->faceList[i][j].vIndex].ptr);
		}
		glEnd();
	}
}

void DrawBasis()
{
	///TO DO: draw basis
	///
	glBegin(GL_POLYGON);
	/*
	glVertex3f(0.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);*/
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	float x, y = 0, z, r = 10;
	float theta = 0; 
	float points[20][2];
	
	for (int i = 0; i < 20; i++) {
		
		x = r * sin(theta * pi);
		z = r * cos(theta * pi);
		theta = theta + 0.1;	// 18/360*2*pi = 0.1*pi
		points[i][0] = x;
		points[i][1] = z;
		glVertex3f(x, y, z);
	}
	glEnd();


	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	y = -5;
	for (int i = 0; i < 20; i++) {
		glVertex3f(points[i][0], y, points[i][1]);
	}
	glEnd();

	for (int i = 0; i < 20; i++) {

		glBegin(GL_POLYGON);
		// normal vector is the midpoint of the top edge
		float normal_l = sqrt((points[i][0] + points[(i + 1) % 20][0]) * (points[i][0] + points[(i + 1) % 20][0])\
			+ (points[i][1] + points[(i + 1) % 20][1]) * (points[i][1] + points[(i + 1) % 20][1])) / 2;
		float normal_x = (points[i][0] + points[(i + 1) % 20][0]) / 2 / normal_l;
		float  normal_y = 0;
		float normal_z = (points[i][1] + points[(i + 1) % 20][1]) / 2 / normal_l;
		glNormal3f(normal_x, normal_y, normal_z);
		glVertex3f(points[(i + 1) % 20][0], 0, points[(i + 1) % 20][1]);
		glVertex3f(points[i][0], 0, points[i][1]);
		glVertex3f(points[i][0], -5, points[i][1]);
		glVertex3f(points[(i + 1) % 20][0], -5, points[(i + 1) % 20][1]);
		glEnd();

	}
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	glEnd();

}

void specialkeyboard(int key, int x, int y)
{
	/// TO DO: Set the keyboard control
	///
	if (key == GLUT_KEY_UP) {
		if (scale < 1.9)
			scale = scale + 0.1;
		else
			scale = 2;
	}
	if (key == GLUT_KEY_DOWN) {
		if (scale > 0.5)
			scale = scale - 0.1;
		else
			scale = 0.5;
	}
	

}

void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		elephant2 = false;
		elephant3 = false;
		elephant4 = false;
		elephant5 = false;
		elephant6 = false;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 0.5;
		}
		last_theta_add = 0.5;
		
	}
	if (key == '2') {
		elephant2 = true;
		elephant3 = false;
		elephant4 = false;
		elephant5 = false;
		elephant6 = false;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 1;
		}
		last_theta_add = 1;
	}
	if (key == '3') {
		elephant2 = true;
		elephant3 = true;
		elephant4 = false;
		elephant5 = false;
		elephant6 = false;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 1.8;
		}
		last_theta_add = 1.8;
	}
	if (key == '4') {
		elephant2 = true;
		elephant3 = true;
		elephant4 = true;
		elephant5 = false;
		elephant6 = false;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 2.5;
		}
		last_theta_add = 2.5;
	}
	if (key == '5') {
		elephant2 = true;
		elephant3 = true;
		elephant4 = true;
		elephant5 = true;
		elephant6 = false;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 3.2;
		}
		last_theta_add = 3.2;
	}
	if (key == '6') {
		elephant2 = true;
		elephant3 = true;
		elephant4 = true;
		elephant5 = true;
		elephant6 = true;
		if (stop) {
			theta_add = 0;
		}
		else {
			theta_add = 4;
		}
		last_theta_add = 4;
	}
	if (key == 's') {
		if (stop) {
			stop = false;
			theta_add = last_theta_add;
		}
		else {
			stop = true;
			last_theta_add = theta_add;
			theta_add = 0;
		}
	}

}