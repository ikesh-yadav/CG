
#include<GL/glut.h>

#define windowWidth 600
#define windowHeight 300


int downx, downy,rectx,recty;

void init(void) {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}
void drawPoint(int x, int y) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POINTS);
		glVertex2i(downx, downy);
	glEnd();
}
void draw() {
	//glClear(GL_COLOR_BUFFER_BIT);
	drawPoint(downx, downy);
	glFlush();
}
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		downx = x;
		downy = windowHeight-y;
		
	} else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		rectx = x;
		recty = y;
	}
	glutPostRedisplay();
}
int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(" Mouse interactions ");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	init();
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}


