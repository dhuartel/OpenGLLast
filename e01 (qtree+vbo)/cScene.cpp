
#include "cScene.h"
#include "cLog.h"
#include "globals.h"
#include <gl/glut.h>
#include <string.h>
#include <math.h>

cScene::cScene()
{
	Terrain = NULL;
	Quadtree = NULL;
}
cScene::~cScene()
{
	#define freemem(a) { if(a!=NULL) { delete a; a = NULL; } }
	if(Terrain != NULL)
	{
		Terrain->Destroy();
		delete Terrain;
		Terrain = NULL;
	}
	if(Quadtree != NULL)
	{
		Quadtree->Release();
		delete Quadtree;
		Quadtree = NULL;
	}
}

bool cScene::Init()
{
	cLog *Log;

	Terrain = new cTerrain();
	if(!Terrain->Load())
	{
		Log = cLog::Instance();
		Log->Msg("Error loading Terrain.");
		return false;
	}

	Quadtree = new cQuadtree();
	if(!Quadtree->Load(Terrain,MAP_SIZE,SPATIAL_SUBDIVISION_LEVELS))
	{
		Log = cLog::Instance();
		Log->Msg("Error generating Quadtree.");
		return false;
	}

	Terrain->SetQuadtree(Quadtree);
	int map_width  = Terrain->GetWidth();
	Quadtree->ComputeIds(SPATIAL_SUBDIVISION_LEVELS,map_width);

	SetLights();

	return true;
}
void cScene::SetLights()
{
	GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
	GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };
    
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
    
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
    
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}
void cScene::Render(cPlayer *Player,int show,int enable)
{
	cVector3D obs;
	Player->GetPosition(&obs);

	//Camera setup
	if(show & SHOW_TOP_VIEW)
	{
		if(show & SHOW_ORTHO)				SetupTopViewOrtho();
		else								SetupTopViewPersp();
	}
	else
	{
		Player->Look();
	}
	//Terrain
	if(enable & ENABLE_FRUSTUM_TERRAIN)
	{
		Terrain->RenderInFrustum(Player->GetFrustum());
	}
	else
	{
		Terrain->Render();
	}
	//Axis
	if(show & SHOW_AXIS) {					RenderAABB();
											RenderCenter();
											RenderAxis();
	}
	//Quadtree
	if(show & SHOW_QUADTREE)				Quadtree->RenderDebug();
	//Frustum
	if(show & SHOW_FRUSTUM_TERRAIN)			Player->RenderFrustum();
}
void cScene::SetupTopViewOrtho()
{
	glMatrixMode(GL_PROJECTION);

		float offset_y = MAP_SIZE/8;
		float VIRTUAL_N = MAP_SIZE + offset_y;
		float offset_x = ((4.0f/3.0f) * VIRTUAL_N) - VIRTUAL_N;
		float Xo = -offset_x/2 -offset_y/2,
			  Xf = MAP_SIZE +offset_x/2 +offset_y/2,
			  Yo = -offset_y/2,
			  Yf = MAP_SIZE + offset_y/2;

		glLoadIdentity();
		glOrtho(Xo,Xf,Yo,Yf,0.1,2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(90,1,0,0);
	glTranslatef(0,-MAP_SIZE*1.5,0);
}
void cScene::SetupTopViewPersp()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,800.0/600.0, 1.0, 10000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(90,1,0,0);
	glTranslatef(-MAP_SIZE/2,-MAP_SIZE*1.5,MAP_SIZE/2);
}
void cScene::RenderAxis()
{
	glLineWidth(16.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(MAP_SIZE/2,0.0f,0.0f);
	glEnd();
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,MAP_SIZE/2,0.0f);
	glEnd();
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,-MAP_SIZE/2);
	glEnd();
	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(1.0f);
}
void cScene::RenderCenter()
{
	GLUquadric *ballQuadric = gluNewQuadric();
	gluQuadricOrientation(ballQuadric, GLU_OUTSIDE);
	gluQuadricNormals(ballQuadric, GLU_SMOOTH);
	gluQuadricTexture(ballQuadric, false);   

	glColor3f( 1.0f, 0.0f, 0.0f );
	gluSphere( ballQuadric, 16, 16, 16);

	gluDeleteQuadric( ballQuadric );
	glColor3f(1.0f, 1.0f, 1.0f);
}
void cScene::RenderAABB()
{
	int width = MAP_SIZE,
		height = MAP_SIZE,
		length = MAP_SIZE;
	int x=0,y=0,z=0;

	int mode[2];
	glGetIntegerv(GL_POLYGON_MODE,mode);
	if(mode[0]!=GL_LINE) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0.0f,0.0f,0.0f);

	glPushMatrix();
	glScalef(1.0f,1.0f,-1.0f);
	glBegin(GL_QUADS);		
		glVertex3f(x + width,	y,			z);
		glVertex3f(x + width,	y + height, z); 
		glVertex3f(x,			y + height, z);
		glVertex3f(x,			y,			z);
	
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y + height, z + length);
		glVertex3f(x + width,	y + height, z + length); 
		glVertex3f(x + width,	y,			z + length);

		glVertex3f(x,			y,			z);
		glVertex3f(x,			y,			z + length);
		glVertex3f(x + width,	y,			z + length); 
		glVertex3f(x + width,	y,			z);

		glVertex3f(x + width,	y + height, z);
		glVertex3f(x + width,	y + height, z + length); 
		glVertex3f(x,			y + height,	z + length);
		glVertex3f(x,			y + height,	z);

		glVertex3f(x,			y + height,	z);	
		glVertex3f(x,			y + height,	z + length); 
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y,			z);		

		glVertex3f(x + width,	y,			z);
		glVertex3f(x + width,	y,			z + length);
		glVertex3f(x + width,	y + height,	z + length); 
		glVertex3f(x + width,	y + height,	z);
	glEnd();
	glPopMatrix();

	if(mode[0]!=GL_LINE) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(1.0f,1.0f,1.0f);
}