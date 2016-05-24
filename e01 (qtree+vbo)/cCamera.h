
#pragma once

#include <gl/glut.h>

#define PI			3.1415265359f
#define PIdiv180	0.0174529252f

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

#include "cVector3D.h"
#include "cInput.h"

class cCamera
{
private:
	cVector3D Position;
	cVector3D ViewDir;		/*Not used for rendering the camera, but for "moveforwards"
							So it is not necessary to "actualize" it always. It is only
							actualized when ViewDirChanged is true and moveforwards is called*/
	bool ViewDirChanged;
	GLfloat RotatedX, RotatedY, RotatedZ;

	int width,height;
	float speed;

	void GetViewDir (void);

public:
	cCamera();						
	void Init(int w,int h,float s);	//necessary for input updates
	void Look();					//glRotates and a glTranslate command
	
	void Rotate	(cVector3D Angles);
	void Update (cInput *Input);

	void GetPosition(cVector3D *pos);
	void GetRotated(cVector3D *rot);
	void GetViewDir(cVector3D *vdir);
	void SetPosition(cVector3D pos);
	void MoveForwards(GLfloat distance);

	void SetSpeed(float s);
	float GetSpeed();

private:
	void RotateX(GLfloat angle);
	void RotateY(GLfloat angle);
	void RotateZ(GLfloat angle);

	void StrafeRight(GLfloat distance);
};

