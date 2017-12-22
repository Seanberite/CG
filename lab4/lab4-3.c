// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	vec3 u, v;
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 5.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 5.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		// beräkna normalvectorer
			if(x>=1 && z>= 1){
				u.x =vertexArray[(x+1 + z * tex->width)*3+0] - vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				u.y = vertexArray[(x+1 + z * tex->width)*3+1] - vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				u.z =vertexArray[(x+1 + z * tex->width)*3+2] - vertexArray[(x-1 + (z-1) * tex->width)*3+2];
	
				v.x= vertexArray[(x + (z+1) * tex->width)*3+0] - vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				v.y=vertexArray[(x + (z+1) * tex->width)*3+1] - vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				v.z=vertexArray[(x + (z+1) * tex->width)*3+2] - vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				vec3 normal= CrossProduct(v,u);

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1]= normal.y;
				normalArray[(x + z * tex->width)*3+ 2]= normal.z;
			}
		}

	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm, *sphere;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
vec3 l, p, v;
mat4 trans, trans2, rot, camera, rotateUpDown,  total, modelView, camMatrix, modelToWorld;

void init(void)
{

	p = SetVector(20, 20, 0);
	l = SetVector(0,0,-1);
	v = SetVector(0,1,0);

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain3.vert", "terrain3.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
// Load objects
sphere = LoadModelPlus("groundsphere.obj");

}

void display(void)
{

	vec3 lookingDir = Normalize(VectorSub(l, p));
	vec3 walkSideways = Normalize(CrossProduct(lookingDir, v));
	
	
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


	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	camMatrix = lookAtv(p,l,v);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

//Draw sphere
	modelToWorld = T(10, 10, 10);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelToWorld.m);
	DrawModel(sphere, program, "inPosition", "inNormal", "inTexCoord");


	printError("display 2");
	
	glutSwapBuffers();
}


void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
