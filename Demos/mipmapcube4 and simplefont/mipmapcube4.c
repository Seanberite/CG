// mipmapcube4.c
// Rotating, textured cubes with or without mipmapping
// The "3" reflects that I made two simpler variants first.
// This is the extended demo with interactive selection of texture filtering.
// By Ingemar Ragnemalm 2009
// Version 4 2013 - ported to OpenGL 3.2!

// Linux command-line: gcc -lGL MicroGlut.c mipmapcube4.c simplefont.c VectorUtils3.c GL_utilities.c -o mipmapcube4
// or (with reusable code somewhere else):
// gcc -lGL ../common/*.c mipmapcube4.c simplefont.c -o mipmapcube4 -I ../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include <math.h>
#include "simplefont.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"

float myTime = 0;
GLuint miptex;

mat4 projectionMatrix, worldToViewMatrix;

int leftSelection=0, rightSelection=1;

enum{
	kSingleNearest = 0,
	kSingleLinear,
	kMipmapNearestNearest,
	kMipmapLinearNearest,
	kMipmapNearestLinear,
	kMipmapLinearLinear
};

int texRepeat = 32;

	GLubyte chesstex8[8][8][3] =
	{
		{ {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}},
		{ {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}},
		{ {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}},
		{ {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}},
		{ {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}},
		{ {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}},
		{ {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}},
		{ {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}},
	};

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

//	dumpInfo();

	// GL inits
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	worldToViewMatrix = lookAt(0,0,3, 0,0,0, 0,1,0);
	
	projectionMatrix = frustum(-0.9, 0.9, -0.5, 0.5, 1, 20);
	
	// Load and compile shader
	program = loadShaders("mipmapcube.vert", "mipmapcube.frag");
	
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
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	// End of upload of geometry
	
	// Inline texture:
	
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	
	// Mipmapped inline texture
	glGenTextures(1, &miptex);
	glBindTexture(GL_TEXTURE_2D, miptex); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, chesstex8);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Mipmapping pointless when magnifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// 4 mipmapping variants:
	// GL_NEAREST_MIPMAP_NEAREST closest image, closest texel (poor!)
	// GL_NEAREST_MIPMAP_LINEAR closest texel, linear filtering between images
	// GL_LINEAR_MIPMAP_NEAREST closest image, linear filtering between texels
	// GL_LINEAR_MIPMAP_LINEAR linear filtering between images and texels (best)
	// 2 single-level filtering/magnifying variants:
	// GL_NEAREST closest texel (poor)
	// GL_LINEAR linear filtering (good)	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//sfDrawString(20, -20, "SELECT ZXCVBN");
	//sfDrawString(-20, -20, "SELECT 123456");

	//sglUniform1i(glGetUniformLocation(program, "texRepeat"), texRepeat); // Texture repeat

	switch (leftSelection)
	{
	case kSingleNearest:
		//sfDrawString(-20, 20, "Single texture, nearest");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case kSingleLinear:
		//sfDrawString(-20, 20, "Single texture, linear");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case kMipmapNearestNearest:
		//sfDrawString(-20, 20, "Mipmap, nearest level, nearest texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case kMipmapLinearNearest:
		//sfDrawString(-20, 20, "Mipmap, linear level, nearest texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case kMipmapNearestLinear:
		//sfDrawString(-20, 20, "Mipmap, nearest level, linear texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case kMipmapLinearLinear:
		//sfDrawString(-20, 20, "Mipmap, linear level, linear texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}
	
	mat4 tr, rot, sc, m;
	Point3D axis = {1,1,1};
	
	glUseProgram(program); // Not really needed since simplefont restores the shader.
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	tr = T(1, 0, 0);
	rot = ArbRotate(axis, myTime*M_PI/180);
	m = Mult(worldToViewMatrix, tr);
	m = Mult(m, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
	rot = Ry(myTime*M_PI/180);
	tr = T(1, 0, 0);
	sc = S(0.5, 0.5, 0.5);
	m = Mult(m, rot);
	m = Mult(m, tr);
	m = Mult(m, sc);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
	switch (rightSelection)
	{
	case kSingleNearest:
		//sfDrawString(20, 40, "Single texture, nearest");
		glBindTexture(GL_TEXTURE_2D, miptex); // singletex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case kSingleLinear:
		//sfDrawString(20, 40, "Single texture, linear");
		glBindTexture(GL_TEXTURE_2D, miptex); // singletex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case kMipmapNearestNearest:
		//sfDrawString(20, 40, "Mipmap, nearest level, nearest texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case kMipmapLinearNearest:
		//sfDrawString(20, 40, "Mipmap, linear level, nearest texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case kMipmapNearestLinear:
		//sfDrawString(20, 40, "Mipmap, nearest level, linear texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case kMipmapLinearLinear:
		//sfDrawString(20, 40, "Mipmap, linear level, linear texel");
		glBindTexture(GL_TEXTURE_2D, miptex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}
	
	tr = T(-1, 0, 0);
	rot = ArbRotate(axis, myTime*M_PI/180);
	m = Mult(worldToViewMatrix, tr);
	m = Mult(m, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object

	rot = Ry(myTime*M_PI/180);
	tr = T(1, 0, 0);
	sc = S(0.5, 0.5, 0.5);
	m = Mult(m, rot);
	m = Mult(m, tr);
	m = Mult(m, sc);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	// Viewport is a separate setting
	glViewport(0, 0, w, h);
	//sfSetRasterSize(w, h); // update dimensions for font rendering
}

void timer(int i)
{
	myTime += 1;
	glutTimerFunc(20, timer, i);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
	case 'z': case 'Z':
		rightSelection = kSingleNearest; break;
	case 'x': case 'X':
		rightSelection = kSingleLinear; break;
	case 'c': case 'C':
		rightSelection = kMipmapNearestNearest; break;
	case 'v': case 'V':
		rightSelection = kMipmapLinearNearest; break;
	case 'b': case 'B':
		rightSelection = kMipmapNearestLinear; break;
	case 'n': case 'N':
		rightSelection = kMipmapLinearLinear; break;
	case '1':
		leftSelection = kSingleNearest; break;
	case '2':
		leftSelection = kSingleLinear; break;
	case '3':
		leftSelection = kMipmapNearestNearest; break;
	case '4':
		leftSelection = kMipmapLinearNearest; break;
	case '5':
		leftSelection = kMipmapNearestLinear; break;
	case '6':
		leftSelection = kMipmapLinearLinear; break;
	case '+':
		texRepeat = texRepeat * 2; break;
	case '-':
		if (texRepeat > 1)
			texRepeat = texRepeat / 2;
		break;
   }
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600,400);
	glutInitContextVersion(3, 2);
	glutCreateWindow("mip mapping demo + -");
	glutDisplayFunc(display);
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
		
	init(); // Load cube
	
	//sfMakeRasterFont(); // init font
		
	glutMainLoop();
}
