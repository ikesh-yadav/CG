#include<GL/glut.h>
void init(void) {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);

}
void lineSegement() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(10, 10);
		glVertex2i(10, 100);
		glVertex2i(100, 100);
		glVertex2i(100, 10);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(10, 100);
		glVertex2i(100, 100);
		glVertex2i(55, 150);
	glEnd();
	glColor3f(0.36, 0.29, 0.15);
	glBegin(GL_POLYGON);
		glVertex2i(45, 10);
		glVertex2i(65, 10);
		glVertex2i(65, 40);
		glVertex2i(45, 40);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(30, 70);
		glVertex2i(40, 70);
		glVertex2i(40, 80);
		glVertex2i(30, 80);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2i(19, 70);
		glVertex2i(29, 70);
		glVertex2i(29, 80);
		glVertex2i(19, 80);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2i(30, 59);
		glVertex2i(40, 59);
		glVertex2i(40, 69);
		glVertex2i(30, 69);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2i(19, 59);
		glVertex2i(29, 59);
		glVertex2i(29, 69);
		glVertex2i(19, 69);
	glEnd();
	glFlush();
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(400, 300);
	glutCreateWindow("My first opengl window");
	init();
	glutDisplayFunc(lineSegement);
	glutMainLoop();
}