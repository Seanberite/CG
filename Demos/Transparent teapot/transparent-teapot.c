/* transparent teapot demo by Ingemar Ragnemalm*/
// OpenGL 3.2 version

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/glut.h>
#endif
#include <math.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "GL_utilities.h"

GLfloat angle = 0.0;

// For file management
#include <stdio.h>
#include <stdlib.h>

int setting = 1;
char linearInterpolation = 0;

mat4 lookAtMatrix, projMatrix;
Model *teapotModel;
GLuint program;

void init()
{
	program = loadShaders("tex.vert", "tex.frag");

	teapotModel = LoadModelPlus("teapotmini.obj");

	projMatrix = perspective(/* field of view in degree */ 70.0,
		/* aspect ratio */ 1.0, /* Z near */ 0.2, /* Z far */ 100.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &projMatrix.m);
	
	lookAtMatrix = lookAt(0,0,30, 0,0,0, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, &lookAtMatrix.m);

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0	
}

void display()
{
	mat4 m;
	GLfloat s = sin(angle);
	GLfloat c = cos(angle);
	vec3 axis = {1,1,1};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m = ArbRotate(axis, angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, &m.m);
	
// 3 cases: no culling, culling and draw fronts, culling and draw first backs, then fronts
	
	glEnable(GL_DEPTH_TEST); // All but 4 uses this
	glEnable(GL_CULL_FACE);

	switch (setting)
	{
	case 1: // No cull, no Z. Half-decent but fronts get overwritten
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		DrawModel(teapotModel, program, "inPosition", "inNormal", "inTexCoord");
		break;
	case 2: // Z-buffer, no cull. Pretty bad.
		glDisable(GL_CULL_FACE);
		DrawModel(teapotModel, program, "inPosition", "inNormal", "inTexCoord");
		break;
	case 3: // Z-buffer, cull back. Bad!
		glCullFace(GL_BACK);
		DrawModel(teapotModel, program, "inPosition", "inNormal", "inTexCoord");
		break;
	case 4: // Cull front, draw backs, cull backs, draw fronts
		glCullFace(GL_FRONT);
		DrawModel(teapotModel, program, "inPosition", "inNormal", "inTexCoord");
		glCullFace(GL_BACK);
		DrawModel(teapotModel, program, "inPosition", "inNormal", "inTexCoord");
		break;
	}
	
	glutSwapBuffers();
}

void timer(int i)
{
	angle += 0.03; // 1.0;
	glutTimerFunc(20, timer, i);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
	case '1':
		setting = 1; break;
	case '2':
		setting = 2; break;
	case '3':
		setting = 3; break;
	case '4':
		setting = 4; break;
	case 'i':
		linearInterpolation = !linearInterpolation;
		if (linearInterpolation)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
}

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };
	GLfloat light_position[] = { 1.0, 1.0, 2.0, 0.0 }; // ljuset g�r fr�n det h�llet?

// A simple texture
	GLubyte minitex4[4][4][3] =
	{
		{ {255,  0,255}, {  0,  0,255}, {  0,  0,255}, {  0,255,255}},
		{ {  0,  0,255}, {255,  0,255}, {  0,255,255}, {  0,  0,255}},
		{ {  0,  0,255}, {  0,255,255}, {255,  0,255}, {  0,  0,255}},
		{ {  0,255,255}, {  0,  0,255}, {  0,  0,255}, {255,  0,255}},
	};

// Texture with transparency
	GLubyte minitexrgba[4][4][4] =
	{
		{ {255,  0,255, 255}, {  0,  0,255, 128}, {  0,  0,255, 128}, {  0,255,255, 255}},
		{ {  0,  0,255, 128}, {255,  0,255, 0}, {  0,255,255, 0}, {  0,  0,255, 128}},
		{ {  0,  0,255, 128}, {  0,255,255, 0}, {255,  0,255, 0}, {  0,  0,255, 128}},
		{ {  0,255,255, 255}, {  0,  0,255, 128}, {  0,  0,255, 128}, {255,  0,255, 255}},
	};
	

GLuint tex;
	

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Transparent teapot 1234i");
	
	// Set the background color
//	glClearColor(0.9, 0.9, 0.2, 0);
	glClearColor(1.0, 1.0, 1.0, 0);
	
	// Various initializations, enable OpenGL features
	glEnable(GL_CULL_FACE);
	
	init();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Install textures	
	glGenTextures(1, &tex);			// Generate OpenGL texture IDs
	glActiveTexture(GL_TEXTURE0); // Just make sure the texture unit match
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, minitexrgba);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	
	// Install GLUT callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(20, timer, 0);
	glutKeyboardFunc(keyboard);
	
	// Run!
	glutMainLoop();
	return 0;
}
