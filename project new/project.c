
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// --------------------------Global variables---------------------

mat4 projectionMatrix;

float mapScaling = 4.0;

// vertex array object
Model *m, *m2, *tm, *sphere, *skybox, *skySphere, *wholeTree;
//
// Reference to shader program
GLuint programTerrain, programSkyBox, programObjects, programCloudBox, programTrees;
GLuint grasstex, dirt, venus, cloudTex, crown, nightSky;
TextureData ttex;

vec3 l, p, v, walkSideways, lookingDir;
float ballXPos, ballYPos, ballZPos, dx, dz, dyx, dyz, deltaX, deltaY, previousHeight, currentHeight, heightDiff, a, ballRotation;
mat4 trans, trans2, rot, ballRotMat, ballTransMat, ballTotMat, camera, rotateUpDown, modelView, camMatrix, modelToWorld, cameraSkyBox, worldmatrix1, worldmatrix2, worldmatrix3, worldmatrix4, worldmatrix5, worldmatrix6, worldmatrix7, worldmatrix8, worldmatrix9;
int texWidth, texHeight;
int windowWidth, windowHeight, xMouseOrigin, yMouseOrigin;
int numberOfTrees = 100;
int numberOfClusters = 9;
bool fly = false;


		// Define  Night light
Point3D lightSourcesColorsArr[10];

GLfloat specularExponent[10];
GLint isDirectional[10];

Point3D lightSourcesDirectionsPositions[10];

typedef struct Tree {

	float pos[3];
	} tree;
	tree *treeList;

typedef struct Cluster{
	float pos[3];
	float xCoordinate[10];
	float yCoordinate[10];
	float zCoordinate[10];
	}cluster;

	cluster *clusterList;


