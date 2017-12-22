
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

mat4 projectionMatrix;

Point3D objectcolor = {1, 1, 0};//yellow

// Array of balls (world coords)
#define kNumBalls 10
Point3D pos[kNumBalls];
Point3D speed[kNumBalls];
Point3D grav = {0, -0.1, 0};


mat4 projectionMatrix;
float mapScaling = 4.0;

// vertex array object
Model *m, *m2, *tm, *sphere, *skybox, *smallballs;
// Reference to shader program
GLuint firefly, programt, programSkyBox;
GLuint grasstex, tex2, skyTex, venus;
TextureData ttex; // terrain

vec3 l, p, v, walkSideways, lookingDir;
float ballXPos, ballYPos, ballZPos, dx, dz, dyx, dyz, deltaX, deltaY, previousHeight, currentHeight, heightDiff;
mat4 trans, trans2, rot, camera, rotateUpDown, modelView, camMatrix, modelToWorld, cameraSkyBox, worldmatrix1, worldmatrix2;
int texWidth;
int windowWidth, windowHeight, xMouseOrigin, yMouseOrigin;

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
			vertexArray[(x + z * tex->width)*3 + 0] = x / mapScaling;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 80.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / mapScaling;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width*10;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height*10;
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
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
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
float calcHeight(float x, float z){
	
	int d = floor(x * mapScaling);
	int e = floor(z * mapScaling);
	float y;
	dx = x * mapScaling - d;
	dz = z * mapScaling - e; 
	
	
	if(dx+dz<1){
		dyx = tm->vertexArray[(d+1 + e * texWidth)*3+1] - tm->vertexArray[(d + e * texWidth)*3+1];	
		dyz = tm->vertexArray[(d + (e+1) * texWidth)*3+1] - tm->vertexArray[(d + e * texWidth)*3+1];
		y = dyx*dx+dyz*dz + tm->vertexArray[(d + e * texWidth)*3+1];
		
	} 
	else{
		dyx = tm->vertexArray[(d+1 + e * texWidth)*3+1] - tm->vertexArray[((d+1) + (e+1) * texWidth)*3+1];	
		dyz = tm->vertexArray[(d + (e+1) * texWidth)*3+1] - tm->vertexArray[((d+1) + (e+1) * texWidth)*3+1];
		dx = (ceil(x * mapScaling)-x * mapScaling);
		dz = (ceil(z * mapScaling)-z * mapScaling);
		y = dyx*dx+dyz*dz + tm->vertexArray[((d+1) + (e+1) * texWidth)*3+1];
		}

	return y;
}

