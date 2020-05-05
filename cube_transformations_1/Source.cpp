#include<GL/glut.h>
#include<math.h>


#define windowWidth 600
#define windowHeight 300

GLfloat vertices[][3] = {
	{ -1.0, -1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{ 1.0,  1.0, -1.0 },
	{ -1.0,  1.0, -1.0 },
	{ -1.0, -1.0,  1.0 },
	{ 1.0, -1.0,  1.0 },
	{ 1.0,  1.0,  1.0 },
	{ -1.0,  1.0,  1.0 } };

GLfloat normals[][3] = {
	{ -1.0, -1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{ 1.0,  1.0, -1.0 },
	{ -1.0,  1.0, -1.0 },
	{ -1.0, -1.0,  1.0 },
	{ 1.0, -1.0,  1.0 },
	{ 1.0,  1.0,  1.0 },
	{ -1.0,  1.0,  1.0 } };

GLfloat colors[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 } };

static GLint axis = 2;
static GLfloat theta[3] = { 0.0, 0.0, 0.0 };

int par = 0;
int scale[3] = {2,1,2};

int cube_side = 10;

void init(void) {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}
void mouse(int button, int state, int mousex, int mousey){

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		par = 1;glutPostRedisplay();
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		par = 2;glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		par = 3;glutPostRedisplay();
	}
	else {
		par = 0;
	}

}
 
void face(int x1, int x2, int x3, int x4) {
	glColor3f(0.3, 0.85, 1.0);
	glBegin(GL_POLYGON);
		glVertex3fv(vertices[x1]);
		glVertex3fv(vertices[x2]);
		glVertex3fv(vertices[x3]);
		glVertex3fv(vertices[x4]);
	glEnd();
}
void cube() {
	face(0, 3, 2, 1);
	face(2, 3, 7, 6);
	face(0, 4, 7, 3);
	face(1, 2, 6, 5);
	face(4, 5, 6, 7);
	face(0, 1, 5, 4);
}
void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.9, 1, 1, 0.8);
	cube();
	//glScalef(scale[0], scale[1], scale[2]);
	glFlush();
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("cube transformation");
	init();
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMainLoop();
}