// ----------------------------- Functions------------------------------

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
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 70.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / mapScaling;
// Normal vectors. You need to calculate these.
// further down
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width*10;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height*10;

		}

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Beräkna indexar
			if(x < tex->width-1 && z < tex->height-1){
			// Triangle 1
				indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
			// Triangle 2
				indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
				}


			// beräkna normalvectorer
			if(x>0 && z>0 && x < tex->width-1 && z < tex->height-1){
				u.x = vertexArray[(x+1 + z * tex->width)*3+0] - vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				u.y = vertexArray[(x+1 + z * tex->width)*3+1] - vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				u.z = vertexArray[(x+1 + z * tex->width)*3+2] - vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				v.x = vertexArray[(x + (z+1) * tex->width)*3+0] - vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				v.y = vertexArray[(x + (z+1) * tex->width)*3+1] - vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				v.z = vertexArray[(x + (z+1) * tex->width)*3+2] - vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				vec3 normal= Normalize(CrossProduct(v,u));

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}

			if(x == 0 && z > 0 && z < tex->height-1){
				u.x = vertexArray[(x+1 + z * tex->width)*3+0] - (vertexArray[(x + z * tex->width)*3+0]-1/mapScaling);
				u.y = vertexArray[(x+1 + z * tex->width)*3+1] - vertexArray[(tex->width-2 + (z-1) * tex->width)*3+1];
				u.z = vertexArray[(x+1 + z * tex->width)*3+2] - (vertexArray[(x + z * tex->width)*3+2]-1/mapScaling);

				v.x = vertexArray[(x + (z+1) * tex->width)*3+0] - ((vertexArray[(x + z * tex->width)*3+0])-1/mapScaling);
				v.y = vertexArray[(x + (z+1) * tex->width)*3+1] - vertexArray[(tex->width-2 + (z-1) * tex->width)*3+1];
				v.z = vertexArray[(x + (z+1) * tex->width)*3+2] - ((vertexArray[(x + z * tex->width)*3+2]-1/mapScaling));

				vec3 normal= Normalize(CrossProduct(v,u));

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}

			if(x == tex->width-1 && z > 0 && z < tex->height-1){
				u.x = (vertexArray[(x + z * tex->width)*3+0]+1/mapScaling) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				u.y = vertexArray[(0+1 + z * tex->width)*3+1] - 		vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				u.z = vertexArray[(x + z * tex->width)*3+2] - 			vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				v.x = (vertexArray[(x + (z+1) * tex->width)*3+0]) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				v.y = vertexArray[(x + (z+1) * tex->width)*3+1] - 	vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				v.z = (vertexArray[(x + (z+1) * tex->width)*3+2]) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				vec3 normal= Normalize(CrossProduct(v,u));

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}

			if(z == 0 && x> 0 && x < tex->width-1){
				u.x = vertexArray[(x+1 + z * tex->width)*3+0] - (vertexArray[(x + z * tex->width)*3+0]-1/mapScaling);
				u.y = vertexArray[(x+1 + z * tex->width)*3+1] - vertexArray[(x-1 + (tex->height-2) * tex->width)*3+1];
				u.z = vertexArray[(x+1 + z * tex->width)*3+2] - (vertexArray[(x + z * tex->width)*3+2]-1/mapScaling);

				v.x = vertexArray[(x + (z+1) * tex->width)*3+0] - (vertexArray[(x + z * tex->width)*3+0]-1/mapScaling);
				v.y = vertexArray[(x + (z+1) * tex->width)*3+1] - vertexArray[(x-1 + (tex->height-2) * tex->width)*3+1];
				v.z = vertexArray[(x + (z+1) * tex->width)*3+2] - (vertexArray[(x + z * tex->width)*3+2]-1/mapScaling);

				vec3 normal= Normalize(CrossProduct(v,u));

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}

			if(z == tex->height-1 && x > 0  && x < tex->width-1){
				u.x = (vertexArray[(x+1 + z * tex->width)*3+0]) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				u.y = vertexArray[(x+1 + z * tex->width)*3+1] - 	vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				u.z = (vertexArray[(x+1 + z * tex->width)*3+2]) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				v.x = (vertexArray[(x + z * tex->width)*3+0]) - 				vertexArray[(x-1 + (z-1) * tex->width)*3+0];
				v.y = vertexArray[(x + (0+1) * tex->width)*3+1] - 				vertexArray[(x-1 + (z-1) * tex->width)*3+1];
				v.z = (vertexArray[(x + z * tex->width)*3+2]+1/mapScaling) - 	vertexArray[(x-1 + (z-1) * tex->width)*3+2];

				vec3 normal= Normalize(CrossProduct(v,u));

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}
			if((z == tex->height-1 && x == tex->width-1) || (x == 0 && z == 0) || (x == 0 && z == tex->height-1) || (z == 0 && x == tex->width-1)){

				vec3 normal;

				normal.x = 0;
				normal.y = 0.5;
				normal.z = 0;

				normalArray[(x + z * tex->width)*3+ 0] = normal.x;
				normalArray[(x + z * tex->width)*3+ 1] = normal.y;
				normalArray[(x + z * tex->width)*3+ 2] = normal.z;
			}

		}


	// End of terrain generation

	// Create Model

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

// This function is used to get the x-position on the middle tile (if the input is a position on any of the surrounding tiles)
float warpXIfOutside(float x){
	if(x > (texWidth-1)/mapScaling){
		x = x - (texWidth-1)/mapScaling;
	}
	if(x < 0){
		x = x + (texWidth-1)/mapScaling;
	}

	if(x>(texWidth-3)/mapScaling){
		x = x-2;
	}
	if(x<2){
		x = x+2;
	}
	return x;
}

// This function is used to get the z-position on the middle tile (if the input is a position on any of the surrounding tiles)
float warpZIfOutside(float z){
	if(z > (texHeight-1)/mapScaling){
		z = z - (texHeight-1)/mapScaling;
	}
	if(z < 0){
		z = z + (texWidth-1)/mapScaling;
	}

	if(z>(texWidth-3)/mapScaling){
		z = z-2;
	}
	if(z<2){
		z = z+2;
	}
	return z;
}

