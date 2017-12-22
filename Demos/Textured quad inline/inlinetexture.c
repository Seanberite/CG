// Textured quad using an inline texture, a small texture defined in the code

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE:
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "LoadTGA.h"

// Globals
// Data would normally be read from files
GLfloat vertices[] = {	-0.5f,-0.5f,0.0f,
						-0.5f,0.5f,0.0f,
						0.5f,-0.5f,0.0f,
						
						0.5f,-0.5f,0.0f,
						-0.5f,0.5f,0.0f,
						0.5f,0.5f,0.0f };

GLfloat texcoord[] = {	0.0f, 1.0f,
						0.0f, 0.0f,
						1.0f, 1.0f,
						
						1.0f, 1.0f,
						0.0f, 0.0f,
						1.0f, 0.0f};

// My "classic" inline texture (I have been using them for many older demos)
	GLubyte minitex[4][4][3] =
	{
		{ {255, 50,255}, { 50, 50,255}, { 50, 50,255}, { 50,255,255}},
		{ { 50, 50,255}, {255, 50,255}, { 50,255,255}, { 50, 50,255}},
		{ { 50, 50,255}, { 50,255,255}, {255, 50,255}, { 50, 50,255}},
		{ { 50,255,255}, { 50, 50,255}, { 50, 50,255}, {255, 50,255}},
	};

// vertex array object
unsigned int vertexArrayObjID;
GLuint minitexid;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int texCoordBufferObjID;
	// Reference to shader program
	GLuint program;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("inlinetexture.vert", "inlinetexture.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload gemoetry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	// VBO for texCoord data
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

// init minitex
	glGenTextures(1, &minitexid); // texture id
	glBindTexture(GL_TEXTURE_2D, minitexid);
	printError("init arrays 1");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	printError("init arrays 2");
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // eller GL_LINEAR
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // eller GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 
	printError("init arrays 3");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, minitex);

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 inline textured triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}
