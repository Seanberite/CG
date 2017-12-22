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
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include <stdio.h>

// Globals
// Data would normally be read from files
	

#define near 1.0
#define far 50.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = { 2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };

Model *blade;
Model *walls;
Model *roof;
GLuint program;
GLuint myTex;
GLuint myTex2;
GLfloat a = 0.0;
float b = 0;
mat4 camera;


void init(void)
{	
	 
  	walls = LoadModelPlus("windmill-walls.obj");
	blade =  LoadModelPlus("blade.obj");
	roof = LoadModelPlus("windmill-roof.obj");


  LoadTGATextureSimple("maskros512.tga", &myTex);
  LoadTGATextureSimple("dirt.tga", &myTex2);


	// Reference to shader program
	dumpInfo();
	// GL inits
	glClearColor(0.4,0.2,0.0,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, myTex2);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "texUnit2"), 1); // Texture unit 1
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
   
       
	// End of upload of geometry

		
	printError("init arrays");

}

void display(void)
{ 
	

        
 	a += 0.1;
	mat4 rot, trans, trans2, transCam, transCam2, rot2, rotTot, totCam, total, bladeRotMat, windMillPos;

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rot = Ry(0/10);
	transCam = T(0, 0, 16);
	transCam2 = T(0,0,-16);
	totCam = Mult(rot, transCam); 
	totCam = Mult(transCam2, totCam); 

	vec3 p = SetVector(0, 8, 10);
	vec4 s = vec3tovec4(p);
	vec4 d = MultVec4(totCam, s);
	vec3 q = vec4tovec3(d);
	vec3 l = SetVector(0,8,-16);
	vec3 v = SetVector(0,1,0);
	 
	camera = lookAtv(q,l,v);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camera.m);

	// walls
	windMillPos = T(0,0,-16);
	total = Mult(windMillPos, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(walls, program,"in_Position" , "in_Normal", "inTexCoord");

	// Blade rotation matrix
	trans = T(0, -9, 16);
	trans2 = T(0, 9, -16);
	rot = Rz(a/6);
	bladeRotMat = Mult(rot, trans);
	bladeRotMat = Mult(trans2, bladeRotMat);

	// Bladener
	rot = Ry(1.57);
	trans = T(0, 9, -11);
	total = Mult(trans, rot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position" , "in_Normal", "inTexCoord");
	// Bladupp
	rot = Ry(1.57);
	rot2 = Rz(3.14);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position" , "in_Normal", "inTexCoord");
	// BladeV
	rot = Ry(1.57);
	rot2 = Rz(1.57);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position" , "in_Normal", "inTexCoord");
	// BladeH
	rot = Ry(1.57);
	rot2 = Rz(-1.57);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position" , "in_Normal", "inTexCoord");
	
	

	//roof model
	rot  = Ry(0/6);
	trans = T(0,0,-16);
	total = Mult(trans, rot);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof, program,"in_Position" , "in_Normal", "inTexCoord");


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