// This function calculates the height of the terrain given a specific x,z - coordinate
float calcHeight(float x, float z){

	int d = floor(x * mapScaling);
	int e = floor(z * mapScaling);
	if(d>(texWidth-3)){
		d = d-2;
	}
	if(d<2){
		d = d+2;
	}
	if(e>(texWidth-3)){
		e = e-2;
	}
	if(e<2){
		e = e+2;
	}
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

//	Initialize the arrays with lightsources used during night.
void setNightLight(){
	// First source is directional
	lightSourcesColorsArr[0].x = 1.0f;
	lightSourcesColorsArr[0].y = 0.5f;
	lightSourcesColorsArr[0].z = 0.5f;
	specularExponent[0] = 60.0f;
	isDirectional[0] = 1;
	lightSourcesDirectionsPositions[0].x = 10.0f;
	lightSourcesDirectionsPositions[0].y = 10.0f;
	lightSourcesDirectionsPositions[0].z = 10.0f;
	// The rest is set to the cluster positions
	for(int i = 1; i<numberOfClusters+1; i++){
		lightSourcesColorsArr[i].x = 0.5f;
		lightSourcesColorsArr[i].y = 0.5f;
		lightSourcesColorsArr[i].z = 0.0f;
		specularExponent[i] = 60.0;
		isDirectional[i] = 0;
		lightSourcesDirectionsPositions[i].x = clusterList[i-1].pos[0];
		lightSourcesDirectionsPositions[i].y = clusterList[i-1].pos[1];
		lightSourcesDirectionsPositions[i].z = clusterList[i-1].pos[2];
	}
}

void updateNightLight(){
	// The sets cluster positions
	for(int i = 1; i<numberOfClusters+1; i++){
		lightSourcesColorsArr[i].x = 0.5f;
		lightSourcesColorsArr[i].y = 0.5f;
		lightSourcesColorsArr[i].z = 0.0f;
		specularExponent[i] = 50.0;
		isDirectional[i] = 0;
		lightSourcesDirectionsPositions[i].x = clusterList[i-1].pos[0];
		lightSourcesDirectionsPositions[i].y = clusterList[i-1].pos[1];
		lightSourcesDirectionsPositions[i].z = clusterList[i-1].pos[2];
	}
  glUseProgram(programTerrain);
  glUniform3fv(glGetUniformLocation(programTerrain, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);
  glUseProgram(programObjects);
  glUniform3fv(glGetUniformLocation(programObjects, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);
  glUseProgram(programTrees);
  glUniform3fv(glGetUniformLocation(programTrees, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);
}





// This function replaces trees that has been warped outside the 9 tiles and replaces those with trees at the empty tile.
void updateTrees(){
		// Check if outside
	for(int i=0; i<numberOfTrees; i++){
			if (treeList[i].pos[0] > 2*(texWidth-1)/mapScaling){
				// define a new random position at the empty tiles
				float randomX = (rand() % (texWidth-1)/mapScaling)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;

				// If the tree is outside the middle patch, instead use coordinates at the middle patch to calc height.
				float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);

				// Update the positions of the objects
				treeList[i].pos[0] = randomX;
				treeList[i].pos[2] = randomZ;
				treeList[i].pos[1] = calcHeight(tempX, tempZ);

			}
			if(treeList[i].pos[2] > 2*(texWidth-1)/mapScaling){
				float randomX = (rand() % (3*texWidth-3)/mapScaling-2)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (texHeight-1)/mapScaling)-(texHeight-1)/mapScaling;

				float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);

				treeList[i].pos[0] = randomX;
				treeList[i].pos[2] = randomZ;
				treeList[i].pos[1] = calcHeight(tempX, tempZ);

				}
			if(treeList[i].pos[0] < -(texWidth-1)/mapScaling){
				float randomX = (rand() % (texWidth-1)/mapScaling)+(texWidth-1)/mapScaling;
				float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;

				float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);

				treeList[i].pos[0] = randomX;
				treeList[i].pos[2] = randomZ;
				treeList[i].pos[1] = calcHeight(tempX, tempZ);
				}
			if(treeList[i].pos[2] < -(texWidth-1)/mapScaling){
				float randomX = (rand() % (3*texWidth-3)/mapScaling-2)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (texHeight-1)/mapScaling)+(texHeight-1)/mapScaling;

				float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);

				treeList[i].pos[0] = randomX;
				treeList[i].pos[2] = randomZ;
				treeList[i].pos[1] = calcHeight(tempX, tempZ);

				}

	}


}


