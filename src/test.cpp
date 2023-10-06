#include <iostream>
#include <windows.h>
//#include <GL/glut.h>
#include <GL/freeglut.h>


using namespace std;
int learn_gl(int argc, char** argv) {
	std::cout << "hello";	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(500,500);

	glutCreateWindow("window 1");




	return 0;
}