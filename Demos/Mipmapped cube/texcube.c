// Textured cube
// By Ingemar Ragnemalm 2013
// This cube is mainly for certain kind of demos, like mipmapping,
// where you need rather dull textures but with perfect control over
// them. So how to make the cube isn't the most interesting part - you
// usually don't want separate vertices like that - but rather its
// qualities in illustrating certain features.

// Somewhat preliminary - I plan to replace the flower with a texture with more aliasing.
// (Or you just try that yourself.)

// Linux compilation line:
// gcc texcube.c ../common/*.c -lGL -o texcube -I../common

// MS Windows needs GLEW or glee.
#include "GL_utilities.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"

// Globals
// Data would normally be read from files
GLfloat vertices[36*3] = {
						-0.5,-0.5,-0.5,	// 0
						-0.5,0.5,-0.5,		// 3
						0.5,0.5,-0.5,		// 2
						-0.5,-0.5,-0.5,	// 0
						0.5,0.5,-0.5,		// 2
						0.5,-0.5,-0.5,		// 1

						0.5,0.5,-0.5,		// 2
						-0.5,0.5,-0.5,		// 3
						-0.5,0.5,0.5,		// 7
						0.5,0.5,-0.5,		// 2
						-0.5,0.5,0.5,		// 7
						0.5,0.5,0.5,		// 6

						-0.5,-0.5,-0.5,	// 0
						-0.5,-0.5,0.5,		// 4
						-0.5,0.5,0.5,		// 7
						-0.5,-0.5,-0.5,	// 0
						-0.5,0.5,0.5,		// 7
						-0.5,0.5,-0.5,		// 3

						0.5,-0.5,-0.5,		// 1
						0.5,0.5,-0.5,		// 2
						0.5,0.5,0.5,		// 6
						0.5,-0.5,-0.5,		// 1
						0.5,0.5,0.5,		// 6
						0.5,-0.5,0.5,		// 5

						-0.5,-0.5,0.5,		// 4
						0.5,-0.5,0.5,		// 5
						0.5,0.5,0.5,		// 6
						-0.5,-0.5,0.5,		// 4
						0.5,0.5,0.5,		// 6
						-0.5,0.5,0.5,		// 7

						-0.5,-0.5,-0.5,	// 0
						0.5,-0.5,-0.5,		// 1
						0.5,-0.5,0.5,		// 5
						-0.5,-0.5,-0.5,	// 0
						0.5,-0.5,0.5,		// 5
						-0.5,-0.5,0.5		// 4						
						};

GLfloat texcoord[36*2] = {
						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,

						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,

						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,

						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,

						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,

						1.0, 0.0,
						0.0, 0.0,
						0.0, 1.0,
						1.0, 0.0,
						0.0, 1.0,
						1.0, 1.0,
					};

// Perspective
#define near 1.0
#define far 30.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
											0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
											0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
											0.0f, 0.0f, -1.0f, 0.0f };

// vertex array object
unsigned int vertexArrayObjID;
// Reference to shader program
GLuint program;
// Reference to texture
GLuint tex;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int texcoordBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("texcube.vert", "texcube.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &texcoordBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	// VBO for texcoord data
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*2*sizeof(GLfloat), texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Texcoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Texcoord"));
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

	// End of upload of geometry

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex);

	printError("init arrays");
}

float a = 0.0;
char animatee = 1;

void display(void)
{
	printError("pre display");
	if (animatee)
		a += 0.1;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 m1, m2, m, ms, tr;
	
	m1 = Rz(M_PI/5);
	m2 = Ry(a);
	ms = S(sin(a/2.0)/4.0+0.75, sin(a/2.0)/4.0+0.75, sin(a/2.0)/4.0+0.75);
	m = Mult(m1,m2);
	m = Mult(m,ms);
	tr = T(0, 0, -2);
	m = Mult(tr, m);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

void timer(int arg)
{
	glutTimerFunc(20, timer, 0);
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
	if (key == '+')
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filtered
	if (key == '-')
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered, no mipmap
	if (key == ' ')
		animatee = !animatee;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 textured cube example +-");
	glutDisplayFunc(display); 
	glutTimerFunc(20, timer, 0);
	glutKeyboardFunc(key);
	init ();
	glutMainLoop();
	exit(0);
}