void clusterMovement(){
  for(int i=0; i<numberOfClusters; i++){
    clusterList[i].pos[0]=clusterList[i].pos[0]+0.02;
    clusterList[i].pos[2]=clusterList[i].pos[2]+0.02;
  }
  updateNightLight();


}
// This function replaces clusters that has been warped outside the 9 tiles and replaces those with clusters at the empty tile.
void updateClusters(){

	for(int i=0; i<numberOfClusters; i++){
		// Check if outside
			if (clusterList[i].pos[0] > 2*(texWidth-1)/mapScaling){
				// define a new random position at the empty tiles
				float randomX = (rand() % (texWidth-1)/mapScaling)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;


        float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);
				// Update the positions of the objects
				clusterList[i].pos[0] = tempX;
				clusterList[i].pos[2] = tempZ;
				clusterList[i].pos[1] = calcHeight(tempX, tempZ)+5;
			}

			if(clusterList[i].pos[2] > 2*(texWidth-1)/mapScaling){
				float randomX = (rand() % (3*texWidth-3)/mapScaling-2)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (texHeight-1)/mapScaling)-(texHeight-1)/mapScaling;
        float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);
				// Update the positions of the objects
				clusterList[i].pos[0] = tempX;
				clusterList[i].pos[2] = tempZ;
				clusterList[i].pos[1] = calcHeight(tempX, tempZ)+5;
			}
			if(clusterList[i].pos[0] < -(texWidth-1)/mapScaling){
				float randomX = (rand() % (texWidth-1)/mapScaling)+(texWidth-1)/mapScaling;
				float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;

        float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);
				// Update the positions of the objects
				clusterList[i].pos[0] = tempX;
				clusterList[i].pos[2] = tempZ;
				clusterList[i].pos[1] = calcHeight(tempX, tempZ)+5;

			}
			if(treeList[i].pos[2] < -(texWidth-1)/mapScaling){
				float randomX = (rand() % (3*texWidth-3)/mapScaling-2)-(texWidth-1)/mapScaling;
				float randomZ = (rand() % (texHeight-1)/mapScaling)+(texHeight-1)/mapScaling;

        float tempX = warpXIfOutside(randomX);
				float tempZ = warpZIfOutside(randomZ);
				// Update the positions of the objects
				clusterList[i].pos[0] = tempX;
				clusterList[i].pos[2] = tempZ;
				clusterList[i].pos[1] = calcHeight(tempX, tempZ)+5;
			}

	}


}

