
#include "cPlayer.h"
#include "cScene.h"
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

cPlayer::cPlayer() 
{
	Camera = NULL;
	Frustum = NULL;
}
cPlayer::~cPlayer()
{
	if(Camera!=NULL)
	{
		delete Camera;
		Camera = NULL;
	}
	if(Frustum!=NULL)
	{
		delete Frustum;
		Frustum = NULL;
	}
}

void cPlayer::Init(int w,int h)
{
	Camera = new cCamera();
	Camera->Init(w,h,STEP_LENGTH);

	Frustum = new cFrustum();

	is_updated = false;
}
void cPlayer::Look()
{
	Camera->Look();
}
void cPlayer::Update(cInput *Input)
{
	cVector3D p1,p2,r1,r2;
	Camera->GetPosition(&p1);
	Camera->GetRotated(&r1);

	Camera->Update(Input);

	Camera->GetPosition(&p2);
	Camera->GetRotated(&r2);

	ComputeIfUpdated(p1,p2);

	glPushMatrix();
		glLoadIdentity();
		Look();
		UpdateFrustum();
	glPopMatrix();
}
void cPlayer::ComputeIfUpdated(cVector3D p1,cVector3D p2)
{
	is_updated = ( ((int)p1.x) != ((int)p2.x) ) || ( ((int)p1.z) != ((int)p2.z) );
}
bool cPlayer::GetUpdated()
{
	return is_updated;
}
void cPlayer::SetUpdated(bool b)
{
	is_updated = b;
}
void cPlayer::Move(cVector3D pos)
{
	Camera->SetPosition(pos);
}
void cPlayer::Rotate(cVector3D rot)
{
	Camera->Rotate(rot);
}
void cPlayer::GetPosition(cVector3D *pos)
{
	Camera->GetPosition(pos);
}
void cPlayer::GetRotated(cVector3D *rot)
{
	Camera->GetRotated(rot);
}
void cPlayer::GetViewDir(cVector3D *vdir)
{
	Camera->GetViewDir(vdir);
}
void cPlayer::SetSpeed(float speed)
{
	Camera->SetSpeed(speed);
}
float cPlayer::GetSpeed()
{
	return Camera->GetSpeed();
}
cFrustum* cPlayer::GetFrustum()
{
	if(!Frustum->IsInitialized())
	{
		Frustum->Extract();
	}
	return Frustum;
}
void cPlayer::UpdateFrustum()
{
	cVector3D obs;
	Camera->GetPosition(&obs);

	Frustum->Extract();
	Frustum->ComputePyramid(&obs);
}
void cPlayer::RenderFrustum()
{
	Frustum->Render();
}
