// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>

	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif

#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"

// Globals
// Data would normally be read from files

Model *bunnyModel;

// vertex array object
unsigned int bunnyVertexArrayObjID;

GLuint program;
void init(void)
{
  
  bunnyModel = LoadModel("bunnyplus.obj");
	// vertex buffer object, used for uploading the geometr
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;
	

	// Reference to shader program


	dumpInfo();
	// GL inits
	glClearColor(0.0,0.2,0.0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glBindVertexArray(bunnyVertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, bunnyModel->numVertices*3*sizeof(GLfloat), bunnyModel->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	
	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, bunnyModel->numVertices*3*sizeof(GLfloat), bunnyModel->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	// VBO for texture coordinates data
	if (bunnyModel->texCoordArray != NULL)
	    {
		glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, bunnyModel->numVertices*2*sizeof(GLfloat), bunnyModel->texCoordArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	    }

	//For indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bunnyModel->numIndices*sizeof(GLuint), bunnyModel->indexArray, GL_STATIC_DRAW);
	// End of upload of geometry

		
	printError("init arrays");
}

void display(void)
{
	printError("pre display");

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000.0;

  	GLfloat myMatrix[] = { 1.0, 0.0f, 0.0, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0, 0.0f, 1.0, 1.5f,
                        0.0f, 0.0f, 0.0f, 1.0f };

  	GLfloat myMatrix2[] = { cos(t), sin(t), 0.0f, 0.0f,
			-sin(t), cos(t), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };
	// Upload transformation matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix2"), 1, GL_TRUE, myMatrix2);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(bunnyVertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, bunnyModel->numIndices, GL_UNSIGNED_INT, 0L);

	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle examplee");
	init ();
	glutDisplayFunc(display);
	glutRepeatingTimer(20);
	glutMainLoop();
	return 0;
}
