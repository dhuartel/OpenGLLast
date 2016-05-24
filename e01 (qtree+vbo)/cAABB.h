#pragma once

#include "cVector3D.h"

class cAABB
{
public:
	cAABB();
	~cAABB();

	void Render();
	bool IsInXZ(cVector3D *p);

	float Xo,Yo,Zo,Xf,Yf,Zf;

private:
	void RenderCube(float Xmin,float Ymin,float Zmin,float Xmax,float Ymax,float Zmax);
};