// Initialize trees
tree *setTrees(){

	tree *array;
	array = malloc (numberOfTrees * sizeof(tree));

	for(int i=0; i<numberOfTrees; i++){
			float randomX = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;
			float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;

			// If the tree is outside the middle patch, instead use coordinates at the middle patch to calc height.
			float tempX = warpXIfOutside(randomX);
			float tempZ = warpZIfOutside(randomZ);

			array[i].pos[0] = randomX;
			array[i].pos[2] = randomZ;
			array[i].pos[1] = calcHeight(tempX, tempZ);
			//printf("tree %.6f\n", array[i].pos[1]);



	}
	return array;

}
// Initialize the firefly clusters
cluster *setFireflies(){

	cluster *array;
	array = malloc (numberOfTrees * sizeof(cluster));

	// Create a number of random placed clusters
	for(int i=0; i<numberOfClusters; i++){
		float randomX = (rand() % (3*texWidth-3)/mapScaling-2)-(texWidth-1)/mapScaling;
		float randomZ = (rand() % (3*texHeight-3)/mapScaling-2)-(texHeight-1)/mapScaling;
		float randomY = rand() % 5;

		array[i].pos[0]=randomX;
		array[i].pos[2]=randomZ;
		array[i].pos[1] = 3 +randomY;

		// Create  7 randomly distributed fireflies around the cluster position
		for(int k = 0; k<7; k++){
			float randX = rand() % 10;
			float randY = rand() % 10;
			float randZ = rand() % 10;
			array[i].xCoordinate[k] = array[i].pos[0] + randX;
			array[i].yCoordinate[k] = array[i].pos[1] + randY;
			array[i].zCoordinate[k] = array[i].pos[2] + randZ;
			//printf("fly %.6f\n", array[i].yCoordinate[k]);

		}
	}
return array;

}
		// This function is looking for key events and updates position.
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

	if (glutKeyIsDown('x')) {
	 }
	if (glutKeyIsDown('z')) {
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// Linear Filtered
	 }
	if (glutKeyIsDown('y')) {
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filtered
	 }
   if (glutKeyIsDown('e')) {
       p.y = p.y + 1;
 	 }
   if (glutKeyIsDown('q')) {
       p.y = p.y + -1;
 	 }



	// This part warps the players position and the position of the objects. The functions: updateTrees and updateClusters are defined earlier and those will replace objects that warps outside the world.

	//If the player walk outside move it back.
	if (p.x > (texWidth-1)/mapScaling){
		p.x = p.x -(texWidth-1)/mapScaling;
		//Move the looking-dir vector aswell
		l.x = l.x -(texWidth-1)/mapScaling;

		for(int i=0; i<numberOfTrees; i++){
			// Also move the trees.
			treeList[i].pos[0] = treeList[i].pos[0] - (texWidth-1)/mapScaling;
		}

			for(int i = 0; i<numberOfClusters; i++){
				// And the clusters if night
				clusterList[i].pos[0] = clusterList[i].pos[0] - (texWidth-1)/mapScaling;
			}
      // Replace trees that is outside the world
      updateTrees();
			// Replace clusters that is outside the world
			updateClusters();
			// Update the lightsource positions to place them at the same place as the new clusters
			updateNightLight();
			//Upload the new light sources


	}

	if (p.z > (texWidth-1)/mapScaling){
		p.z = p.z - (texWidth-1)/mapScaling;
		l.z = l.z - (texWidth-1)/mapScaling;
		for(int i=0; i<numberOfTrees; i++){
			treeList[i].pos[2] = treeList[i].pos[2] - (texWidth-1)/mapScaling;
		}

			for(int i = 0; i<numberOfClusters; i++){
				clusterList[i].pos[2] = clusterList[i].pos[2]- (texWidth-1)/mapScaling;
			}
      updateTrees();
			updateClusters();
			updateNightLight();


	}
	if (p.x < 0){
		p.x = p.x + (texWidth-1)/mapScaling;
		l.x = l.x + (texWidth-1)/mapScaling;
		for(int i=0; i<numberOfTrees; i++){
			treeList[i].pos[0] = treeList[i].pos[0] + (texWidth-1)/mapScaling;
		}
			for(int i = 0; i<numberOfClusters; i++){
				clusterList[i].pos[0] = clusterList[i].pos[0] + (texWidth-1)/mapScaling;
			}
      updateTrees();
			updateClusters();
			updateNightLight();



	}
	if (p.z < 0){
		p.z = p.z + (texWidth-1)/mapScaling;
		l.z = l.z + (texWidth-1)/mapScaling;
		for(int i=0; i<numberOfTrees; i++){
			treeList[i].pos[2] = treeList[i].pos[2] + (texWidth-1)/mapScaling;
		}
			for(int i = 0; i<numberOfClusters; i++){
				clusterList[i].pos[2] = clusterList[i].pos[2] + (texWidth-1)/mapScaling;
			}
      updateTrees();
			updateClusters();
			updateNightLight();


	}

	currentHeight = calcHeight(p.x, p.z);
	heightDiff = currentHeight - previousHeight;
	l.y = l.y + heightDiff;
	p.y = p.y + heightDiff;
	previousHeight = currentHeight;


}

