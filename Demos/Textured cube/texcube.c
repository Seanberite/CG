// Textured cube
// By Ingemar Ragnemalm 2013
// This cube is mainly for certain kind of demos, like mipmapping,
// where you need rather dull textures but with perfect control over
// them. So how to make the cube isn't the most interesting part - you
// usually don't want separate vertices like that - but rather its
// qualities in illustrating certain features.

// MS Windows needs GLEW or glee.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <math.h>

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

// The rotation matrices will be changed for animation
GLfloat rotationMatrix[] = {	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
GLfloat rotationMatrix2[] = {	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
GLfloat translationMatrix[] = {	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, -2.0f,
						0.0f, 0.0f, 0.0f, 1.0f };

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
	
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

	// End of upload of geometry

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex);

	printError("init arrays");
}

float a = 0.0;

void display(void)
{
	printError("pre display");
	a += 0.1;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rotationMatrix2[0] = cos(a/5.0);
	rotationMatrix2[1] = -sin(a/5.0);
	rotationMatrix2[4] = sin(a/5.0);
	rotationMatrix2[5] = cos(a/5.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix2"), 1, GL_TRUE, rotationMatrix2);
	rotationMatrix[5] = cos(a);
	rotationMatrix[6] = -sin(a);
	rotationMatrix[9] = sin(a);
	rotationMatrix[10] = cos(a);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

void timer(int arg)
{
	glutTimerFunc(20, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 textured cube example");
	glutDisplayFunc(display); 
	glutTimerFunc(20, timer, 0);
	init ();
	glutMainLoop();
}
