#pragma once

#include "cVector3D.h"
#include "cAABB.h"

//frustum sphere length
#define FRUSTUM_LENGTH 2000.0

class cFrustum
{
public:
	cFrustum();
	~cFrustum();

	typedef enum { TEST_IN=0, TEST_OUT, TEST_INTERSECT } frustum_test;

	void Extract();
	void ComputePyramid(cVector3D *obs);

	void Render();

	frustum_test TestInAABB(cAABB *box);
	frustum_test TestInSphere(cVector3D p, float r);

	bool IsInitialized();

private:

	typedef enum { PLANE_LEFT=0, PLANE_RIGHT, PLANE_TOP, PLANE_BOTTOM, PLANE_NEAR, PLANE_FAR, PLANE_PRE, PLANE_POST_OUT, PLANE_POST_IN  } plane;
	enum { A=0, B, C, D };
	float eq[9][4];				//Implicit equation of a plane
	
	cVector3D points[8][4];		//4 points of each frustum plane (ZpostIN points = ZpostOUT points)
								//6 first planes = standard frustum

	cVector3D Interesection3Planes(plane p1,plane p2,plane p3);
	void RenderPyramid();

	cVector3D pyramid[5];
	bool initialized;
};
