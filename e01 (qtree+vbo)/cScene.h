
#pragma once

#include "cPlayer.h"
#include "cTerrain.h"
#include "cQuadtree.h"
#include "cTexture.h"

#define SPATIAL_SUBDIVISION_LEVELS	5

class cScene
{
public:
	cScene();
	virtual ~cScene();

	bool Init();
	void Render(cPlayer *Player,int show,int enable);

private:
	void SetupTopViewOrtho();
	void SetupTopViewPersp();

	void SetLights();
	void RenderCenter();
	void RenderAABB();
	void RenderAxis();

	cTerrain *Terrain;
	cQuadtree *Quadtree;
};
