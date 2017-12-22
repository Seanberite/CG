// Phong shaded cube with fake bump map.
// It is faked since the tangent and cotangent vectors are
// calculated in a most sloppy way - which works when the
// bump map is a plain noise function.

// The normals "try to make the cube round".
// There are also "normals2" which are for a "real cube",
// but I find it less suited for illustrating bump mapping.

// MS Windows needs GLEW or glee.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"

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

GLfloat normals[36*3] = {
						-0.58,-0.58,-0.58,	// 0
						-0.58,0.58,-0.58,		// 3
						0.58,0.58,-0.58,		// 2
						-0.58,-0.58,-0.58,	// 0
						0.58,0.58,-0.58,		// 2
						0.58,-0.58,-0.58,		// 1

						0.58,0.58,-0.58,		// 2
						-0.58,0.58,-0.58,		// 3
						-0.58,0.58,0.58,		// 7
						0.58,0.58,-0.58,		// 2
						-0.58,0.58,0.58,		// 7
						0.58,0.58,0.58,		// 6

						-0.58,-0.58,-0.58,	// 0
						-0.58,-0.58,0.58,		// 4
						-0.58,0.58,0.58,		// 7
						-0.58,-0.58,-0.58,	// 0
						-0.58,0.58,0.58,		// 7
						-0.58,0.58,-0.58,		// 3

						0.58,-0.58,-0.58,		// 1
						0.58,0.58,-0.58,		// 2
						0.58,0.58,0.58,		// 6
						0.58,-0.58,-0.58,		// 1
						0.58,0.58,0.58,		// 6
						0.58,-0.58,0.58,		// 5

						-0.58,-0.58,0.58,		// 4
						0.58,-0.58,0.58,		// 5
						0.58,0.58,0.58,		// 6
						-0.58,-0.58,0.58,		// 4
						0.58,0.58,0.58,		// 6
						-0.58,0.58,0.58,		// 7

						-0.58,-0.58,-0.58,	// 0
						0.58,-0.58,-0.58,		// 1
						0.58,-0.58,0.58,		// 5
						-0.58,-0.58,-0.58,	// 0
						0.58,-0.58,0.58,		// 5
						-0.58,-0.58,0.5		// 4						
						};

GLfloat normals2[36*3] = {
						0,0,-1,
						0,0,-1,
						0,0,-1,
						0,0,-1,
						0,0,-1,
						0,0,-1,

						0,1,0,
						0,1,0,
						0,1,0,
						0,1,0,
						0,1,0,
						0,1,0,
						
						-1, 0, 0,
						-1, 0, 0,
						-1, 0, 0,
						-1, 0, 0,
						-1, 0, 0,
						-1, 0, 0,

						1, 0, 0,
						1, 0, 0,
						1, 0, 0,
						1, 0, 0,
						1, 0, 0,
						1, 0, 0,

						0, 0, 1,
						0, 0, 1,
						0, 0, 1,
						0, 0, 1,
						0, 0, 1,
						0, 0, 1,

						0, 0, -1,
						0, 0, -1,
						0, 0, -1,
						0, 0, -1,
						0, 0, -1,
						0, 0, -1
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

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
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
	// three vertex buffer objects, used for uploading the data
	unsigned int vertexBufferObjID, texcoordBufferObjID, normalBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("fakebump.vert", "fakebump.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &texcoordBufferObjID);
	glGenBuffers(1, &normalBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
	printError("init vertices");

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), normals2, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
	printError("init normals");

	// VBO for texcoord data
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*2*sizeof(GLfloat), texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Texcoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Texcoord"));

	// End of upload of geometry

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
//	LoadTGATextureSimple("maskros512.tga", &tex);
//	LoadTGATextureSimple("spots.tga", &tex);
	LoadTGATextureSimple("conc2.tga", &tex);
//	LoadTGATextureSimple("lines.tga", &tex);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	printError("init arrays");
}

GLfloat a = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Matrix4D m1, m2, m, tr;
	
	a += 0.01;
	m1 = Rz(M_PI/5);
	m2 = Ry(a);
	m = Mult(m2,m1);
	tr = T(0, 0, -2);
	m = Mult(tr, m); // tr * ry * rz
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3,2);
	glutCreateWindow ("GL3 faked bump map example");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}
