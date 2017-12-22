// Gouraud shaded cube
// gcc gouraudcube.c ../common/*.c -lGL -o gouraudcube -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"

// Globals
// Data would normally be read from files
GLfloat vertices[8][3] = {{-0.5,-0.5,-0.5},
									{0.5,-0.5,-0.5},
									{0.5,0.5,-0.5},
									{-0.5,0.5,-0.5},
									{-0.5,-0.5,0.5},
									{0.5,-0.5,0.5},
									{0.5,0.5,0.5},
									{-0.5,0.5,0.5}};
GLfloat normals[8][3] = {{-0.58,-0.58,-0.58},
									{0.58,-0.58,-0.58},
									{0.58,0.58,-0.58},
									{-0.58,0.58,-0.58},
									{-0.58,-0.58,0.58},
									{0.58,-0.58,0.58},
									{0.58,0.58,0.58},
									{-0.58,0.58,0.58}};
GLubyte cubeIndices[36] = {0,3,2, 0,2,1, 
									2,3,7, 2,7,6,
									0,4,7, 0,7,3,
									1,2,6, 1,6,5,
									4,5,6, 4,6,7,
									0,1,5, 0,5,4};

// NEW
GLfloat rotationMatrix[] = {	0.7f, -0.7f, 0.0f, 0.0f,
						0.7f, 0.7f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };


// vertex array object
unsigned int vertexArrayObjID;
	// Reference to shader program
	GLuint program;

void init(void)
{
	// three vertex buffer objects, used for uploading the data
	unsigned int vertexBufferObjID, indexBufferObjID, normalBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("gouraud.vert", "gouraud.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &indexBufferObjID);
	glGenBuffers(1, &normalBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endifID);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
	printError("init vertices");

	// VBO for normal/color data
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
	printError("init normals");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(GLubyte), cubeIndices, GL_STATIC_DRAW);
	printError("init index");

	// End of upload of geometry

// NEW
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, rotationMatrix);
	
	printError("init arrays");
}

#define Pi 3.1415
GLfloat a = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 m1, m2, m;
	
	a += 0.1;
	m1 = Rz(Pi/5);
	m2 = Ry(a);
	m = Mult(m2,m1);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, m.m);
	
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 cube example");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}
