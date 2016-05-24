#include "cQuadtreeNode.h"
#include <stdio.h>
#include "cTerrain.h"
#include "cLog.h"
#include "cRect.h"
#include "cQuadtree.h"

cQuadtreeNode::cQuadtreeNode()
{
	int i;
	for(i=0;i<4;i++) child[i] = NULL;

	Terrain = NULL;
}
cQuadtreeNode::~cQuadtreeNode()
{
	Release();
}
void cQuadtreeNode::Release()
{
	int i;
	for(i=0;i<4;i++)
	{
		if(child[i]!=NULL)
		{
			child[i]->Release();
			delete child[i];
			child[i] = NULL;
		}
	}
}
bool cQuadtreeNode::Load(cTerrain *T,int levels,int Xo,int Yo,int Xf,int Yf)
{
	int i,j,h;
	int Xm,Ym;
	bool res;
	
	Terrain = T;
	AABB.Xo = (float)Xo;	AABB.Zo = (float)Yo;
	AABB.Xf = (float)Xf;	AABB.Zf = (float)Yf;
	
	if(levels>1)
	{
		Xm = Xo+((Xf-Xo)/2);
		Ym = Yo+((Yf-Yo)/2);
		child[0] = new cQuadtreeNode();
		if(child[0] == NULL) return false;
		res = child[0]->Load(Terrain,levels-1,Xo,Yo,Xm,Ym);
		if(!res) return false;
		child[1] = new cQuadtreeNode();
		if(child[1] == NULL) return false;
		res = child[1]->Load(Terrain,levels-1,Xm,Yo,Xf,Ym);
		if(!res) return false;
		child[2] = new cQuadtreeNode();
		if(child[2] == NULL) return false;
		res = child[2]->Load(Terrain,levels-1,Xo,Ym,Xm,Yf);
		if(!res) return false;
		//--TODO: 3rd child
		child[3] = new cQuadtreeNode();
		if (child[3] == NULL) return false;
		res = child[3]->Load(Terrain, levels - 1, Xm, Ym, Xf, Yf);
		if (!res) return false;

		//--

		AABB.Yo = (float)child[0]->GetYmin();
		AABB.Yf = (float)child[0]->GetYmax();
		for(i=1;i<=3;i++)
		{
			h = child[i]->GetYmin();
			if(h<AABB.Yo) AABB.Yo = (float)h;
			//--TODO: update max value

			h = child[i]->GetYmax();
			if (h>AABB.Yf) AABB.Yf = (float)h;

			//--
		}
	}
	else
	{
		AABB.Yo = Terrain->GetHeight(Xo,Yo);
		AABB.Yf = AABB.Yo;
		for(i=Yo;i<=Yf;i++)
		{
			for(j=Xo;j<=Xf;j++)
			{
				//--TODO: get height, there aren't childs!
				h =  GetYmin();
				//--
				if(h<AABB.Yo) AABB.Yo = h;
				else if(h>AABB.Yf) AABB.Yf = h;
			}
		}
	}
	return true;
}
void cQuadtreeNode::RenderDebug()
{
	int i;
	
	if(!IsLeaf())
	{
		for(i=0;i<4;i++)
		{
			child[i]->RenderDebug();
		}
	}
	else AABB.Render();
}
void cQuadtreeNode::ComputeId(int size_node,int width_map)
{
	int x,y,n;

	//--TODO: what's that?
	if(IsLeaf())
	{		
		n = (int)pow(2,(double)size_node-1);
		x = AABB.Xo / (width_map/n);
		y = AABB.Zo / (width_map/n);

		id = x + (y * n);
	}
	else
	{
		child[0]->ComputeId(size_node,width_map);
		child[1]->ComputeId(size_node,width_map);
		child[2]->ComputeId(size_node,width_map);
		child[3]->ComputeId(size_node,width_map);
	}
	//--
}
void cQuadtreeNode::Render()
{
	int i;
	//--TODO: walk through Quadtree and test recursively
	if(IsLeaf())
	{
		RenderGeometry();
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			child[i]->RenderGeometry();
		}
	}
	//--
}
void cQuadtreeNode::Render(cFrustum *Frustum)
{
	cFrustum::frustum_test test;
	int i;

	//Walk through Quadtree and test recursively
	if(IsLeaf())
	{
		RenderGeometry();
	}
	else
	{
		for(i=0;i<4;i++)
		{
			test = Frustum->TestInAABB(child[i]->GetAABB());
			switch(test)
			{
				case cFrustum::TEST_OUT:		//Out
												break;
				case cFrustum::TEST_INTERSECT:	//Recusivity
												child[i]->Render(Frustum);
												break;
				case cFrustum::TEST_IN:			//Draw all
												child[i]->RenderGeometry();
												break;
			}
		}
	}
}
void cQuadtreeNode::RenderGeometry()
{
	//-- TODO: render terrain data
	if(IsLeaf())
	{
		//Render VBO node
		Terrain->RenderVBONode(id);
	}
	else
	{
		child[0]->RenderGeometry();
		child[1]->RenderGeometry();
		child[2]->RenderGeometry();
		child[3]->RenderGeometry();
	}
	//--
}
int cQuadtreeNode::GetYmin() {	return AABB.Yo;	}
int cQuadtreeNode::GetYmax() {	return AABB.Yf;	}
int cQuadtreeNode::GetXmin() {	return AABB.Xo;	}
int cQuadtreeNode::GetXmax() {	return AABB.Xf;	}
int cQuadtreeNode::GetZmin() {	return AABB.Zo;	}
int cQuadtreeNode::GetZmax() {	return AABB.Zf; }
bool cQuadtreeNode::IsLeaf() {	return child[0]==NULL; }
cAABB *cQuadtreeNode::GetAABB() { return &AABB; }

cQuadtreeNode *cQuadtreeNode::GetChild(int i)
{
	if(i<0 || i>3) return NULL;
	return child[i];
}
