
#include <math.h>
#include "cCamera.h"

cCamera::cCamera()
{
}
void cCamera::Init(int w,int h,float s)
{
	//Init with standard OGL values:
	Position = cVector3D(0.0f,0.0f, 0.0f);
	ViewDir  = cVector3D(0.0f,0.0f,-1.0f);
	ViewDirChanged = false;
	//Only to be sure:
	RotatedX = RotatedY = RotatedZ = 0.0f;
	//Screen
	width = w;
	height = h;
	//Step length
	SetSpeed(s);
}
void cCamera::Look()
{
	glRotatef(-RotatedX , 1.0f, 0.0f, 0.0f);
	glRotatef(-RotatedY , 0.0f, 1.0f, 0.0f);
	glRotatef(-RotatedZ , 0.0f, 0.0f, 1.0f);
	glTranslatef( -Position.x, -Position.y, -Position.z );
}

void cCamera::GetPosition(cVector3D *pos)
{
	*pos = Position;
}

void cCamera::SetPosition(cVector3D pos)
{
	Position.x = pos.x;
	Position.y = pos.y;
	Position.z = pos.z;
}
void cCamera::GetRotated(cVector3D *rot)
{
	*rot = cVector3D(RotatedX,RotatedY,RotatedZ);
}
void cCamera::GetViewDir(cVector3D *vdir)
{
	if(ViewDirChanged) GetViewDir();
	*vdir = ViewDir;
}
void cCamera::Rotate(cVector3D v)
{
	RotatedX = v.x;
	RotatedY = v.y;
	RotatedZ = v.z;
	
	//ViewDirChanged = true;
	GetViewDir();
}
void cCamera::SetSpeed(float s)
{
	speed = s;
}
float cCamera::GetSpeed()
{
	return speed;
}
void cCamera::Update(cInput *Input)
{
	int mx,my;
	int middleX,middleY;
	float angle;

	Input->GetMousePosition(&mx,&my);

	// Move the camera's view by the mouse
	middleX = width  >> 1;
	middleY = height >> 1;

	if(mx!=middleX)
	{
		angle = ((float)(middleX - mx)) / 50.0f;
		RotateY(angle);
	}
	if(my!=middleY)
	{
		angle = ((float)(middleY - my)) / 50.0f;
		RotateX(angle);
	}

	if(Input->KeyIsDown(GLUT_KEY_UP))		MoveForwards(-speed);				
	if(Input->KeyIsDown(GLUT_KEY_DOWN))		MoveForwards( speed);
	if(Input->KeyIsDown(GLUT_KEY_LEFT))		StrafeRight(-speed);
	if(Input->KeyIsDown(GLUT_KEY_RIGHT))	StrafeRight( speed);
}

void cCamera::GetViewDir(void)
{
	cVector3D Step1, Step2;
	//Rotate around Y-axis:
	Step1.x = cos( (RotatedY + 90.0f) * PIdiv180);
	Step1.z = -sin( (RotatedY + 90.0f) * PIdiv180);
	//Rotate around X-axis:
	double cosX = cos (RotatedX * PIdiv180);
	Step2.x = Step1.x * cosX;
	Step2.z = Step1.z * cosX;
	Step2.y = sin(RotatedX * PIdiv180);
	//Rotation around Z-axis not implemented, so:
	ViewDir = Step2;
}
void cCamera::RotateX (GLfloat angle)
{
	RotatedX += angle;
	ViewDirChanged = true;
}

void cCamera::RotateY (GLfloat angle)
{
	RotatedY += angle;
	ViewDirChanged = true;
}

void cCamera::RotateZ (GLfloat angle)
{
	RotatedZ += angle;
	ViewDirChanged = true;
}

void cCamera::MoveForwards( GLfloat distance )
{
	if (ViewDirChanged) GetViewDir();
	cVector3D MoveVector;
	MoveVector.x = ViewDir.x * -distance;
	MoveVector.y = ViewDir.y * -distance;
	MoveVector.z = ViewDir.z * -distance;
	Position += MoveVector;
}

void cCamera::StrafeRight ( GLfloat distance )
{
	if (ViewDirChanged) GetViewDir();
	cVector3D MoveVector;
	MoveVector.z = -ViewDir.x * -distance;
	MoveVector.y = 0.0;
	MoveVector.x = ViewDir.z * -distance;
	Position += MoveVector;
}
