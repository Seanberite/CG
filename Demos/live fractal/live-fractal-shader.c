// Live fractal shader
// by Ingemar Ragnemalm 2012-2013

// This demo shows how you can implement a Julia fractal in real time
// using a shader, in this case rendered on the surface of the famous
// Utah Teapot (loaded from OBJ file).

// The demo supports minimal controls: space to stop and restart rotation,
// + and - to zoom. I am, however, not very pleased with the zooming.
// A fractal is better zoomed by zooming the scaling of the mapping of
// the complex plane.


#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 translationMatrix, projectionMatrix;

// vertex array object
unsigned int bunnyVertexArrayObjID;
Model *m;
// Reference to shader program
GLuint program;
GLuint tex;

void init(void)
{
	// three vertex buffer objects, used for uploading the data
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	projectionMatrix = frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 50.0);
	translationMatrix = T(0,-6,-20);

	// Load and compile shader
	program = loadShaders("minimal.vert", "fract.frag");
	glUseProgram(program);
	
	// Upload geometry to the GPU:
	m = LoadModelPlus("teapot.obj");

	// End of upload of geometry

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, translationMatrix.m); // A "look-at" function could be more practical
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
// Julia control
	glUniform1f(glGetUniformLocation(program, "juliaRe"), 0.0);
	glUniform1f(glGetUniformLocation(program, "juliaIm"), 0.0);

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
//	LoadTGATextureSimple("maskros512.tga", &tex); // 5c
}

GLfloat juliaRe = 0.0;
GLfloat juliaIm = 0.0;

GLfloat a = 0.0;
GLfloat scale = 2.0;
char dorot = 1;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (dorot) a += 0.1;
	mat4 rot, trans, scalem, total;

	rot = Ry(a/2);
	trans = T(0, 0, -scale/0.8);
	scalem = S(scale, scale, scale);
	total = Mult(Mult(trans, rot), scalem);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

// Julia control
	glUniform1f(glGetUniformLocation(program, "juliaRe"), juliaRe);
	glUniform1f(glGetUniformLocation(program, "juliaIm"), juliaIm);
	
	DrawModel(m, program, "inPosition", NULL, "inTexCoord");

	glutSwapBuffers();
}

void key(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '+':
			scale += 0.1; break;
		case '-':
			scale -= 0.1; break;
		case ' ':
			dorot = !dorot; break;
	}
}

void mouse(int x, int y)
{
	juliaRe = (y-600) / 1200.0;
	juliaIm = (x-400) / 800.0;
	printf("juliaRe = %f juliaIm = %f \n", juliaRe, juliaIm);
}

static void timer(int arg)
{
	glutTimerFunc(20, timer, 0);
	glutPostRedisplay();
}

int main(int argc, const char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize (600, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 live Julia");
	glutDisplayFunc(display);
//	glutRepeatingTimerFunc(20); // I prefer this but it only exists in MicroGlut
	glutTimerFunc(20, timer, 0);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(mouse);
	init ();
	glutMainLoop();
	exit(0);
}
