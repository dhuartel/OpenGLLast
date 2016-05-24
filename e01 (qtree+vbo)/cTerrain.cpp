
#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "cTerrain.h"
#include "cScene.h"
#include "cLog.h"

cTerrain::cTerrain()
{
	HeightMap = NULL;
	NormalMap = NULL;
	Quadtree = NULL;
	vbo_data = NULL;
}
cTerrain::~cTerrain() 
{
	Destroy();
}
bool cTerrain::Load()
{
	cLog *Log = cLog::Instance();

	//Load HeightMap from raw file
	if(!LoadRawFile("terrain.raw",MAP_SIZE,true))
	{
		Log->Msg("Error loading heightmap file.");
		return false;
	}
	//Create NormalMap
	NormalMap = (float *)malloc(GridSize * GridSize * sizeof(float) * 3);
	if(NormalMap == NULL)
	{
		Log->Msg("Error allocating memory for NormalMap array.");
		return false;
	}
	ComputeNormals();
	//VBO
	CreateVBOData();
	CreateVBOIndexRule();
	//Create skybox display list
	SkyBox.CreateDL();

	return true;
}
void cTerrain::Destroy()
{
	if (HeightMap != NULL)
	{
		free(HeightMap);
		HeightMap = NULL;
	}
	if (NormalMap != NULL)
	{
		free(NormalMap);
		NormalMap = NULL;
	}
	Quadtree = NULL;

	if(vbo_data != NULL)
	{
		free(vbo_data);
		vbo_data = NULL;
	}
}
bool cTerrain::LoadRawFile(char *filename,int size,bool flipY)
{
	FILE *file = NULL;
	cLog *Log = cLog::Instance();

	file = fopen(filename,"rb");
	if(file == NULL)
	{
		Log->Msg("Error in \"fopen\" function.");
		return false;
	}

	if(HeightMap != NULL)	Destroy();

	GridSize = size;

	unsigned char *data;
	if(flipY)
	{
		data = (unsigned char *)malloc(GridSize * GridSize * sizeof(unsigned char));
		if(data == NULL)
		{
			Log->Msg("Error allocating memory for HeightMap array.");
			return false;
		}
	}
	HeightMap = (unsigned char *)malloc(GridSize * GridSize * sizeof(unsigned char));
	if(HeightMap == NULL)
	{
		Log->Msg("Error allocating memory for HeightMap array.");
		return false;
	}
	if(flipY)	fread(data,1,size*size,file);
	else		fread(HeightMap,1,size*size,file);
	int result = ferror(file);
	if (result)
	{
		Log->Msg("Error in \"fread\" function.");
		return false;
	}
	fclose(file);
	
	//FLIP Y Axis
	if(flipY)
	{
		int i,j,k,lim;
		lim = size*(size-1);
		for(i=0;i<size;i++)
		{
			k = i*size;
			for(j=0;j<size;j++)
			{
				HeightMap[k+j]=data[lim-k+j];
				//australia
				/*if(data[lim-k+j]>64)
					HeightMap[k+j]=(int)data[lim-k+j]/4.0;
				else
					HeightMap[k+j]=data[lim-k+j];*/
			}
		}
	}
	return true;
}