// Track the mouse and rotate the world accordingley. Also keep the pointer in the middle of the screen
void mouse(int x, int y)
{
	if (x != xMouseOrigin || y != xMouseOrigin){
		deltaX = (float)(xMouseOrigin - x)/2000.0;
		deltaY = (float) (yMouseOrigin - y)/2000.0;
		glutWarpPointer(xMouseOrigin, yMouseOrigin);

		trans = T(-p.x, -p.y, -p.z);
		trans2 = T(p.x, p.y, p.z);
		rot = Ry(deltaX);

		rotateUpDown = ArbRotate(walkSideways, deltaY);

		l = MultVec3(trans2, MultVec3(rot, MultVec3(trans, l)));
		l = MultVec3(trans2, MultVec3(rotateUpDown, MultVec3(trans, l)));
	}

}

// ------------------ Initialize everything needed to run ---------------------

void init(void)
{

	srand ( time(NULL) );
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Frustumläng cant be "longer" than one tile
	projectionMatrix = frustum(-0.2, 0.2, -0.1, 0.1, 0.2, 60);

	// ^^^Stuff for skybox^^^

	// Load and compile Skybox shaders
	programSkyBox = loadShaders("skybox.vert", "skybox.frag");
	programCloudBox = loadShaders("cloudBox.vert", "cloudBox.frag");
	printError("init shader");

	// Load skybox object and skybox texture
	skybox = LoadModelPlus("skyboxmod2.obj");
	skySphere = LoadModelPlus("sphere.obj");
	LoadTGATextureSimple("Textures/moon.tga", &nightSky);
	LoadTGATextureSimple("Textures/cloudsBW.tga", &cloudTex);

	// Upload the projectionmatrix to the skybox and cloudbox shader
	glUseProgram(programSkyBox);
	glUniformMatrix4fv(glGetUniformLocation(programSkyBox, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(programCloudBox);
	glUniformMatrix4fv(glGetUniformLocation(programCloudBox, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// For Terrain, Load and compile shader
	programTerrain = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(programTerrain);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Load terrain data
	LoadTGATextureData("fft-terrain2.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	TextureData *texturePointer = &ttex;
	texWidth = texturePointer->width;
	texHeight = texturePointer->height;
	printError("init terrain");

	// Load objects
	sphere = LoadModelPlus("sphere.obj");
	wholeTree = LoadModelPlus("Fir.obj");

	// Load and compile shaders for objects------------
	programObjects = loadShaders("objects.vert", "objects.frag");
	glUseProgram(programObjects);
	printError("init shader");
  glUniformMatrix4fv(glGetUniformLocation(programObjects, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

  programTrees = loadShaders("trees.vert", "trees.frag");
  glUseProgram(programTrees);
  printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(programTrees, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);


	// Load and init textures
	LoadTGATextureSimple("Textures/grassTexture.tga", &grasstex);
	LoadTGATextureSimple("Textures/dirt.tga", &dirt);
	LoadTGATextureSimple("Textures/venus.tga", &venus);
	LoadTGATextureSimple("Textures/crown.tga", &crown);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dirt);

	glUseProgram(programTerrain);
	glUniform1i(glGetUniformLocation(programTerrain, "grassTexture"), 0);
	glUniform1i(glGetUniformLocation(programTerrain, "dirt"), 1);


	// update ball pos and init stuff dependent on above loading
	windowWidth = 1000;
	windowHeight = 1000;
	xMouseOrigin = windowWidth/2;
	yMouseOrigin = windowHeight/2;

	ballXPos = 0.0;
	ballZPos = 0.0;
	ballYPos = calcHeight(ballXPos, ballZPos)+1.0;

	p = SetVector(1.0, calcHeight(1.0,1.0)+2, 1.0);
	l = SetVector(3.0, ballYPos+2.0, 3.0);
	v = SetVector(0.0, 1.0, 0.0);

	previousHeight = calcHeight(p.x,p.z);

	// Calculate matrixes for the world
	worldmatrix1 = IdentityMatrix();
	worldmatrix2 = Mult(worldmatrix1, T((texWidth-1)/mapScaling, 0, 0));
	worldmatrix3 = Mult(worldmatrix1, T((texWidth-1)/mapScaling, 0, (texWidth-1)/mapScaling));
	worldmatrix4 = Mult(worldmatrix1, T((texWidth-1)/mapScaling, 0, -(texWidth-1)/mapScaling));
	worldmatrix5 = Mult(worldmatrix1, T(-(texWidth-1)/mapScaling, 0, 0));
	worldmatrix6 = Mult(worldmatrix1, T(-(texWidth-1)/mapScaling, 0, (texWidth-1)/mapScaling));
	worldmatrix7 = Mult(worldmatrix1, T(-(texWidth-1)/mapScaling, 0, -(texWidth-1)/mapScaling));
	worldmatrix8 = Mult(worldmatrix1, T(0, 0, (texWidth-1)/mapScaling));
	worldmatrix9 = Mult(worldmatrix1, T(0, 0, -(texWidth-1)/mapScaling));

	// Init treeList
	treeList = setTrees();
	// Init fireflies
	clusterList = setFireflies();
	// Init lightsources. (placed at firefly cluster positions)
	setNightLight();

	// Upload lightsources and specifications
	glUseProgram(programTerrain);
	glUniform3fv(glGetUniformLocation(programTerrain, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);

	glUniform3fv(glGetUniformLocation(programTerrain, "lightSourcesColorArr"), 10, &lightSourcesColorsArr);
	glUniform1fv(glGetUniformLocation(programTerrain, "specularExponent"), 10, specularExponent);
	glUniform1iv(glGetUniformLocation(programTerrain, "isDirectional"), 10, isDirectional);

	glUseProgram(programObjects);
	glUniform3fv(glGetUniformLocation(programObjects, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);
	glUniform3fv(glGetUniformLocation(programObjects, "lightSourcesColorArr"), 10, &lightSourcesColorsArr);
	glUniform1fv(glGetUniformLocation(programObjects, "specularExponent"), 10, specularExponent);
	glUniform1iv(glGetUniformLocation(programObjects, "isDirectional"), 10, isDirectional);

  glUseProgram(programTrees);
  glUniform3fv(glGetUniformLocation(programTrees, "lightSourcesDirPosArr"), 10, &lightSourcesDirectionsPositions);
  glUniform3fv(glGetUniformLocation(programTrees, "lightSourcesColorArr"), 10, &lightSourcesColorsArr);
	  printf("%f\n",&lightSourcesColorsArr);

  glUniform1fv(glGetUniformLocation(programTrees, "specularExponent"), 10, specularExponent);
  glUniform1iv(glGetUniformLocation(programTrees, "isDirectional"), 10, isDirectional);


}


// --------------------------- Loop this 30 times/sek -----------------------

void display(void)
{
	ballXPos += 0.01;
	ballZPos += 0.01;
	ballYPos = calcHeight(ballXPos, ballZPos)+1.0;

 	ballRotation += 0.1;
	ballRotMat = Mult(Rx(ballRotation/6), Rz(ballRotation/6));
	ballTransMat = T(ballXPos, ballYPos, ballZPos);
	ballTotMat = Mult(ballTransMat, ballRotMat);




  clusterMovement();
	calcCamPos();

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	printError("pre display");

	// Build matrix
	camMatrix = lookAtv(p,l,v);
	cameraSkyBox = mat3tomat4(mat4tomat3(camMatrix));
  cameraSkyBox = Mult(cameraSkyBox, Ry(-0.9));
	cameraSkyBox = Mult(T(0.0, -0.1, 0.0), cameraSkyBox);

	// ^^^^Do thing with skybox^^^

	// Upload camMatrix to skyboxShader
	glUseProgram(programSkyBox);
	glUniformMatrix4fv(glGetUniformLocation(programSkyBox, "camSkyBoxMatrix"), 1, GL_TRUE, cameraSkyBox.m);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, nightSky);
	glUniform1i(glGetUniformLocation(programSkyBox, "skyTex"), 2);

	DrawModel(skybox, programSkyBox,"inPosition" , NULL, "inTexCoord");

	// Calculate and draw cloudBox if night

	glUseProgram(programCloudBox);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cloudTex);
	glUniform1i(glGetUniformLocation(programCloudBox, "cloudTex"), 2);
	cameraSkyBox = mat3tomat4(mat4tomat3(camMatrix));
	cameraSkyBox =  Mult(cameraSkyBox, Ry(a+=0.0005));
	cameraSkyBox = Mult(T(0.0, 0.1, 0.0), cameraSkyBox);
	glUniformMatrix4fv(glGetUniformLocation(programCloudBox, "camSkyBoxMatrix"), 1, GL_TRUE, cameraSkyBox.m);
	glUniformMatrix4fv(glGetUniformLocation(programCloudBox, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	DrawModel(skySphere, programCloudBox,"inPosition" , "inNormal", "inTexCoord");
	glCullFace(GL_BACK);
	DrawModel(skySphere, programCloudBox,"inPosition" , "inNormal", "inTexCoord");
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	// Upload matrixes to terrain shader
	glUseProgram(programTerrain);
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix1.m);
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);

	// Draw terrain
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");

	// Upload and draw the rest of the terrain tiles
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix2.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix3.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix4.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix5.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix6.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix7.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix8.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, worldmatrix9.m);
	DrawModel(tm, programTerrain, "inPosition", "inNormal", "inTexCoord");



	//Upload sphere
	modelToWorld = ballTotMat;
	glUseProgram(programObjects);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, venus);
	glUniform1i(glGetUniformLocation(programObjects, "objectTexture"), 3);
	glUniformMatrix4fv(glGetUniformLocation(programObjects, "mdlMatrix"), 1, GL_TRUE, modelToWorld.m);
	glUniformMatrix4fv(glGetUniformLocation(programObjects, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	//Draw sphere
	glCullFace(GL_FRONT);
	DrawModel(sphere, programObjects, "inPosition", "inNormal", "inTexCoord");
	glCullFace(GL_BACK);
	DrawModel(sphere, programObjects,"inPosition" , "inNormal", "inTexCoord");
	glDisable(GL_BLEND);


  // Upload and draw all trees
  glUseProgram(programTrees);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, crown);

  glUniform1i(glGetUniformLocation(programTrees, "objectTexture"), 3);
  glUniformMatrix4fv(glGetUniformLocation(programTrees, "cameraMatrix"), 1, GL_TRUE, camMatrix.m);
  for(int i = 0; i<numberOfTrees; i++){
    modelToWorld = Mult(T(treeList[i].pos[0], treeList[i].pos[1], treeList[i].pos[2]), S(0.01,0.01,0.01));
    glUniformMatrix4fv(glGetUniformLocation(programTrees, "mdlMatrix"), 1, GL_TRUE, modelToWorld.m);
    DrawModel(wholeTree, programTrees, "inPosition", "inNormal", "inTexCoord");
  }
	printError("display 2");

	// Upload and draw all fireflies if night is precent

		for (int i = 0; i < numberOfClusters; i++){
      vec3 d;
      d.x = p.x - clusterList[i].pos[0];
      d.z = p.z - clusterList[i].pos[2];

			for(int k = 0; k<10; k++){

			}

		}


	glutSwapBuffers();
	printError("display 3");


}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 600);
	glutCreateWindow ("Whoot");
	init();
	glutDisplayFunc(display);
	glutRepeatingTimer(20);
	glutPassiveMotionFunc(mouse);
	glutHideCursor();
	glutMainLoop();
	exit(0);

}
