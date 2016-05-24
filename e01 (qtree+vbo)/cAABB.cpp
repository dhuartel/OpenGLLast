#include "cAABB.h"
#include <gl/glut.h>

cAABB::cAABB() {}
cAABB::~cAABB(){}

void cAABB::Render()
{
	RenderCube(Xo,Yo,-Zo,Xf,Yf,-Zf);
}
bool cAABB::IsInXZ(cVector3D *p)
{
	return ( (Xo <= p->x && p->x <= Xf) && (Zo <= p->z && p->z <= Zf) );
}
void cAABB::RenderCube(float Xmin,float Ymin,float Zmin,float Xmax,float Ymax,float Zmax)
{
	glBegin(GL_QUADS);
		//TopFace
		glNormal3f(0.0f,1.0f,0.0f);
		glVertex3f(Xmin,Ymax,Zmin);
		glVertex3f(Xmin,Ymax,Zmax);
		glVertex3f(Xmax,Ymax,Zmax);
		glVertex3f(Xmax,Ymax,Zmin);
		//BottomFace
		glNormal3f(0.0f,-1.0f,0.0f);
		glVertex3f(Xmin,Ymin,Zmin);
		glVertex3f(Xmax,Ymin,Zmin);
		glVertex3f(Xmax,Ymin,Zmax);
		glVertex3f(Xmin,Ymin,Zmax);
		//FrontFace
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(Xmin,Ymin,Zmax);
		glVertex3f(Xmax,Ymin,Zmax);
		glVertex3f(Xmax,Ymax,Zmax);
		glVertex3f(Xmin,Ymax,Zmax);
		//BackFace
		glNormal3f(0.0f,0.0f,-1.0f);
		glVertex3f(Xmin,Ymin,Zmin);
		glVertex3f(Xmin,Ymax,Zmin);
		glVertex3f(Xmax,Ymax,Zmin);
		glVertex3f(Xmax,Ymin,Zmin);
		//Rightface
		glNormal3f(1.0f,0.0f,0.0f);
		glVertex3f(Xmax,Ymin,Zmin);
		glVertex3f(Xmax,Ymax,Zmin);
		glVertex3f(Xmax,Ymax,Zmax);
		glVertex3f(Xmax,Ymin,Zmax);
		//LeftFace
		glNormal3f(-1.0f,0.0f,0.0f);
		glVertex3f(Xmin,Ymin,Zmin);
		glVertex3f(Xmin,Ymin,Zmax);
		glVertex3f(Xmin,Ymax,Zmax);
		glVertex3f(Xmin,Ymax,Zmin);
	glEnd();
}
