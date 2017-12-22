#include "MicroGlut.h"
#include "GL_utilities.h"

int main()
{
	glutInit();
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle examplee");
	glutDisplayFunc(display);
	init ();
	glutMainLoop();
	return 0;
}