void cTerrain::CreateVertNormMapWithOrla(cVector3D *Vertices,cVector3D *Normals)
{
	//Pre:	N*N
	//		HM = 0..n-1
	//      NM = 0..n-1
	//		Orla => reply last row & column => 0..n
	int i,j,k,N;
	
	N = GridSize;
	
	for(i=0;i<N;i++) //row
	{
		k = i*(N+1);
		for(j=0;j<N;j++) //col
		{
			Vertices[k+j].x = j;
			Vertices[k+j].y = HeightMap[(i*N)+j];
			Vertices[k+j].z = -i;
			Normals[k+j].x = NormalMap[((i*N)+j)*3];
			Normals[k+j].y = NormalMap[((i*N)+j)*3+1];
			Normals[k+j].z = NormalMap[((i*N)+j)*3+2];
		}
		//Reply col
		Vertices[k+j].x = Vertices[k+j-1].x;
		Vertices[k+j].y = Vertices[k+j-1].y;
		Vertices[k+j].z = Vertices[k+j-1].z;
		Normals[k+j].x = Normals[k+j-1].x;
		Normals[k+j].y = Normals[k+j-1].y;
		Normals[k+j].z = Normals[k+j-1].z;
	}
	//Reply row
	k = i*(N+1);
	for(j=0;j<N;j++) //col
	{
		Vertices[k+j].x = Vertices[k+j-(N+1)].x;
		Vertices[k+j].y = Vertices[k+j-(N+1)].y;
		Vertices[k+j].z = Vertices[k+j-(N+1)].z;
		Normals[k+j].x = Normals[k+j-(N+1)].x;
		Normals[k+j].y = Normals[k+j-(N+1)].y;
		Normals[k+j].z = Normals[k+j-(N+1)].z;
	}
}
bool cTerrain::CreateVBOData()
{
	cLog *Log = cLog::Instance();
	cVector3D *Vertices,*Normals,*subVertices,*subNormals;
	int i,j,k,node,base;
	int N,subN;

	Vertices = (cVector3D *)malloc((GridSize+1) * (GridSize+1) * sizeof(cVector3D));
	if(Vertices == NULL)
	{
		Log->Msg("Error allocating memory for Vertices array.");
		return false;
	}
	Normals = (cVector3D *)malloc((GridSize+1) * (GridSize+1) * sizeof(cVector3D));
	if(Normals == NULL)
	{
		Log->Msg("Error allocating memory for Normals array.");
		return false;
	}
	CreateVertNormMapWithOrla(Vertices,Normals);

	NUM_NODES  = (int)pow((double)4,SPATIAL_SUBDIVISION_LEVELS-1);
	WIDTH_NODE = GridSize/(int)pow((double)2,SPATIAL_SUBDIVISION_LEVELS-1);

	size_vertices_node = sizeof(cVector3D)*(WIDTH_NODE+1)*(WIDTH_NODE+1);
	size_normals_node  = size_vertices_node;

	vbo_data   = (GLuint*)malloc(sizeof(GLuint)*NUM_NODES);
	subN        = (WIDTH_NODE+1)*(WIDTH_NODE+1);
	subVertices = (cVector3D*)malloc(sizeof(cVector3D)*subN);
	subNormals  = (cVector3D*)malloc(sizeof(cVector3D)*subN);
	
	//--TODO: VBO management - glGenBuffers - glBindBuffer - glBufferData - glBufferSubData 
	//Generate "NUM_NODES" vbo id's

	glGenBuffers(NUM_NODES, vbo_data);
	//Generar subArrays
	for(node=0;node<NUM_NODES;node++)
	{
		//Generar subArrays
		base = ((node/(int)sqrt((double)NUM_NODES))*((GridSize+1)*WIDTH_NODE)) + ((node%(int)sqrt((double)NUM_NODES))*WIDTH_NODE);
		N = WIDTH_NODE+1;
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				k = base + (i*(GridSize+1)) + j;
				subVertices[i*N+j].x = Vertices[k].x;
				subVertices[i*N+j].y = Vertices[k].y;
				subVertices[i*N+j].z = Vertices[k].z;
				subNormals[i*N+j].x = Normals[k].x;
				subNormals[i*N+j].y = Normals[k].y;
				subNormals[i*N+j].z = Normals[k].z;
			}
		}
		//Create vbo's
		//1. bind node i-èssim
		glBindBuffer(GL_ARRAY_BUFFER,vbo_data[node]);
		//2. set vbo data storage & usage
		glBufferData(GL_ARRAY_BUFFER, size_vertices_node + size_normals_node, 0, GL_STATIC_DRAW);
		//3. load vertices (0 offset)
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_vertices_node, subVertices);
		//4. load normals (size vertices offset)

		glBufferSubData(GL_ARRAY_BUFFER, size_vertices_node, size_normals_node, subNormals);
	}
	//--

	free(Vertices);
	free(Normals);
	free(subVertices);
	free(subNormals);

	return true;
}
void cTerrain::CreateVBOIndexRule()
{
	int N = WIDTH_NODE;
	int size = sizeof(int)*N*N*2*3;

	int *idx_buff = (int*)malloc(size);
	
	//--TODO:
	glGenBuffers(1,&vbo_idxrule);
	CreateVBOIndexRule_NE_bot2top(idx_buff,0,0,0,N,N);
	//1. bind 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idxrule);
	//2. data storage
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, idx_buff, GL_STATIC_DRAW);
	//--

	free(idx_buff);
}
// 3___2   ___3
// |  /|  |  /|
// |a/ |  | /b|
// |/__|  |/__|
// 1      1   2
#define TRI_A(i,j,N,k){	idx[k]   = i*(N+1)       + j;	\
						idx[k+1] = i*(N+1)+(N+1) + j+1;	\
						idx[k+2] = i*(N+1)+(N+1) + j;	}
