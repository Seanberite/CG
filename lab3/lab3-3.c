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
#define far 500.0
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
Model *balcony;
Model *skybox;
Model *bunnyPlusModel;
// FOR GROUND-------------------------------------
GLfloat vertices[3*3] =
{
	-500.0f,0.0f,500.0f,
	-500.0f,.0f,-500.0f,
	500.0f,0.0f,500.0f
};
GLfloat normals[3*3] =
{
	0.0f,1.0f,0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f,1.0f,0.0f
};

GLfloat texcoord[3*2] =
{
	
		10.0, 0.0,
		0.0, 0.0,
		0.0, 10.0,
	};


// vertex array object
unsigned int groundVertexArrayObjID;
unsigned int groundVertexBufferObjID; 
unsigned int groundNormalBufferObjID;
unsigned int texCoordBufferObjID;

// FOR GROUND---------------------------



GLuint programSkyBox;
GLuint program;

GLuint myTex1;
GLuint myTex2;
GLuint myTex3;
GLfloat a = 0.0;
float x = 0;
float z = 0;

mat4 camera, rotateUpDown, cameraSkyBox, rot, rot2, trans, trans2, rotTot, total, bladeRotMat;

vec3 p; 
vec3 l;
vec3 v;

void init(void)
{	
	
	glGenVertexArrays(1, &groundVertexArrayObjID);
	
	// Initialize vectors for cameramatrix
	p = SetVector(0, 5, 0);
	l = SetVector(0,5,-1);
	v = SetVector(0,1,0);
	
	//------------------Stuff for skybox---------------------
	
	// Load and compile Skybox shaders
	programSkyBox = loadShaders("skybox.vert", "skybox.frag");
	printError("init shader");
		
	// Load skybox object and skybox texture
	skybox = LoadModelPlus("skybox.obj");
	LoadTGATextureSimple("SkyBox512.tga", &myTex3);
	
	// Upload the projectionmatrix to the skybox shader
	glUseProgram(programSkyBox);
	glUniformMatrix4fv(glGetUniformLocation(programSkyBox, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

	//------------------Stuff for the rest of the world---------------------------

	// Load and compile shader
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	printError("init shader");
	
	 // Load models
  	walls = LoadModelPlus("windmill-walls.obj");
	blade =  LoadModelPlus("blade.obj");
	roof = LoadModelPlus("windmill-roof.obj");
	balcony = LoadModelPlus("windmill-balcony.obj");
	bunnyPlusModel = LoadModelPlus("bunnyplus.obj");

	// Load textures
  	LoadTGATextureSimple("maskros512.tga", &myTex1);
  	LoadTGATextureSimple("dirt.tga", &myTex2);


	// Reference to shader program
	dumpInfo();
	// GL inits
	glClearColor(0.4,0.2,0.0,0);
		
	printError("GL inits");
	
	// Upload projection matrix to main program shader
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
       
	// End of upload of geometry

		
	printError("init arrays");

}

void display(void)
{ 
	// Calsculate the parameters needed for the camera movement
	vec3 lookingDir = Normalize(VectorSub(l, p));
	vec3 walkSideways = Normalize(CrossProduct(lookingDir, v));
	
	// Key functions
	if (glutKeyIsDown('a')) { 
	 p = VectorSub(p, walkSideways);
	 l = VectorSub(l, walkSideways);
	}
	
	if (glutKeyIsDown('d')) { 
	  p = VectorAdd(p, walkSideways);
	 l = VectorAdd(l, walkSideways);
	}

	if (glutKeyIsDown('w')) { 		
		p = VectorAdd(p, lookingDir);
		l = VectorAdd(l, lookingDir);
		} 
		
	if (glutKeyIsDown('s')) { 
		p = VectorSub(p, lookingDir);
		l = VectorSub(l, lookingDir);
	 }
		
	if (glutKeyIsDown('l')) { 
		trans = T(-p.x, -p.y, -p.z);
		trans2 = T(p.x, p.y, p.z);
		rot = Ry(-0.05);
		l = MultVec3(trans2, MultVec3(rot, MultVec3(trans, l)));
	}
	
	if (glutKeyIsDown('j')) { 
		trans = T(-p.x, -p.y, -p.z);
		trans2 = T(p.x, p.y, p.z);
		rot = Ry(0.05);
		l = MultVec3(trans2, MultVec3(rot, MultVec3(trans, l)));
	}
	
	if (glutKeyIsDown('i')) { 
	 trans = T(-p.x, -p.y, -p.z);
		trans2 = T(p.x, p.y, p.z);
		rotateUpDown = ArbRotate(walkSideways, 0.05);
		l = MultVec3(trans2, MultVec3(rotateUpDown, MultVec3(trans, l)));
	}
	if (glutKeyIsDown('k')) { 
	 trans = T(-p.x, -p.y, -p.z);
		trans2 = T(p.x, p.y, p.z);
		rotateUpDown = ArbRotate(walkSideways, -0.05);
		l = MultVec3(trans2, MultVec3(rotateUpDown, MultVec3(trans, l)));
	}


	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate worl-to-view matrix
	camera = lookAtv(p,l,v);
	cameraSkyBox = mat3tomat4(mat4tomat3(camera));
	
	// ----------------------Do thing with skybox-----------------------------------
	
	glUseProgram(programSkyBox);
	
	// Upload camMatrix to skyboxShader
	glUniformMatrix4fv(glGetUniformLocation(programSkyBox, "camSkyBoxMatrix"), 1, GL_TRUE, cameraSkyBox.m);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTex3);
	
	glDisable(GL_DEPTH_TEST);	
	glUniform1i(glGetUniformLocation(programSkyBox, "texUnit0"), 0); // Texture unit 0
	DrawModel(skybox, programSkyBox,"in_Position" , NULL, "inTexCoord");	
	glEnable(GL_DEPTH_TEST);
	
	// ----------------------Do thing with the resat of program Draw rest---------------------------
	
	// Upload camMatrix to Shader
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camera.m);	
	
	// Calculate Blade rotation matrix
	trans = T(0, -9, 16);
	trans2 = T(0, 9, -16);
 	a += 0.1;
	rot = Rz(a/6);
	bladeRotMat = Mult(rot, trans);
	bladeRotMat = Mult(trans2, bladeRotMat);
	
	// Draw walls
	rot = Ry(-1.57);
	trans = T(0,0,-16);
	total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTex1);
	glUniform1i(glGetUniformLocation(program, "texUnit1"), 0); // Texture unit 2
	DrawModel(walls, program, "in_Position" , "in_Normal", "inTexCoord");

	// Draw Bladener
	rot = Ry(1.57);
	trans = T(0, 9, -11);
	total = Mult(trans, rot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program, "in_Position" , "in_Normal", "inTexCoord");
	// Draw Bladupp
	rot = Ry(1.57);
	rot2 = Rz(3.14);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position", "in_Normal", "inTexCoord");
	// Draw BladeV
	rot = Ry(1.57);
	rot2 = Rz(1.57);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position", "in_Normal", "inTexCoord");
	// Draw BladeH
	rot = Ry(1.57);
	rot2 = Rz(-1.57);
	rotTot = Mult(rot2, rot);
	trans = T(0, 9, -11);
	total = Mult(trans, rotTot);
	total = Mult(bladeRotMat, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(blade, program,"in_Position", "in_Normal", "inTexCoord");
	

	// Draw roof model
	trans = T(0,0,-16);
	total = trans;

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof, program,"in_Position", "in_Normal", "inTexCoord");
	
	// Draw Balcony
	trans = T(0,0,-16);
	rot = Ry(-1.57);
	total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(balcony, program,"in_Position", "in_Normal", "inTexCoord");
	
	// Draw Bunny
	trans = T(0,16,-16);
	rot = Ry(-1.57);
	total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(bunnyPlusModel, program,"in_Position", "in_Normal", "inTexCoord");
	
	
	// -----Draw ground triangle--------------------------------------
	
	// Allocate and activate Vertex Array Object
	glBindVertexArray(groundVertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &groundVertexBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
	glGenBuffers(1, &groundNormalBufferObjID);
	
	// VAO
	glBindBuffer(GL_ARRAY_BUFFER, groundVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, groundNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
	
	// VBO for tex data
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(GLfloat), texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	
	total = IdentityMatrix();
        
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindVertexArray(groundVertexArrayObjID);	// Select VAO
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTex2);
	glUniform1i(glGetUniformLocation(program, "texUnit1"), 0); // Texture unit 0

	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	rot = Ry(3.14);
	total = rot;
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindVertexArray(groundVertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	// ---- END Draw ground triangle -----------------------------
      

	printError("display");
	glutSwapBuffers();
	
}

	
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Whoot??? A windmill !!!!!!!!");
	init();
	glutDisplayFunc(display);
	glutRepeatingTimer(20);
	glutMainLoop();
	return 0;
}