void calcCamPos(){
	
	lookingDir = ScalarMult(Normalize(VectorSub(l, p)),0.1);
	walkSideways = ScalarMult(Normalize(CrossProduct(lookingDir, v)), 0.1);
	walkSideways.y = 0;
	lookingDir.y = 0;
	
	
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
	currentHeight = calcHeight(p.x, p.z);
	heightDiff = currentHeight - previousHeight;
	l.y = l.y + heightDiff;
	p.y = p.y + heightDiff;
	previousHeight = currentHeight;
	
	if (p.x > texWidth/mapScaling*2/3){
		p.x = p.x -texWidth/mapScaling/3;
		l.x = l.x -texWidth/mapScaling/3;
		}
	if (p.z > texWidth/mapScaling*2/3){
		p.z = p.z - texWidth/mapScaling/3;
		l.z = l.z - texWidth/mapScaling/3;
		}
	if (p.x < texWidth/mapScaling/3){
		p.x = p.x + texWidth/mapScaling/3;
		l.x = l.x + texWidth/mapScaling/3;
		}
	if (p.z < texWidth/mapScaling/3){
		p.z = p.z + texWidth/mapScaling/3;
		l.z = l.z + texWidth/mapScaling/3;
		}

}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 40);
	// ---------- For Terrain, Load and compile shader ----------------
	programt = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(programt);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(programt, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Load terrain data
	LoadTGATextureData("fft-terrainBig.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	TextureData *texturePointer = &ttex;
	texWidth = texturePointer->width;
	printError("init terrain");
	
	// Load and compile shader
	firefly = loadShaders("firefly.vert", "firefly.frag");
	glUseProgram(firefly);
	printError("init shader");
	
	// Upload geometry to the GPU:
	smallballs= LoadModelPlus("sphere20.obj");

	//projectionMatrix = frustum(-0.6, 0.6, -0.6, 0.6, 1, 30);
	
	glUniformMatrix4fv(glGetUniformLocation(firefly, "projectionMatrix"), 1, GL_TRUE, &projectionMatrix.m);

	worldmatrix1 = IdentityMatrix();
	// Load and init textures
	LoadTGATextureSimple("Textures/grassTexture.tga", &grasstex);
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glUniform1i(glGetUniformLocation(programt, "grassTexture"), 0);
	glUniform1i(glGetUniformLocation(programt, "texUnit1"), 1); 
	///
	
	// update ball pos and init stuff dependent on above loading
	windowWidth = 600;
	windowHeight = 600;
	xMouseOrigin = windowWidth/2.0;
	yMouseOrigin = windowHeight/2.0;

	ballXPos = ((float)texWidth/mapScaling)/2.0;
	ballZPos = ((float)texWidth/mapScaling)/2.0;
	ballYPos = calcHeight(ballXPos, ballZPos)+1.0;
	
	p = SetVector(5.0,10.0,5.0);
	l =SetVector(0.0,0.0,-1.0);
	v=SetVector(0.0,1.0,0.0);
	//.................

	pos[0] = SetVector(0, 2, 0);
	speed[0] = SetVector(0, -0.2, 0);
	
	
	pos[1] = SetVector(-1, 0, 0);
	speed[1] = SetVector(0.5, 0, 0);
	

	pos[2] = SetVector(0, 1, 0);
	speed[2] = SetVector(0.2, 0.01, 0);
	

	pos[3] = SetVector(1, 1, 1);
	speed[3] = SetVector(0, 1, 0);
	

	pos[4] = SetVector(0, 1, 0.5);
	speed[4] = SetVector(0.2, 0.3, 0.4);
	
	pos[5] = SetVector(1, 1, 0.5);
	speed[5] = SetVector(0.34, 0.09, 0);
	

	pos[6] = SetVector(-1, 1, 0);
	speed[6] = SetVector(0, 2, 0);
	

	pos[7] = SetVector(0.5, 0, 0.5);
	speed[7] = SetVector(0.7, 0, 0);
	

	pos[8] = SetVector(-0.5, 0, 0.5);
	speed[8] = SetVector(0.5, 0, 0);
	
	pos[9] = SetVector(0.5, 1, 0);
	speed[9] = SetVector(0, 0.7, 0.7);
	

	printError("init arrays");
}



void display(void)
{
	int i;
	mat4 modeltoworld, worldtoview, m;
	calcCamPos();
	printError("pre display");
	camMatrix = lookAtv(p,l,v);
	//worldtoview = lookAt(5,10,5, 0,0,-1, 0,1,0);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// Upload matrixes to terrain shader 
	glUseProgram(programt);
	glUniformMatrix4fv(glGetUniformLocation(programt, "mdlMatrix"), 1, GL_TRUE, worldmatrix1.m);
	glUniformMatrix4fv(glGetUniformLocation(programt, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	// Draw terrain
	DrawModel(tm, programt, "inPosition", "inNormal", "inTexCoord");


	
	for (i = 0; i < kNumBalls; i++)
	{
		speed[i] = VectorAdd(grav, speed[i]);
		pos[i] = VectorAdd(pos[i], speed[i]);
		if (pos[i].y < 2) {speed[i].y = -speed[i].y; pos[i].y = 2;}
		if (pos[i].y > 7) {speed[i].y = (speed[i].y -1); pos[i].y = 7;}
		if (pos[i].x < -2) {speed[i].x = -speed[i].x; pos[i].x = -2;}
		if (pos[i].x > 2) {speed[i].x = -speed[i].x; pos[i].x = 2;}
		if (pos[i].z < -2) {speed[i].z = -speed[i].z; pos[i].z = -2;}
		if (pos[i].z > 2) {speed[i].z = -speed[i].z; pos[i].z = 2;}
		
		
		modeltoworld = T(pos[i].x, pos[i].y, pos[i].z);
		m = Mult(camMatrix, modeltoworld); // tr * ry * rz
		glUseProgram(firefly);
		glUniformMatrix4fv(glGetUniformLocation(firefly, "mdlMatrix"), 1, GL_TRUE, m.m);
		glUniformMatrix4fv(glGetUniformLocation(firefly, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
		glUseProgram(firefly);
		glUniform3fv(glGetUniformLocation(firefly, "color"), 1, &objectcolor);
		DrawModel(smallballs, firefly, "inPosition", "inNormal", "");
	}
	
	printError("display");
	
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w': pos[0].y += 0.1;break;
		case 'a': pos[0].x += 0.1;break;
		case 's': pos[0].y -= 0.1;break;
		case 'd': pos[0].x -= 0.1;break;
	}
	
	glutPostRedisplay();
}

static void timer(int arg)
{
	glutTimerFunc(20, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3,2);
	glutCreateWindow ("simple collision example 1");
//	glutRepeatingTimerFunc(20); // I prefer this but it only exists in MicroGlut
	init ();
	glutDisplayFunc(display);	
	//glutTimerFunc(20, timer, 0);
	glutRepeatingTimer(20);
	 
	glutKeyboardFunc(keys);
	
	glutMainLoop();
	exit(0);
}