#define TRI_B(i,j,N,k){	idx[k  ] = i*(N+1)       + j;	\
						idx[k+1] = i*(N+1)       + j+1;	\
						idx[k+2] = i*(N+1)+(N+1) + j+1;	}

void cTerrain::CreateVBOIndexRule_NE_bot2top(int *idx,int offset,int Xo,int Yo,int Xf,int Yf)
{
	int i,j,k=offset;

	for(i=Yo;i<Yf;i++)
	{
		for(j=Xo;j<Xf;j++)
		{
			TRI_A(i,j,WIDTH_NODE,k)
			TRI_B(i,j,WIDTH_NODE,k+3)
			k+=6;
		}
	}
}
void cTerrain::RenderVBONode(int id)
{
	glBindBuffer(GL_ARRAY_BUFFER,vbo_data[id]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo_idxrule);
	
	//-- TODO: enable the 3 client states needed
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	
	
	//--
	//-- TODO: vertex pointer
	glVertexPointer(3, GL_FLOAT, 0, 0);
	//--
	glNormalPointer(GL_FLOAT,0,(void*)size_vertices_node);
	glIndexPointer(GL_UNSIGNED_INT,0,0);
	
	switch(id%6)
	{
		case 0:	glColor3f(0,1,0);	break;
		case 1:	glColor3f(1,1,0);	break;
		case 2:	glColor3f(1,0,0);	break;
		case 3:	glColor3f(1,0,1);	break;
		case 4:	glColor3f(0,0,1);	break;
		case 5:	glColor3f(0,1,1);	break;
	}
	//-- TODO: last step?
	glDrawElements(GL_TRIANGLES,WIDTH_NODE*WIDTH_NODE*2*3,GL_UNSIGNED_INT,0);
	//--
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void cTerrain::Render()
{
	glColor3f(0.2f,0.5f,0.0f);
	Quadtree->Render();
	if(glIsEnabled(GL_DEPTH_TEST)) SkyBox.Render();
}
void cTerrain::RenderInFrustum(cFrustum *Frustum)
{
	Quadtree->Render(Frustum);
	SkyBox.Render();
}

float *cTerrain::CrossProduct(int x1,int z1,int x2,int z2,int x3,int z3) {

	float *auxNormal,v1[3],v2[3];
		
	v1[0] = x2-x1; 
	v1[1] = -HeightMap[z1 * GridSize + x1] 
			+ HeightMap[z2 * GridSize + x2];
	v1[2] = z2-z1; 


	v2[0] = x3-x1; 
	v2[1] = -HeightMap[z1 * GridSize + x1] 
			+ HeightMap[z3 * GridSize + x3];
	v2[2] = z3-z1; 

	auxNormal = (float *)malloc(sizeof(float)*3);

	auxNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];
	auxNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	auxNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];

	return(auxNormal);
}
void cTerrain::Normalize(float *v) {

	double d,x,y,z;

	x = v[0];
	y = v[1];
	z = v[2];
	
	d = sqrt((x*x) + (y*y) + (z*z));

	v[0] = x / d;
	v[1] = y / d;
	v[2] = z / d;
}
void cTerrain::AddVector(float *a, float *b) {

	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
}
void cTerrain::ComputeNormals()
{
	float *norm1,*norm2,*norm3,*norm4; 
	int i,j,k;
	cLog *Log = cLog::Instance();
	
	if (NormalMap == NULL)
	{
		Log->Msg("Error in function \"ComputeNormals\". NormalMap array is empty.");
		return;
	}

	for(i = 0; i < GridSize; i++) {
		for(j = 0; j < GridSize; j++) {
			norm1 = NULL;
			norm2 = NULL;
			norm3 = NULL;
			norm4 = NULL;

			// normals for the four corners
			if (i == 0 && j == 0) {
				norm1 = CrossProduct(0,0, 0,1, 1,0);	
				Normalize(norm1);				
			}
			else if (j == GridSize-1 && i == GridSize-1) {
				norm1 = CrossProduct(i,j, j,i-1, j-1,i);	
				Normalize(norm1);				
			}
			else if (j == 0 && i == GridSize-1) {
				norm1 = CrossProduct(i,j, j,i-1, j+1,i);	
				Normalize(norm1);				
			}
			else if (j == GridSize-1 && i == 0) {
				norm1 = CrossProduct(i,j, j,i+1, j-1,i);	
				Normalize(norm1);				
			}

			// normals for the borders
			else if (i == 0) {
				norm1 = CrossProduct(j,0, j-1,0, j,1);
				Normalize(norm1);
				norm2 = CrossProduct(j,0,j,1,j+1,0);
				Normalize(norm2);
			}
			else if (j == 0) {
				norm1 = CrossProduct(0,i, 1,i, 0,i-1);
				Normalize(norm1);
				norm2 = CrossProduct(0,i, 0,i+1, 1,i);
				Normalize(norm2);
			}
			else if (i == GridSize) {
				norm1 = CrossProduct(j,i, j,i-1, j+1,i);
				Normalize(norm1);
				norm2 = CrossProduct(j,i, j+1,i, j,i-1);
				Normalize(norm2);
			}
			else if (j == GridSize) {
				norm1 = CrossProduct(j,i, j,i-1, j-1,i);
				Normalize(norm1);
				norm2 = CrossProduct(j,i, j-1,i, j,i+1);
				Normalize(norm2);
			}

			// normals for the interior
			else {
				norm1 = CrossProduct(j,i, j-1,i, j,i+1);
				Normalize(norm1);
				norm2 = CrossProduct(j,i, j,i+1, j+1,i);
				Normalize(norm2);
				norm3 = CrossProduct(j,i, j+1,i, j,i-1);
				Normalize(norm3);
				norm4 = CrossProduct(j,i, j,i-1, j-1,i);
				Normalize(norm4);
			}
			if (norm2 != NULL) {
				AddVector(norm1,norm2);
				free(norm2);
			}
			if (norm3 != NULL) {
				AddVector(norm1,norm3);
				free(norm3);
			}
			if (norm4 != NULL) {
				AddVector(norm1,norm4);
				free(norm4);
			}
			Normalize(norm1);
			norm1[2] = - norm1[2];
			for (k = 0; k< 3; k++) 
				NormalMap[3*(i*GridSize + j) + k] = norm1[k];
			free(norm1);
		}
	}
}
bool cTerrain::Scale(float min,float max) {

	float amp,aux,min1,max1,height;
	int total,i;
	cLog *Log = cLog::Instance();

	if (HeightMap == NULL)
	{
		Log->Msg("Error in function \"Scale\". HeightMap array is empty.");
		return false;
	}

	if (min > max) {
		aux = min;
		min = max;
		max = aux;
	}

	amp = max - min;
	total = GridSize * GridSize;

	min1 = HeightMap[0];
	max1 = HeightMap[0];
	for(i=1;i < total ; i++) {
		if (HeightMap[i] > max1)
			max1 = HeightMap[i];
		if (HeightMap[i] < min1)
			min1 = HeightMap[i];
	}
	for(i=0;i < total; i++) {
		height = (HeightMap[i] - min1) / (max1-min1);
		HeightMap[i] = height * amp - min;
	}
	if (NormalMap != NULL)
		ComputeNormals();
	return true;
}

unsigned char cTerrain::GetHeight(int x, int z) 
{
	if (HeightMap == NULL) 
			return(0);

	if(x > GridSize-1)	
		x = GridSize-1;
	else if(x < 0)			
		x = 0;
	if(z > GridSize-1)	
		z = GridSize-1;
	else if(z < 0)			
		z = 0;

	return HeightMap[z * GridSize + x];
}
void cTerrain::Vertex(int x,int z)
{
	glVertex3f(x, GetHeight(x,z), -z);
}
void cTerrain::Normal(int x, int z) 
{
	int idx;

	if(NormalMap != NULL)
	{
		if(x > GridSize-1)	
			x = GridSize-1;
		else if(x < 0)			
			x = 0;
		if(z > GridSize-1)	
			z = GridSize-1;
		else if(z < 0)			
			z = 0;

		idx = (z * GridSize + x) * 3;
		glNormal3f(	NormalMap[idx  ],
					NormalMap[idx+1],
					NormalMap[idx+2]);
	}
}

void cTerrain::SetQuadtree(cQuadtree *Q)
{
	Quadtree = Q;
}
int cTerrain::GetWidth()
{
	return GridSize;
}
int cTerrain::GetLength()
{
	return GridSize;
}