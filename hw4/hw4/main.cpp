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

using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void reshape(GLsizei w, GLsizei h);
void DrawUmbreon(GLuint);
void LoadTexture(unsigned int&, const char*);

GLuint Normalprogram, Umbreonprogram, Triangleprogram, Explodeprogram;
GLuint VAO, VBO[3];
unsigned int modeltexture;
float windowSize[2] = { 600, 600 };
float angle = 0.0f;
float magnitude = 0.0f;
bool explode = false;
bool stop = false;
bool triangle = true;
float show = 0.0f;
float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

glm::vec3 WorldLightPos = glm::vec3(2, 5, 5);
glm::vec3 WorldCamPos = glm::vec3(7.5, 5.0, 7.5);

Object* model = new Object("Umbreon.obj");

int main(int argc, char** argv) {


	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw4");

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
	GLuint vert = createShader("Shaders/normal.vert", "vertex");
	GLuint goem = createShader("Shaders/normal.geom", "geometry");
	GLuint frag = createShader("Shaders/normal.frag", "fragment");
	Normalprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/triangle.vert", "vertex");
	goem = createShader("Shaders/triangle.geom", "geometry");
	frag = createShader("Shaders/triangle.frag", "fragment");
	Triangleprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/explode.vert", "vertex");
	goem = createShader("Shaders/explode.geom", "geometry");
	frag = createShader("Shaders/explode.frag", "fragment");
	Explodeprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/Umbreon.vert", "vertex");
	frag = createShader("Shaders/Umbreon.frag", "fragment");
	Umbreonprogram = createProgram(vert, 0, frag);




}

void bindbufferInit() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->positions[0]) * model->positions.size(), &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->normals[0]) * model->normals.size(), &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->texcoords[0]) * model->texcoords.size(), &model->texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(modeltexture, "Umbreon.jpg");
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 's':
	{
		stop = !stop;
		explode = false;
		break;
	}
	case 'e':
	{
		explode = true;
		triangle = false;
		show = 0;
		break;
	}
	case 'w':
	{
		explode = true;
		triangle = true;
		break;
	}
	}
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 45.0f;
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

	//DrawUmbreon(Normalprogram);

	DrawUmbreon(Triangleprogram);
	DrawUmbreon(Explodeprogram);
	DrawUmbreon(Umbreonprogram);



	if (!stop) {
		angle += 0.05;




		if (b < 1) {

			b += 0.05;
		}
		else if (g < 1) {
			g += 0.05;
		}
		else if (r < 1) {

			r += 0.05;
		}

		else {
			r = 0;
			g = 0;
			b = 0;
		}

		if (explode) {
			if (magnitude < 20)
				magnitude += 0.01;
			else if (magnitude < 40)
				magnitude += 0.008;
			else
				magnitude += 0.005;

			if (magnitude > 60) {
				magnitude = 0;
				explode = false;
				triangle = true;

			}
		}
		else {
			show += 0.0015;

			if (show > 200)
				show = 200;
		}

	}


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

void idle() {
	glutPostRedisplay();
}

void DrawUmbreon(GLuint program)
{
	glUseProgram(program);

	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	//M = glm::translate(M, glm::vec3(0, 0, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);

	GLuint ParameterID = glGetUniformLocation(program, "r");
	glUniform1f(ParameterID, r);

	ParameterID = glGetUniformLocation(program, "g");
	glUniform1f(ParameterID, g);

	ParameterID = glGetUniformLocation(program, "b");
	glUniform1f(ParameterID, b);

	ParameterID = glGetUniformLocation(program, "magnitude");
	glUniform1f(ParameterID, magnitude);

	ParameterID = glGetUniformLocation(program, "triangle");
	glUniform1i(ParameterID, triangle);

	ParameterID = glGetUniformLocation(program, "show");
	glUniform1f(ParameterID, show);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * model->fNum);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}


