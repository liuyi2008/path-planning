#include <stdlib.h>
#include <GL/glut.h> 
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include<iostream>
#include <string> 
#include <fstream>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include<vector>
using std::vector;
//using namespace std;
GLfloat x = 0.0;

GLfloat y = 0.0;

GLfloat size = 50.0;

GLsizei wh = 500, ww = 500;

struct point
{
	int x;
	int y;
}p;

vector<point>polypoint;

bool scan = false;


void drawSquare(GLint x, GLint y) {

	y = wh - y;

	glBegin(GL_POLYGON);

	glVertex3f(x + size, y + size, 0);

	glVertex3f(x - size, y + size, 0);

	glVertex3f(x - size, y - size, 0);

	glVertex3f(x + size, y - size, 0);

	glEnd();

}

void drawPolugon()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < polypoint.size(); i++)
	{
		glVertex2i(polypoint[i].x,wh- polypoint[i].y);
	}
	glEnd();
}

void ControlPoint()

{

	glPointSize(2);

	for (int i = 0; i < polypoint.size(); i++)

	{

		glBegin(GL_POINTS);

		glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(polypoint[i].x, wh- polypoint[i].y);

		glEnd();

	}


}

void mydisplay()

{

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);

	ControlPoint();
	//drawPolugon();
	if (scan) 
	{
		for (int i = 0; i < polypoint.size(); i++)
		{
		    printf("x:%d,y:%d\n", polypoint[i].x, wh - polypoint[i].y);
		}
		drawPolugon();
		//scan = false;
	}
	

	glutSwapBuffers();

	glutPostRedisplay();

}

void init()

{

	glClearColor(1.0, 1.0, 1.0, 0.0);

}

void myreshape(GLint w, GLint h) {

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();          //³õÊ¼»¯

	glOrtho(0, w, 0, h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	ww = w;

	wh = h;

}

void mymouse(GLint button, GLint state, GLint wx, GLint wy)

{
	

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)

		exit(0);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

	{

		int x = wx;
		int y = wy;
		printf("x:%d,y:%d", x, y);
		p = { x,y };
		polypoint.push_back(p);
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)

		scan = true;

}

void main(int argc, char** argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(500, 500);

	glutInitWindowPosition(0, 0);

	glutCreateWindow("double");

	init();

	glutDisplayFunc(mydisplay);

	glutReshapeFunc(myreshape);

	glutMouseFunc(mymouse);

	glutMainLoop();

}
