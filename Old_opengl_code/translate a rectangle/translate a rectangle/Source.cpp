#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0


#include<stdio.h>
#include<GL/freeglut.h>
#include<math.h>


#define windowWidth 600
#define windowHeight 300


int par = 0;
int scale = 0.5;


void init(void) {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}
void mouse(int button, int state, int mousex, int mousey)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		par = 1;glutPostRedisplay();
	} else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		par = 2;glutPostRedisplay();
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		par = 3;glutPostRedisplay();
	} else {
		par = 0;
	}
	
}
void Rectangle() {
	int rectx = 30, recty = 30;
	glColor3f(0.3, 0.85, 1.0);
	glBegin(GL_POLYGON);
	glVertex2i(0, 0);
	glVertex2i(rectx, 0);
	glVertex2i(rectx, recty);
	glVertex2i(0, recty);
	glEnd();
}
void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.9, 1, 1, 0.8);
	if (par == 0) {
		Rectangle();
	} 
	else if (par == 1) {
		Rectangle();
		glTranslatef(20, 20, 0);
	}
	else if (par == 2) {
		Rectangle();
		glTranslatef(-20, -20, 0);
	}
	else if (par == 3) {
		Rectangle();
		glScalef(scale, scale, 0);
	}
	glFlush();
}
void someting_main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("translate rectangle");
	init();
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMainLoop();
}