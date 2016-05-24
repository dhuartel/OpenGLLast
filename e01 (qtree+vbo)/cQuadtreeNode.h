#pragma once

#include "cAABB.h"
#include "cVector3D.h"

//         2 3
// Childs: 0 1

class cTerrain;
class cFrustum;

class cQuadtreeNode
{
public:
	cQuadtreeNode();
	~cQuadtreeNode();

	bool Load(cTerrain *T,int levels,int Xo,int Yo,int Xf,int Yf);
	void Release();
	
	bool IsLeaf();
	
	cAABB *GetAABB();
	int GetYmin();
	int GetYmax();
	int GetXmin();
	int GetXmax();
	int GetZmin();
	int GetZmax();

	void ComputeId(int size_node,int width_map);
	void RenderDebug();								//render quadtree (AABB's)

	void Render();
	void Render(cFrustum *Frustum);
	cQuadtreeNode *GetChild(int i);

private:
	void RenderGeometry();
	void RenderCube(int Xo,int Yo,int Zo,int Xf,int Yf,int Zf);

	//-- TODO: AABB of the node instance
	cAABB AABB;
	//--
	//-- TODO: child instances
	cQuadtreeNode* child[4];
	//--
	int id;

	cTerrain *Terrain;
};
