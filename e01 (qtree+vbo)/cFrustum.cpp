
#include "cFrustum.h"
#include <gl/glut.h>
#include "cVector3D.h"

cFrustum::cFrustum()
{
	initialized = false;
}
cFrustum::~cFrustum(){}

void cFrustum::Render()
{
	RenderPyramid();
}

cFrustum::frustum_test cFrustum::TestInAABB(cAABB *box)
{
	int i;
	bool in;

	in = true;
	for(i=0;i<6;i++)
	{
		if(eq[i][A] * box->Xo + eq[i][B] * box->Yo - eq[i][C] * box->Zo + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xf + eq[i][B] * box->Yo - eq[i][C] * box->Zo + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xo + eq[i][B] * box->Yf - eq[i][C] * box->Zo + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xf + eq[i][B] * box->Yf - eq[i][C] * box->Zo + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xo + eq[i][B] * box->Yo - eq[i][C] * box->Zf + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xf + eq[i][B] * box->Yo - eq[i][C] * box->Zf + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xo + eq[i][B] * box->Yf - eq[i][C] * box->Zf + eq[i][D] > 0) { in = false; continue; }
		if(eq[i][A] * box->Xf + eq[i][B] * box->Yf - eq[i][C] * box->Zf + eq[i][D] > 0) { in = false; continue; }

		// If we get here, it isn't in the frustum
		return TEST_OUT;
	}
	// Return a true for the box being inside of the frustum
	if(in)	return TEST_IN;
	else	return TEST_INTERSECT;
}
cFrustum::frustum_test cFrustum::TestInSphere(cVector3D p, float r)
{
	cVector3D n;
	float d;

	for(int i=0;i<6;i++) 
	{
		n.x = eq[i][A];
		n.y = eq[i][B];
		n.z = eq[i][C];

		d = Dot(n,p);
		if( (d + eq[i][D]) < -r)	return TEST_OUT;
	}
	return TEST_INTERSECT; //or IN
}
void cFrustum::Extract()
{
	float proj[16];
    float modl[16];
    float clip[16];
    float t;

    // Get the current PROJECTION matrix from OpenGL
    glGetFloatv(GL_PROJECTION_MATRIX, proj);

    // Get the current MODELVIEW matrix from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modl);

    // Combine the two matrices (multiply projection by modelview)
    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

    /* Extract the numbers for the RIGHT plane */
    eq[PLANE_RIGHT][A] = clip[ 3] - clip[ 0];
    eq[PLANE_RIGHT][B] = clip[ 7] - clip[ 4];
    eq[PLANE_RIGHT][C] = clip[11] - clip[ 8];
    eq[PLANE_RIGHT][D] = clip[15] - clip[12];

    /* Normalize the result */
    t = sqrt( eq[PLANE_RIGHT][A] * eq[PLANE_RIGHT][A] + eq[PLANE_RIGHT][B] * eq[PLANE_RIGHT][B] + eq[PLANE_RIGHT][C] * eq[PLANE_RIGHT][C] );
    eq[PLANE_RIGHT][A] /= t;
    eq[PLANE_RIGHT][B] /= t;
    eq[PLANE_RIGHT][C] /= t;
    eq[PLANE_RIGHT][D] /= t;

    /* Extract the numbers for the LEFT plane */
    eq[PLANE_LEFT][A] = clip[ 3] + clip[ 0];
    eq[PLANE_LEFT][B] = clip[ 7] + clip[ 4];
    eq[PLANE_LEFT][C] = clip[11] + clip[ 8];
    eq[PLANE_LEFT][D] = clip[15] + clip[12];

    /* Normalize the result */
    t = sqrt( eq[PLANE_LEFT][A] * eq[PLANE_LEFT][A] + eq[PLANE_LEFT][B] * eq[PLANE_LEFT][B] + eq[PLANE_LEFT][C] * eq[PLANE_LEFT][C] );
    eq[PLANE_LEFT][A] /= t;
    eq[PLANE_LEFT][B] /= t;
    eq[PLANE_LEFT][C] /= t;
    eq[PLANE_LEFT][D] /= t;

    /* Extract the BOTTOM plane */
    eq[PLANE_BOTTOM][A] = clip[ 3] + clip[ 1];
    eq[PLANE_BOTTOM][B] = clip[ 7] + clip[ 5];
    eq[PLANE_BOTTOM][C] = clip[11] + clip[ 9];
    eq[PLANE_BOTTOM][D] = clip[15] + clip[13];

    /* Normalize the result */
    t = sqrt( eq[PLANE_BOTTOM][A] * eq[PLANE_BOTTOM][A] + eq[PLANE_BOTTOM][B] * eq[PLANE_BOTTOM][B] + eq[PLANE_BOTTOM][C] * eq[PLANE_BOTTOM][C] );
    eq[PLANE_BOTTOM][A] /= t;
    eq[PLANE_BOTTOM][B] /= t;
    eq[PLANE_BOTTOM][C] /= t;
    eq[PLANE_BOTTOM][D] /= t;

    /* Extract the TOP plane */
    eq[PLANE_TOP][A] = clip[ 3] - clip[ 1];
    eq[PLANE_TOP][B] = clip[ 7] - clip[ 5];
    eq[PLANE_TOP][C] = clip[11] - clip[ 9];
    eq[PLANE_TOP][D] = clip[15] - clip[13];

    /* Normalize the result */
    t = sqrt( eq[PLANE_TOP][A] * eq[PLANE_TOP][A] + eq[PLANE_TOP][B] * eq[PLANE_TOP][B] + eq[PLANE_TOP][C] * eq[PLANE_TOP][C] );
    eq[PLANE_TOP][A] /= t;
    eq[PLANE_TOP][B] /= t;
    eq[PLANE_TOP][C] /= t;
    eq[PLANE_TOP][D] /= t;

    /* Extract the FAR plane */
    eq[PLANE_FAR][A] = clip[ 3] - clip[ 2];
    eq[PLANE_FAR][B] = clip[ 7] - clip[ 6];
    eq[PLANE_FAR][C] = clip[11] - clip[10];
    eq[PLANE_FAR][D] = clip[15] - clip[14];

    /* Normalize the result */
    t = sqrt( eq[PLANE_FAR][A] * eq[PLANE_FAR][A] + eq[PLANE_FAR][B] * eq[PLANE_FAR][B] + eq[PLANE_FAR][C] * eq[PLANE_FAR][C] );
    eq[PLANE_FAR][A] /= t;
    eq[PLANE_FAR][B] /= t;
    eq[PLANE_FAR][C] /= t;
    eq[PLANE_FAR][D] /= t;

    /* Extract the NEAR plane */
    eq[PLANE_NEAR][A] = clip[ 3] + clip[ 2];
    eq[PLANE_NEAR][B] = clip[ 7] + clip[ 6];
    eq[PLANE_NEAR][C] = clip[11] + clip[10];
    eq[PLANE_NEAR][D] = clip[15] + clip[14];

    /* Normalize the result */
    t = sqrt( eq[PLANE_NEAR][A] * eq[PLANE_NEAR][A] + eq[PLANE_NEAR][B] * eq[PLANE_NEAR][B] + eq[PLANE_NEAR][C] * eq[PLANE_NEAR][C] );
    eq[PLANE_NEAR][A] /= t;
    eq[PLANE_NEAR][B] /= t;
    eq[PLANE_NEAR][C] /= t;
    eq[PLANE_NEAR][D] /= t;

	initialized = true;
}
void cFrustum::ComputePyramid(cVector3D *obs)
{
	pyramid[0] = *obs;
	pyramid[1] = Interesection3Planes(PLANE_LEFT, PLANE_TOP,   PLANE_FAR);
	pyramid[2] = Interesection3Planes(PLANE_RIGHT,PLANE_TOP,   PLANE_FAR);
	pyramid[3] = Interesection3Planes(PLANE_RIGHT,PLANE_BOTTOM,PLANE_FAR);
	pyramid[4] = Interesection3Planes(PLANE_LEFT, PLANE_BOTTOM,PLANE_FAR);
}
cVector3D cFrustum::Interesection3Planes(plane p1,plane p2,plane p3)
{
	cVector3D n1,n2,n3;
	cVector3D p;

	n1.Set(eq[p1][A],eq[p1][B],eq[p1][C]);
	n2.Set(eq[p2][A],eq[p2][B],eq[p2][C]);
	n3.Set(eq[p3][A],eq[p3][B],eq[p3][C]);
	p = ( - eq[p1][D]*Cross(n2,n3) - eq[p2][D]*Cross(n3,n1) - eq[p3][D]*Cross(n1,n2) ) / Dot(n1,Cross(n2,n3));

	return p;
}
void cFrustum::RenderPyramid()
{
	int i;
	int mode[2];
	glGetIntegerv(GL_POLYGON_MODE,mode);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_TRIANGLE_FAN);
		for(i=0;i<5;i++)
		{
			glVertex3f(pyramid[i].x,pyramid[i].y,pyramid[i].z);
		}
		glVertex3f(pyramid[1].x,pyramid[1].y,pyramid[1].z);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4f(0.0f,0.0f,1.0f,0.5f);
	glBegin(GL_TRIANGLE_FAN);
		for(i=0;i<5;i++)
		{
			glVertex3f(pyramid[i].x,pyramid[i].y,pyramid[i].z);
		}
		glVertex3f(pyramid[1].x,pyramid[1].y,pyramid[1].z);
	glEnd();

	glDisable(GL_BLEND);
	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if(mode[0]==GL_LINE)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}
bool cFrustum::IsInitialized()
{
	return initialized;
}