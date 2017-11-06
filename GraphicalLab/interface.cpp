#include <GL\glew.h>
#include <GL\freeglut.h>
#include "Voronoi.h"

#include <iostream>
using namespace std;


static GLfloat pointsColor[3] = { 0.96, 0.36, 0.24 };
static GLfloat linesColor[3] = { 0.96, 0.36, 0.24 };
const float border = 1.0;


void setPointsColor(int r, int g, int b) {
	pointsColor[0] = (GLfloat)r / 255;
	pointsColor[1] = (GLfloat)g / 255;
	pointsColor[2] = (GLfloat)b / 255;
}

void setLinesColor(int r, int g, int b) {
	linesColor[0] = (GLfloat)r / 255;
	linesColor[1] = (GLfloat)g / 255;
	linesColor[2] = (GLfloat)b / 255;
}


void drawPoint(float x, float y) {
	glBegin(GL_POINTS);
	glColor3f(pointsColor[0], pointsColor[1], pointsColor[2]);
	glVertex2f((GLfloat)x, (GLfloat)y);
	glEnd();
}
void drawLine(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINES);
	glColor3f(linesColor[0], linesColor[1], linesColor[2]);
	glVertex2f((GLfloat)x1, (GLfloat)y1);
	glVertex2f((GLfloat)x2, (GLfloat)y2);
	glEnd();
}

void display() {
	glClearColor(0.58f, 0.65f, 0.65f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(15.0);
	glLineWidth(8.0);
	glEnable(GL_POINT_SMOOTH);

	setPointsColor(37, 40, 61);
	setLinesColor(30, 3, 120);
	
	//randomize(10, 1);
	
	/*
	float rx, ry;
	for (int i = 0; i < 21; i++){
		rx = getRandNum() * 2 * border - border;
		ry = getRandNum() * 2 * border - border;
		cout << rx << " " << ry << endl;
		drawPoint(rx, ry);
	}*/
	//drawLine(0, 0, 2, 1);
	glutSwapBuffers();
	//glFlush();
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(-border * aspect, border * aspect, -border, border);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-border, border, -border / aspect, border / aspect);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:     // ESC key
		exit(0);
		break;
	}
}

void startPresentation() {
	
	customSet();
	initializeQueue();
	startAlgorithm();

	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*�������� ������� ����������� � ������������������� ����*/

	glutInitWindowPosition(200, 50);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Drawing simple things");
	//glutFullScreen();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//glutIdleFunc()

	glutMainLoop();
}
