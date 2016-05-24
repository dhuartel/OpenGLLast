
#pragma once

#include "cSkyBox.h"
#include "cVector3D.h"
#include "cFrustum.h"
#include "cQuadtreeNode.h"

#define NUM_DATA(a,b,c,d) (((c)-(a))*2*3*((d)-(b)))

class cQuadtree;

class cTerrain
{
public:
	cTerrain();
	virtual ~cTerrain();

	bool Load();
	void Destroy();
	unsigned char GetHeight(int x, int z);
	void Render();

	//Frustum
	void RenderInFrustum(cFrustum *Frustum);
	void SetQuadtree(cQuadtree *Q);
	int  GetWidth();
	int  GetLength();

	//VBO
	void RenderVBONode(int id);

private:
	bool CreateVBOData();
	void CreateVertNormMapWithOrla(cVector3D *Vertices,cVector3D *Normals);
	void CreateVBOIndexRule();
	void CreateVBOIndexRule_NE_bot2top(int *idx,int offset,int Xo,int Yo,int Xf,int Yf);
	
	GLuint *vbo_data;	//vbo data id's
	GLuint vbo_idxrule;	//vbo rule id
	
	int NUM_NODES,WIDTH_NODE;
	int size_vertices_node,size_normals_node;


	void Vertex(int x, int z);
	void Normal(int x, int z);

	float *CrossProduct(int x1,int z1,int x2,int z2,int x3,int z3);
	void  Normalize(float *v);
	void  AddVector(float *a, float *b);
	void  ComputeNormals();
	bool  LoadRawFile(char *filename,int size,bool flipY);
	bool  Scale(float min,float max);

	int   GridSize;  //gridsize = width = length
	unsigned char *HeightMap;
	float *NormalMap;

	cSkyBox SkyBox;
	cQuadtree *Quadtree;
};
