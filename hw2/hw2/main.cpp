#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>
#include <math.h>
#define pi acos(-1)


using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawBasis();
void DrawUmbreon();
void LoadTexture(unsigned int&, const char*);

GLuint program;
GLuint VAO, VBO[3];
unsigned int basistexture, modeltexture, bonustexture;
int windowSize[2] = { 600, 600 };
float angle = 0.0f;
float theta_j = 0;
bool stop = false;
bool jump = false;
bool texture2 = false;
bool increment = false;
float incre = 0;


Object* model = new Object("UmbreonHighPoly.obj");

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw2");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	//// TODO: ////
	//
	// Hint:
	// 1. createShader
	// 2. createProgram
	GLuint vert = createShader("Shaders/vertexShader.vert", "vertex");
	GLuint frag = createShader("Shaders/fragmentShader.frag", "fragment");

	program = createProgram(vert, frag);

}

void bindbufferInit() {
	//// TODO: ////
	//
	//	 Hint:
	// 1. Setup VAO
	// 2. Setup VBO of vertex positions, normals, and texcoords

	// Set up VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Bind 3 VBO buffer objects
	// One for positions, another for normals, and the other for texcoords
	//glGenBuffers(3, VBO);
	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (model->positions.size()), model->positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (model->normals.size()), model->normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (model->texcoords.size()), model->texcoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(basistexture, "basis.jpg");
	LoadTexture(modeltexture, "Umbreon.jpg");
	LoadTexture(bonustexture, "test.jpg");
	
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(7.5, 5.0, 7.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
glm::mat4 getP()
{
	// set perspective view
	float fov = 60.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawUmbreon();

	DrawBasis();

	// angle += 0.1;
	// bonus
	if (stop)
		angle = angle;
	else
		angle += 0.1;

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 's') {
		stop = !stop;
		jump = false;
	}
	if (key == 'j') {
		jump = !jump;
		theta_j = 0;
	}
	if (key == 't') {
		texture2 = !texture2;
		incre = 0;
	}
	if (key == 'i') {
		increment = !increment;
		incre = 0;
	}

}

void idle() {
	glutPostRedisplay();
}

void DrawBasis() {
	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(7.5, 5.0, 7.5,// eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	//// if you don't need this, you can just deleting
	//float edge = 20;
	//float radius = 4;
	//float pi = 3.1415926;

	//// TODO: ////
	//
	// draw the basis and make the side of basis with texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, basistexture);

	glBegin(GL_POLYGON);

	glNormal3f(0.0f, 1.0f, 0.0f);
	float x, y = 0, z, r = 4;
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

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glEnd();


	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	y = -3.5;
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
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(points[(i + 1) % 20][0], 0, points[(i + 1) % 20][1]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(points[i][0], 0, points[i][1]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(points[i][0], -3.5, points[i][1]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(points[(i + 1) % 20][0], -3.5, points[(i + 1) % 20][1]);
		glEnd();

	}


	glPopMatrix();
}

void DrawUmbreon()
{
	glUseProgram(program);

	
	glm::mat4 M(1.0f);	// an identity 4*4 matrix M
	//radians(angle): degree to radian, vec3(0, 1, 0): rotate around y-axis
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	M = glm::translate(M, glm::vec3(0, 1.3, 0));
	
	if (jump) {
		M = glm::translate(M, glm::vec3(0, 1.3 * sin(theta_j / 180 * pi) + 1.3, 0));
		theta_j = theta_j + 0.25;
	}
		

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	//// TODO: ////
	// pass projection matrix, and view matrix and trigger by Uniform (use getP() amd getV())
	// also pass modeltexture to shader and trigger by Uniform
	//input the modelview matrix into vertex shader
	glm::mat4 P = getP();
	GLint pmatLoc = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, &P[0][0]);

	//input the rotation matrix into vertex shader
	glm::mat4 V = getV();
	GLint vmatLoc = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, &V[0][0]);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	GLint textLoc = glGetUniformLocation(program, "Texture");
	glUniform1i(textLoc, 1);

	//bonus
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bonustexture);
	GLint textLoc2 = glGetUniformLocation(program, "Texture2");
	glUniform1i(textLoc2, 2);
	
	GLint Loc = glGetUniformLocation(program, "text2");
	glUniform1i( Loc, int(texture2));

	if (increment) {
		GLint Loc2 = glGetUniformLocation(program, "n");
		glUniform1f(Loc2, float(incre));
		//cout << incre << endl;
		incre = incre + 0.00005;
		if (incre > 0.9)
			incre = 0;
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, 4 * model->fNum);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glUseProgram(0);
}