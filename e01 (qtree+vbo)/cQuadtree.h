#pragma once

#include "cTerrain.h"
#include "cQuadtreeNode.h"

class cQuadtree
{
public:
	cQuadtree();
	~cQuadtree();

	bool Load(cTerrain *Terrain,int size,int levels);
	void ComputeIds(int size_node,int width_map);
	
	void Render();
	void Render(cFrustum *Frustum);
	void RenderDebug();
	void Release();
	
	cQuadtreeNode *GetRoot();

private:
	//--TODO: root node instance
	cQuadtreeNode* root;
	//--
};