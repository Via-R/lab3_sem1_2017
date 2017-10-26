//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(250, 450);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(250, 150);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(550, 150);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(550, 450);
	glEnd();

	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*¬ключаем двойную буферизацию и четырехкомпонентный цвет*/

	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL lesson 1");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

/*
void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}



int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello, GL");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);

	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}


	glutMainLoop();
	return 0;
}*/