
#pragma once

#include "cCamera.h"
#include "cVector3D.h"
#include "cFrustum.h"
#include "cInput.h"
#include "cTexture.h"

#define STEP_LENGTH			4

class cPlayer
{
public:
	cPlayer();
	virtual ~cPlayer();

	void Init(int w,int h);

	void Move(cVector3D pos);
	void Rotate(cVector3D rot);
	void Look();
	void Update(cInput *Input);

	void ComputeIfUpdated(cVector3D p1,cVector3D p2);
	bool GetUpdated();
	void SetUpdated(bool b);

	void GetPosition(cVector3D *pos);
	void GetRotated(cVector3D *rot);
	void GetViewDir(cVector3D *vdir);

	void SetSpeed(float speed);
	float GetSpeed();

	cFrustum *GetFrustum();
	void UpdateFrustum();
	void RenderFrustum();

private:
	cCamera *Camera;
	cFrustum *Frustum;

	bool is_updated;
};