#include "cQuadtree.h"
#include "cFrustum.h"
#include "cLog.h"
#include "globals.h"
#include <stdio.h>
#include <gl/glut.h>

cQuadtree::cQuadtree()
{
	root   = NULL;
}
cQuadtree::~cQuadtree()
{
	Release();
}
void cQuadtree::Release()
{
	if(root != NULL)
	{
		root->Release();
		delete root;
		root = NULL;
	}
}
bool cQuadtree::Load(cTerrain *Terrain,int size,int levels)
{
	bool res;
	cLog *Log = cLog::Instance();

	if(Terrain == NULL)
	{
		Log->Msg("Error trying to construct Quadtree from a NULL Terrain!");
		return false;
	}
	//--TODO: create root instance

	root = new cQuadtreeNode();

	if(root == NULL)
	{
		Log->Msg("Error allocating memory for Quadtree!");
		return false;
	}
	//--
	//-- TODO: load data
	res=root->Load(Terrain, levels,0,0,size,size);
	if(!res)
	{
		Log->Msg("Error loading Quadtree!");
		return false;
	}
	//--
	return true;
}
void cQuadtree::Render()
{
	//-- TODO: render
	root->Render();
	//--
}
void cQuadtree::RenderDebug()
{
	int mode[2];
	glColor3f(1.0f,0.0f,1.0f);
	glGetIntegerv(GL_POLYGON_MODE,mode);

	if(mode[0]!=GL_LINE) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	root->RenderDebug();

	if(mode[0]!=GL_LINE) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(1.0f,1.0f,1.0f);
}
void cQuadtree::Render(cFrustum *Frustum)
{
	cAABB *box = root->GetAABB();
	if(Frustum->TestInAABB(box)!=cFrustum::TEST_OUT)
	{
		root->Render(Frustum);
	}
}
void cQuadtree::ComputeIds(int size_node,int width_map)
{
	root->ComputeId(size_node,width_map);
}
cQuadtreeNode *cQuadtree::GetRoot(){return root;}